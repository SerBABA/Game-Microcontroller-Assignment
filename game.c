#include <stdbool.h>
#include <string.h>

#include "system.h"
#include "pacer.h"
#include "ir_uart.h"
#include "game.h"
#include "interface.h"
#include "controls.h"
#include "timeout.h"

#define PACER_RATE 1000
#define INTERFACE_RATE 500
#define CONTROLS_RATE 200
#define IR_RECEIVING_RATE 300


static state_data current_state_data = {
    WAITING_TO_START,
    NULL,
    0,
    0,
    0,
    0,
    0,
    0
};


bool is_our_win(char our_choice, char their_choice)
{
    if (our_choice == PAPER && their_choice == ROCK) {
        return true;
    } else if (our_choice == ROCK && their_choice == SCISSORS) {
        return true;
    } else if (their_choice == PAPER) {
        return true;
    }
    return false;
}


void choose_letter(char choice)
{
    if (select_choice_push_event_p()) {
        ir_uart_putc(choice);
        current_state_data.current_game_state = WAITING_ON_RESPONSE;
        interface_clear();
    }
}


void receive_choice( const char options[], const uint8_t options_count)
{
    uint8_t i = 0;

    if (ir_uart_read_ready_p()) {
        current_state_data.their_choice = ir_uart_getc();
        for (i=0; i<options_count; i++) {
            if (current_state_data.their_choice == options[i]) {
                current_state_data.current_game_state = SHOWING_RESULTS;
                return;
            }
        }
    }
}


void button_task(const char options[], const uint8_t options_count)
{

    int8_t* our_choice_index = &current_state_data.our_choice_index;
    char* our_choice = &current_state_data.our_choice;

    if (continue_button_event_p()) {
        switch (current_state_data.current_game_state) {
        case WAITING_TO_START:
            current_state_data.current_game_state = SELECTING_CHOICE;
            break;

        default:
            reset_game();
        }
    }

    if (current_state_data.current_game_state == SELECTING_CHOICE) {
        cycle_choices(our_choice_index, options_count);
        *our_choice = options[*our_choice_index];
        choose_letter(*our_choice);
    }

}


void game_init(void)
{
    system_init();
    interface_init(INTERFACE_RATE);
    timeout_init(IR_RECEIVING_RATE);
    controls_init();
    ir_uart_init();
    pacer_init(PACER_RATE);
}


void reset_game(void)
{
    current_state_data.current_game_state = WAITING_TO_START;
    current_state_data.our_choice_index = 0;
    current_state_data.our_choice = 0;
    current_state_data.their_choice = 0;
    current_state_data.prev_char = 0;
    current_state_data.prev_string = NULL;
    interface_clear();
}


uint8_t round_result_code(void)
{
    char our_choice = current_state_data.our_choice;
    char their_choice = current_state_data.their_choice;

    if (our_choice == their_choice) {
        return TIE_CODE;
    } else if (is_our_win(our_choice, their_choice)) {
        return WIN_CODE;
    } else {
        return LOSE_CODE;
    }
}


void interface_task(void)
{

    char* curr_string = NULL;
    char curr_char = 0;
    uint8_t result = 0;
    bool is_string = true;

    switch(current_state_data.current_game_state) {

    case WAITING_TO_START:
        curr_string = WELCOME;
        break;


    case SELECTING_CHOICE:
        is_string = false;
        curr_char = current_state_data.our_choice;
        break;

    case WAITING_ON_RESPONSE:
        curr_string = WAITING;
        // remove lower line
        //current_state_data.current_game_state = SHOWING_RESULTS;
        break;

    case SHOWING_RESULTS:

        result = round_result_code();

        // remove lower line
        //result = WIN_CODE;

        if (result == WIN_CODE) {
            curr_string = WINNER;
        } else if (result == LOSE_CODE) {
            curr_string = LOSER;
        } else if (result == TIE_CODE) {
            curr_string = TIE;
        }

        if (interface_transition(INTERFACE_RATE)) {
            if (result == WIN_CODE) {
                current_state_data.our_score++;
            } else if (result == LOSE_CODE) {
                current_state_data.their_score++;
            }
            current_state_data.current_game_state = SHOWING_SCORES;
        }
        break;


    case SHOWING_SCORES:
        is_string = false;
        curr_char = current_state_data.our_score + '0';

        if (interface_transition(INTERFACE_RATE)) {
            current_state_data.current_game_state = SELECTING_CHOICE;
        }
        break;

    default:
        curr_string = "ERROR HAS OCCURED";
    }

    if (is_string) {
        if(interface_set_string(curr_string, current_state_data.prev_string)) {
            current_state_data.prev_string = curr_string;
        }
    } else {
        if(interface_set_character(curr_char, current_state_data.prev_char)) {
            current_state_data.prev_char = curr_char;
        }
    }

}


void ir_task(const char options[], const uint8_t options_count)
{
    if (current_state_data.current_game_state == WAITING_ON_RESPONSE ||
            current_state_data.current_game_state == SELECTING_CHOICE) {

        ir_receiver_timeout_init(IR_RECEIVING_RATE);
        receive_choice(options, options_count);
        if (ir_receiver_timeout()) {
            ir_uart_putc(current_state_data.our_choice);
        }
    }
}


int main (void)
{
    const char options[] = {PAPER, SCISSORS, ROCK};
    const uint8_t options_count = 3;

    uint8_t controls_tick = 0;
    uint8_t interface_tick = 0;
    uint8_t ir_tick = 0;

    uint8_t control_max_ticks = PACER_RATE/CONTROLS_RATE;
    uint8_t ir_max_ticks = PACER_RATE/IR_RECEIVING_RATE;
    uint8_t interface_max_ticks = PACER_RATE/INTERFACE_RATE;

    game_init();

    while (1) {
        pacer_wait();

        if (controls_tick >= control_max_ticks) {
            controls_update();
            button_task(options, options_count);
            controls_tick = 0;
        }

        if(ir_tick >= ir_max_ticks) {
            ir_task(options, options_count);
            ir_tick = 0;
        }

        if (interface_tick >= interface_max_ticks) {
            interface_task();
            interface_update();
            interface_tick = 0;
        }

        controls_tick++;
        interface_tick++;
        ir_tick++;

    }
}

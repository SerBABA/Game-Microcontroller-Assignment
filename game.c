#include <stdbool.h>
#include <string.h>
#include "system.h"
#include "pacer.h"
#include "ir_uart.h"
#include "game.h"
#include "interface.h"
#include "controls.h"
#include "timeout.h"
#include "communications.h"
#define PACER_RATE 1000
#define INTERFACE_RATE 500
#define CONTROLS_RATE 200
#define IR_RECEIVING_RATE 300
#define MAX_SCORE 3

static state_data current_state_data = {
    WAITING_TO_START,
    NULL, 0, 0, 0, 0, 0, 0, false,
};


static void game_init(void)
{
    system_init();
    interface_init(INTERFACE_RATE);
    timeout_init(PACER_RATE);
    communications_init(IR_RECEIVING_RATE);
    controls_init();
    ir_uart_init();
    pacer_init(PACER_RATE);
}


static void reset_game(void)
{
    current_state_data.current_game_state = WAITING_TO_START;
    current_state_data.our_choice_index = 0;
    current_state_data.our_choice = 0;
    current_state_data.their_choice = 0;
    current_state_data.prev_char = 0;
    current_state_data.prev_string = NULL;
    current_state_data.got_response = false;
    current_state_data.our_score = 0;
    current_state_data.their_score = 0;
    interface_clear();
}


static bool is_our_win(char our_choice, char their_choice)
{
    if (our_choice == PAPER && their_choice == ROCK) {
        return true;
    } else if (our_choice == ROCK && their_choice == SCISSORS) {
        return true;
    } else if (our_choice == SCISSORS && their_choice == PAPER) {
        return true;
    }
    return false;
}


static void choose_letter(char choice)
{
    if (select_choice_push_event_p() && ir_uart_write_ready_p()) {
        ir_uart_putc(choice);
        current_state_data.current_game_state = WAITING_ON_RESPONSE;
        interface_clear();
    }
}


static void button_task(const char options[], const uint8_t options_count)
{
    int8_t* our_choice_index = &current_state_data.our_choice_index;
    char* our_choice = &current_state_data.our_choice;

    if (continue_button_event_p()) {
        switch (current_state_data.current_game_state) {
        case WAITING_TO_START:
            current_state_data.current_game_state = SELECTING_CHOICE;
            break;

        case VICTORY_SCREEN:
            current_state_data.current_game_state = WAITING_TO_START;
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


static uint8_t round_result_code(void)
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


static bool game_is_over(void)
{
    return current_state_data.our_score >= MAX_SCORE || current_state_data.their_score >= MAX_SCORE;
}


static bool our_victory(void)
{
    return current_state_data.our_score >= MAX_SCORE;
}


static void interface_task(void)
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
        break;

    case SHOWING_RESULTS:

        result = round_result_code();

        curr_string = interface_display_round_result(result);
        if (curr_string == NULL) {
            current_state_data.current_game_state = RESET;
        } else {
            if (interface_transition(INTERFACE_RATE)) {
                if (result == WIN_CODE) {
                    current_state_data.our_score++;
                } else if (result == LOSE_CODE) {
                    current_state_data.their_score++;
                }
                current_state_data.current_game_state = SHOWING_SCORES;
                current_state_data.got_response = false;
            }
        }
        break;


    case SHOWING_SCORES:
        is_string = false;
        curr_char = current_state_data.our_score + '0';

        if (interface_transition(INTERFACE_RATE)) {
            clear_ir_buffer();
            if (game_is_over()) {
                current_state_data.current_game_state = VICTORY_SCREEN;
            } else {
                current_state_data.current_game_state = SELECTING_CHOICE;
            }
        }
        break;

    case VICTORY_SCREEN:

        curr_string = interface_display_game_result(our_victory());

        if (interface_transition(INTERFACE_RATE)) {
            reset_game();
            current_state_data.current_game_state = WAITING_TO_START;
        }
        break;

    case RESET:
        curr_string = RESETTING;

        if (interface_transition(INTERFACE_RATE)) {
            reset_game();
            current_state_data.current_game_state = WAITING_TO_START;
        }
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


static void ir_task(const char options[], const uint8_t options_count)
{
    bool got_response = current_state_data.got_response;

    if (got_response && current_state_data.current_game_state == WAITING_ON_RESPONSE) {
        current_state_data.prev_string = NULL;
        current_state_data.got_response = false;
        clear_ir_buffer();
        ir_uart_putc(current_state_data.our_choice);
        current_state_data.current_game_state = SHOWING_RESULTS;
    } else if (!got_response) {

        if (current_state_data.current_game_state == SELECTING_CHOICE) {
            got_response = ir_recev_choice(&current_state_data.their_choice, options, options_count);

        } else if (current_state_data.current_game_state == WAITING_ON_RESPONSE) {
            got_response = ir_recev_choice_and_timeout(&current_state_data.their_choice,
                           current_state_data.our_choice, options, options_count);
        }
        current_state_data.got_response = got_response;
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

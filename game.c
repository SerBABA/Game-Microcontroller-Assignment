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


/*
static game_state_t current_game_state = WAITING_TO_START;
*/

static state_data current_state_data = {
    SELECTING_CHOICE,
    NULL,
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


void reset_game(void)
{
    current_state_data.current_game_state = SELECTING_CHOICE;
    current_state_data.our_choice_index = 0;
    current_state_data.our_choice = 0;
    current_state_data.their_choice = 0;
    current_state_data.prev_char = 0;
    current_state_data.prev_string = NULL;
    interface_clear();
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



void game_init(void)
{
    system_init ();
    interface_init(INTERFACE_RATE);
    timeout_init(IR_RECEIVING_RATE);
    controls_init();
    ir_uart_init();
    pacer_init (PACER_RATE);
}

void button_task(const char options[], const uint8_t options_count)
{

    int8_t* our_choice_index = &current_state_data.our_choice_index;
    char* our_choice = &current_state_data.our_choice;

    if (continue_button_event_p()) {
        reset_game();
    }
    if (current_state_data.current_game_state == SELECTING_CHOICE) {
        cycle_choices(our_choice_index, options_count);
        *our_choice = options[*our_choice_index];
        choose_letter(*our_choice);
    }
}


void interface_task(void)
{

/*
    char* our_choice = &current_state_data.our_choice;
    char* their_choice = &current_state_data.their_choice;
    char* prev_char = &current_state_data.prev_char;
    char* curr_string = current_state_data.curr_string;
    char* prev_string = current_state_data.prev_string;
    uint8_t* our_score = &current_state_data.our_score;
    uint8_t* their_score = &current_state_data.their_score;
*/

    switch(current_state_data.current_game_state) {

        case SELECTING_CHOICE:

            if(current_state_data.our_choice != current_state_data.prev_char) {
                interface_display_character(current_state_data.our_choice);
                current_state_data.prev_char = current_state_data.our_choice;
            }
            break;

        case WAITING_ON_RESPONSE:
            current_state_data.curr_string = WAITING;
            break;

        case SHOWING_RESULTS:
            if (current_state_data.our_choice == current_state_data.their_choice) {
                current_state_data.curr_string = TIE;
            } else if (is_our_win(current_state_data.our_choice, current_state_data.their_choice)) {
                current_state_data.curr_string = WINNER;
                current_state_data.our_score += 1;
            } else {
                current_state_data.curr_string = LOSER;
                current_state_data.their_score += 1;
            }
            break;

        default:
            interface_display_string("ERROR");

        }

        if (strcmp(current_state_data.curr_string, current_state_data.prev_string) != 0) {
            interface_display_string(current_state_data.curr_string);
            current_state_data.prev_string = current_state_data.curr_string;
        }

}



void ir_task(const char options[], const uint8_t options_count)
{
    if (current_state_data.current_game_state == WAITING_ON_RESPONSE || current_state_data.current_game_state == SELECTING_CHOICE) {
        ir_receiver_timeout_init();
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

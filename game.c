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

#define MAX_TIMOUT_SECONDS 5



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


void choose_letter(bool* wait_chosen_letter, char choice)
{
    if (select_choice_push_event_p()) {
        ir_uart_putc(choice);
        *wait_chosen_letter = false;
    } else {
        *wait_chosen_letter = true;
    }
}


void reset_game_choices(int8_t* our_choice_index, char* our_choice, char* their_choice)
{
    *our_choice_index = 0;
    *our_choice = 0;
    *their_choice = 0;
}


void reset_game_states(bool* wait_received_letter, bool* wait_chosen_letter)
{
    *wait_received_letter = true;
    *wait_chosen_letter = true;
}


void receive_choice(bool* wait_received_letter, char* their_choice, char options[], uint8_t options_count)
{
    static uint8_t i = 0;

    if (ir_uart_read_ready_p()) {
        *their_choice = ir_uart_getc();
        for (i=0; i<options_count; i++) {
            if (*their_choice == options[i]) {
                *wait_received_letter = false;
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




int main (void)
{
    char options[] = {PAPER, SCISSORS, ROCK};
    uint8_t options_count = 3;
    set_timeout_max_period(MAX_TIMOUT_SECONDS);

    char* curr_string = NULL;

    char* prev_string = NULL;
    char prev_char = 0;

    int8_t our_choice_index = 0;
    char our_choice = 0;
    char their_choice = 0;
    bool wait_chosen_letter = true;
    bool wait_received_letter = true;


    uint8_t controls_tick = 0;
    uint8_t interface_tick = 0;
    uint8_t ir_receiving_tick = 0;

    uint8_t control_max_ticks = PACER_RATE/CONTROLS_RATE;
    uint8_t ir_receiving_max_ticks = PACER_RATE/IR_RECEIVING_RATE;
    uint8_t interface_max_ticks = PACER_RATE/INTERFACE_RATE;


    uint8_t our_score = 0;
    uint8_t their_score = 0;

    game_init();

    while (1) {
        pacer_wait();



        if (controls_tick >= control_max_ticks) {

            controls_update();

            if (continue_button_event_p()) {
                reset_game_choices(&our_choice_index, &our_choice, &their_choice);
                reset_game_states(&wait_received_letter, &wait_chosen_letter);
                interface_clear();
                prev_char = 0;
                prev_string = NULL;
            }
            if (wait_chosen_letter) {
                cycle_choices(&our_choice_index, options_count);
                our_choice = options[our_choice_index];
                choose_letter(&wait_chosen_letter, our_choice);
                if(!wait_chosen_letter) {
                    interface_clear();
                }
            }

            controls_tick = 0;
        }



        // & timeout &
        // wait for letter
        if(ir_receiving_tick >= ir_receiving_max_ticks) {
            if (wait_received_letter) {
                receive_choice(&wait_received_letter, &their_choice, options, options_count);
                if (ir_receiver_timeout(&wait_received_letter)) {
                    curr_string = "TEST";
                }
            }
            ir_receiving_tick = 0;
        }



        if (interface_tick >= interface_max_ticks) {
            if (wait_chosen_letter) {
                if(our_choice != prev_char) {
                    interface_display_character(our_choice);
                    prev_char = our_choice;
                }
            } else {
                if (wait_received_letter) {
                    curr_string = WAITING;
                } else {
                    if (our_choice == their_choice) {
                        curr_string = TIE;
                    } else if (is_our_win(our_choice, their_choice)) {
                        curr_string = WINNER;
                        our_score++;
                    } else {
                        curr_string = LOSER;
                        their_score++;
                    }

                }

                if (strcmp(curr_string, prev_string) != 0) {
                    interface_display_string(curr_string);
                    prev_string = curr_string;
                }

            }

            interface_update();
            interface_tick = 0;
        }

        controls_tick++;
        interface_tick++;
        ir_receiving_tick++;

    }
}

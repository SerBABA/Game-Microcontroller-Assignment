#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "system.h"
#include "pacer.h"
#include "ir_uart.h"
#include "timer.h"

#include "game.h"
#include "interface.h"
#include "controls.h"

#define PACER_RATE 500
#define INTERFACE_RATE 500
#define CONTROLS_RATE 200
#define RECEIVING_RATE 250

uint8_t our_win(char our_choice, char their_choice)
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


void update_all(void) {
    interface_update();
    controls_update();
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


void cycle_choices(int* our_choice, int options_count) {
    if (cycle_up_event_p()) {
        *our_choice = (*our_choice + 1) % options_count;
    }

    if(cycle_down_event_p()) {
        *our_choice -= 1;
        if (*our_choice < 0) {
            *our_choice = options_count-1;
        }
    }
}


void game_init(void) {
    system_init ();

    interface_init(PACER_RATE);
    controls_init();
    ir_uart_init();
    timer_init ();

    pacer_init (PACER_RATE);
}





int main (void)
{
    char options[] = {PAPER, SCISSORS, ROCK};
    int options_count = 3;

    char* prev_string = NULL;
    char* curr_string = NULL;

    int i = 0;
    int our_choice = 0;
    char their_choice = 0;
    bool wait_chosen_letter = true;
    bool wait_received_letter = true;

    //bool game_started = false;
    //uint8_t score = 0;

    game_init();

    while (1) {
        pacer_wait();
        update_all();

        if (continue_button_event_p()) {
            wait_received_letter = 1;
            wait_chosen_letter = 1;
            our_choice = 0;
            their_choice = 0;
        }

        if (wait_chosen_letter) {
            cycle_choices(&our_choice, options_count);
            choose_letter(&wait_chosen_letter, options[our_choice]);
        }

        // & timeout &
        // wait for letter
        if (wait_received_letter) {
            if (ir_uart_read_ready_p()) {
                their_choice = ir_uart_getc();
                for (i=0; i<options_count; i++) {
                    if (their_choice == options[i]) {
                        wait_received_letter = false;
                    }
                }
            }
        }


        if (prev_string == NULL && curr_string==NULL) {
            curr_string = &options[our_choice];
        }

        // compare and display result
        if (wait_chosen_letter) {
            curr_string = &options[our_choice];
        } else if (wait_received_letter) {
            curr_string = WAITING;
        } else {
            if (options[our_choice] == their_choice) {
                curr_string = TIE;
            } else if (our_win(options[our_choice], their_choice)) {
                curr_string = WINNER;
            } else {
                curr_string = LOSER;
            }
        }

        if (strcmp(curr_string, prev_string) != 0) {
            interface_display_character(*curr_string);
            prev_string = curr_string;
        }

    }
}

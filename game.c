/** @file game.c
 *  @author Dan Ronen, Elvis Chen
 *  @date 15 October 2020
 *  @brief This is the main game file.
 * */
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

//  This defines the rate of the pacer (the time it waits between while loops).
#define PACER_RATE 1000
// This defines the rate at which the interface task occurs.
#define INTERFACE_RATE 500
// This defines the rate at which the controls task occurs.
#define CONTROLS_RATE 200
// This defines the rate at which the IR task occurs.
#define IR_RECEIVING_RATE 300
// This defines the max score the game goes up to.
#define MAX_SCORE 3
// This is the base our_choice_index value.
#define BASE_OUR_CHOICE_INDEX 0
// This is the base our_choice value.
#define BASE_OUR_CHOICE 0
// This is the base their_choice value.
#define BASE_THEIR_CHOICE 0
// This is the base prev_char value.
#define BASE_PREV_CHAR 0
// This is the base prev_string value.
#define BASE_PREV_STRING NULL
// This is the base got_response value.
#define BASE_GOT_RESPONSE false
// This is the base our_score value.
#define BASE_OUR_SCORE 0
// This is the base their_score value.
#define BASE_THEIR_SCORE 0

// This is used to store the current state data and keep track of it.
static state_data current_state_data = {
    WAITING_TO_START,
    BASE_PREV_STRING,
    BASE_PREV_CHAR,
    BASE_OUR_SCORE,
    BASE_THEIR_SCORE,
    BASE_OUR_CHOICE,
    BASE_THEIR_CHOICE,
    BASE_OUR_CHOICE_INDEX,
    BASE_GOT_RESPONSE,
};


/** This initiates the game.*/
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

/** This resets the game*/
static void reset_game(void)
{
    current_state_data.current_game_state = WAITING_TO_START;
    current_state_data.our_choice_index = BASE_OUR_CHOICE_INDEX;        // Most of the lines here simply reset the current state object.
    current_state_data.our_choice = BASE_OUR_CHOICE;
    current_state_data.their_choice = BASE_THEIR_CHOICE;
    current_state_data.prev_char = BASE_PREV_CHAR;
    current_state_data.prev_string = BASE_PREV_STRING;
    current_state_data.got_response = BASE_GOT_RESPONSE;
    current_state_data.our_score = BASE_OUR_SCORE;
    current_state_data.their_score = BASE_THEIR_SCORE;
    interface_clear();
}


/** This is used to determine if we won the round (given it is not a tie)
 *  @param our_choice is our chosen attack
 *  @param their_choice is their chosen attack
 *  @return true if we won the round. Otherwise false.*/
static bool is_our_win(char our_choice, char their_choice)
{
    if (our_choice == PAPER && their_choice == ROCK) {              // These are the only cases we win in.
        return true;                                                // all other cases are considered false
    } else if (our_choice == ROCK && their_choice == SCISSORS) {    // since we assume we are not in a tie.
        return true;
    } else if (our_choice == SCISSORS && their_choice == PAPER) {
        return true;
    }
    return false;
}


/** This is used to choose the character, send to the other player and move to the next state.
 *  @param choice is the choice we made.*/
static void choose_letter(char choice)
{
    if (select_choice_push_event_p() && ir_uart_write_ready_p()) {
        ir_uart_putc(choice);
        current_state_data.current_game_state = WAITING_ON_RESPONSE;
        interface_clear();                                              // We need to also clear the interface, to make sure it is ready
    }                                                                   // for the new text to appear. Otherwise they overlay.
}


/** This is used to perform all the tasks needed to be performed realting to the controls.
 *  @param options[] is the list of all possible choises to be made.
 *  @param options_count is the number of available choices that can be made.*/
static void controls_task(const char options[], const uint8_t options_count)
{
    int8_t* our_choice_index = &current_state_data.our_choice_index;    // This is only used to make it easier
    char* our_choice = &current_state_data.our_choice;                  // to read the code.


    if (continue_button_event_p()) {
        switch (current_state_data.current_game_state) {                // Depending on the current state the continue button
        case WAITING_TO_START:                                          // has different functionality.
            current_state_data.current_game_state = SELECTING_CHOICE;
            break;

        case VICTORY_SCREEN:
            current_state_data.current_game_state = WAITING_TO_START;
            break;

        default:
            reset_game();
        }
    }

    if (current_state_data.current_game_state == SELECTING_CHOICE) {    // This is used in the attack selection
        cycle_choices(our_choice_index, options_count);                 // phase, which you cycle thorugh the options and can choose them.
        *our_choice = options[*our_choice_index];
        choose_letter(*our_choice);
    }

}


/** This is used to get the round result code
 *  @return the corresponding code to the correct result of the game.*/
static uint8_t round_result_code(void)
{
    char our_choice = current_state_data.our_choice;        // This is not a pointer, as the values are not being updated. Only
    char their_choice = current_state_data.their_choice;    // consulted for what code to return. Again purely aesthetic reasons.

    if (our_choice == their_choice) {
        return TIE_CODE;
    } else if (is_our_win(our_choice, their_choice)) {
        return WIN_CODE;
    } else {
        return LOSE_CODE;
    }
}


/** This is used to determine if the game is over.
 *  @return true if either of the players have reached the max score. Otherwise false.*/
static bool game_is_over(void)
{
    return current_state_data.our_score >= MAX_SCORE || current_state_data.their_score >= MAX_SCORE;
}


/** Determines if we won the game.
 *  @return true if our score has reached the max score value.*/
static bool our_victory(void)
{
    return current_state_data.our_score >= MAX_SCORE;
}


/** This function performs all the functionality needed by the interface task.*/
static void interface_task(void)
{
    char* curr_string = NULL;           // These variables are used to store the current values.
    char curr_char = 0;                 // They get updated by the functions that use them and
    uint8_t result = 0;                 // effect the progression and interface.
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
        if (curr_string == NULL) {                              // If the round result is unknown the game will reset its values.
            current_state_data.current_game_state = RESET;      // The other player will need to reset manually.

        } else {

            if (interface_transition(INTERFACE_RATE)) {                 // This performs a transition and adds the a point to the winner of
                if (result == WIN_CODE) {                               // round.
                    current_state_data.our_score++;
                } else if (result == LOSE_CODE) {
                    current_state_data.their_score++;
                }
                current_state_data.current_game_state = SHOWING_SCORES;
            }
        }
        break;


    case SHOWING_SCORES:
        is_string = false;
        curr_char = current_state_data.our_score + '0';                 // We add the character '0' to make it display the score
                                                                        // in ASCII format. But it does have the limitation of being
        if (interface_transition(INTERFACE_RATE)) {                     // only able to show numbers between 0-9.Otherwise it become sother symbols.
            if (game_is_over()) {
                current_state_data.current_game_state = VICTORY_SCREEN;
            } else {
                current_state_data.current_game_state = SELECTING_CHOICE;
                clear_ir_buffer();                                          // Regarding why the IR buffer clear and the IR response are in the interface task
                current_state_data.got_response = false;                    // This is because it is running only once during the transition to the selecting state
            }                                                               // Since if they called more than once, we get stuck in a loop where each of the microcontrollers
        }                                                                   // wipes its memory before the other can receive it.
        break;

    case VICTORY_SCREEN:

        curr_string = interface_display_game_result(our_victory());

        if (interface_transition(INTERFACE_RATE)) {
            reset_game();
        }
        break;

    case RESET:
        curr_string = RESETTING;

        if (interface_transition(INTERFACE_RATE)) {
            reset_game();
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


/** This peforms most of the IR related tasks needed to be performed.
 *  @param options[] is the options available to be sent and received.
 *  @param options_count is the number of options available.*/
static void ir_task(const char options[], const uint8_t options_count)
{
    bool got_response = current_state_data.got_response;

    if (got_response && current_state_data.current_game_state == WAITING_ON_RESPONSE) {
        current_state_data.prev_string = NULL;
        current_state_data.got_response = false;                        // Once we are in the waiting on response state and got the response
        clear_ir_buffer();                                              // we need to wipe the buffer and set the got response to false.
        ir_uart_putc(current_state_data.our_choice);                    // and we send through the our_choice one more time, and show the results.
        current_state_data.current_game_state = SHOWING_RESULTS;
    } else if (!got_response) {

        if (current_state_data.current_game_state == SELECTING_CHOICE) {                                // If we don't have the response then we receive it.
            got_response = ir_recev_choice(&current_state_data.their_choice, options, options_count);   // using the funcions below.

        } else if (current_state_data.current_game_state == WAITING_ON_RESPONSE) {
            got_response = ir_recev_choice_and_timeout(&current_state_data.their_choice,
                           current_state_data.our_choice, options, options_count);
        }
        current_state_data.got_response = got_response;
    }


}


/** This is the main function that runs everything.*/
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
            controls_task(options, options_count);
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

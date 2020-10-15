/** @file communications.c
 *  @author Dan Ronen, Elvis Chen
 *  @date 14 October 2020
 *  @brief Handles the IR communications (mostly).
 * */
#include <stdbool.h>
#include "ir_uart.h"
#include "timeout.h"

static uint16_t ir_receiver_rate = 0; // This is used to store the rate at which
                                      // the IR receiver is called.

/** Checks whether the choice is within the given options.
 *  @param choice is the value you want to compare the possible choices.
 *  @param options is a list of character options to choose from.
 *  @param options_count is the number of options available.
 *  @return true if the options is within the options array. Otherwise false.*/
bool is_valid_option(char choice, const char options[], const uint8_t options_count)
{
    uint8_t i = 0;

    for (i=0; i<options_count; i++) {  // This functions simply loops through all
        if (options[i] == choice) {    // the options in the options array and
            return true;               // compares them. If it finds a match returns
        }                              // true. Otherwise false.
    }
    return false;
}


/** This function initilizes the communications module.
 *  @param update_rate is the rate at which the module is called.*/
void communications_init(uint16_t update_rate)
{
    ir_receiver_rate = update_rate;
}


/** Clears the IR buffer.*/
void clear_ir_buffer(void)
{
    if (ir_uart_read_ready_p()) {         // If there is an items in the buffer
        while (ir_uart_read_ready_p()) {  // it will loop through all of them and
            ir_uart_getc();               // "read" them, which discards the items.
            continue;
        }
    }
}


/** Receives the choice made by the other player. Without any timeouts.
 *  @param their_choice is a pointer to their_choice char.
 *  @param options is the available options to choose from.
 *  @param options_count is the number of available options to choose from.
 *  @return true if the IR received a choice and it is a valid option. Otherwise false.*/
bool ir_recev_choice(char* their_choice, const char options[], const uint8_t options_count)
{
    if (ir_uart_read_ready_p()) {
        while (ir_uart_read_ready_p()) {        // Reads the buffer and only until the last character
            *their_choice = ir_uart_getc();     // which is stored into the their_choice pointer.
        }
        return is_valid_option(*their_choice, options, options_count);
    }
    return false;
}


/** Receives the choice made by the other player. With a timeout.
 *  @param their_choice is a pointer to their_choice char.
 *  @param our_choice is the choice the player made, which will be sent automatically
 *         after a timeout.
 *  @param options is the available options to choose from.
 *  @param options_count is the number of available options to choose from.
 *  @return true if the IR received a choice and it is a valid option. Otherwise false.*/
bool ir_recev_choice_and_timeout(char* their_choice, char our_choice, const char options[], const uint8_t options_count)
{
    ir_receiver_timeout_init(ir_receiver_rate);                     // Initiates the timeout, which will only send our choice out
    if (ir_recev_choice(their_choice, options, options_count)) {    // once it reaches the timeout and then sends our message again.
        return true;
    } else if (ir_receiver_timeout() && ir_uart_write_ready_p()) {
        ir_uart_putc(our_choice);
    }
    return false;
}


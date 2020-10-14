/** @file communications.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 14 October 2020
 *  @brief Handles the IR communications (mostly).
 * */
#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H


/** This function initilizes the communications module.
 *  @param update_rate is the rate at which the module is called.*/
void communications_init(uint16_t update_rate);


/** Receives the choice made by the other player. Without any timeouts.
 *  @param their_choice is a pointer to their_choice char.
 *  @param options is the available options to choose from.
 *  @param options_count is the number of available options to choose from.
 *  @return true if the IR received a choice and it is a valid option. Otherwise false.*/
bool ir_recev_choice(char* their_choice, const char options[], const uint8_t options_count);


/** Receives the choice made by the other player. With a timeout.
 *  @param their_choice is a pointer to their_choice char.
 *  @param our_choice is the choice the player made, which will be sent automatically
 *         after a timeout.
 *  @param options is the available options to choose from.
 *  @param options_count is the number of available options to choose from.
 *  @return true if the IR received a choice and it is a valid option. Otherwise false.*/
bool ir_recev_choice_and_timeout(char* their_choice, char our_choice, const char options[], const uint8_t options_count);


/** Clears the IR buffer.*/
void clear_ir_buffer(void);

#endif

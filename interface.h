/** @file interface.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 8 October 2020
 *  @brief Interface Driver.
*/
#ifndef INTERFACE_H
#define INTERFACE_H

// This defines the PAPER character
#define PAPER 'P'
// This defines the SCISSORS character
#define SCISSORS 'S'
// This defines the ROCK character
#define ROCK 'R'
// This defines the round TIE message string
#define TIE "Tie"
// This defines the round WINNER message string
#define WINNER "Winner"
// This defines the round LOSER message string
#define LOSER "Loser"
// This defines the code used for a round tie
#define TIE_CODE 2
// This defines the code used for a round lose
#define LOSE_CODE 1
// This defines the code used for a round win
#define WIN_CODE 0

// This defines the WAITING message string
#define WAITING "WAITING"
// This defines the WELCOME message string
#define WELCOME "WELCOME PRESS THE BUTTON TO START!!!"
// This defines the RESETTING message string
#define RESETTING "RESETTING"
// This defines the GAME_LOSE message string
#define GAME_LOSE "NICE TRY L!"
// This defines the GAME_WIN message string
#define GAME_WIN "VICTORY!"


/** Sets the string we want to display on the screen.
 *  @param curr_string (current string) is the new string we want to display. or the current one.
 *  @param prev_string (previous string) is the string that is already displaying on the screen.
 *  @return true if the interface has updated. Otherwise false.*/
bool interface_set_string(char* curr_string, char* prev_string);


/** Sets the character to display on the screen.
 *  @param curr_char (current character) is the current (new) string we want to display.
 *  @param prev_char (previous character) is the previously set character on the screen.
 *  @return true if the interface was updated. Otherwise false.*/
bool interface_set_character(char prev_char, char curr_char);


/** Clears the interface elements.*/
void interface_clear(void);


/** Initializes the display.
 *  @param new_pacer_rate is the pacer rate which is
 *         for initializing the tinygl module.*/
void interface_init(uint16_t new_pacer_rate);


/** This is used to delay the transition between states
 *  @param interface_rate is the rate at which the function is called.
 *  @return true if the transition should occur. Otherwise false.*/
bool interface_transition(uint16_t interface_rate);


/** Updates the display.*/
void interface_update(void);


/** Gets the pointer to the correct game result. Depending if you won or not.
 *  @param our_victory defines if we won the game or the other player won.
 *  @return the character pointer to the correct game result string.*/
char* interface_display_game_result(bool our_victory);


/** This returns the curr_string, which describes the correct round result.
 *  @param result is the round result code.
 *  @return a pointer to the string that associated with that result code.
 *  If it is unknown we return null.*/
char* interface_display_round_result(uint8_t result);


#endif

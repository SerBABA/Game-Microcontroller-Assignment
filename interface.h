/** @file interface.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 8 October 2020
 *  @brief Interface Driver.
*/
#ifndef INTERFACE_H
#define INTERFACE_H


#define PAPER 'P'
#define SCISSORS 'S'
#define ROCK 'R'


#define TIE "Tie"
#define WINNER "Winner"
#define LOSER "Loser"
#define TIE_CODE 2
#define LOSE_CODE 1
#define WIN_CODE 0

#define WAITING "WAITING"
#define WELCOME "WELCOME PRESS THE BUTTON TO START!!!"
#define RESETTING "RESETTING"
#define GAME_LOSE "NICE TRY L!"
#define GAME_WIN "VICTORY!"

/** Display a string on the display.
 *  @param string is the string you want to display.*/
void interface_display_string(char string[]);


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


/** Displays a single chracter on the display.
 *  @param character is the character you want to display.*/
void interface_display_character(char character);


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

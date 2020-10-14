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

bool interface_set_string(char* curr_string, char* prev_string);

bool interface_set_character(char prev_char, char curr_char);

/** Displays a single chracter on the display.
 *  @param character is the character you want to display.*/
void interface_display_character(char character);


void interface_clear(void);


/** Initializes the display.
 *  @param new_pacer_rate is the pacer rate which is
 *         for initializing the tinygl module.*/
void interface_init(uint16_t new_pacer_rate);


/** Updates the display.*/
void interface_update(void);

char* interface_display_game_result(bool our_victory);
char* interface_display_round_result(uint8_t result);


#endif

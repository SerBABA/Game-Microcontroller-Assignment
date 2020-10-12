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
#define WAITING "WAITING"
#define WELCOME "WELCOME PRESS THE NAV DOWN TO START!!!"


/** Display a string on the display.
 *  @param string is the string you want to display.*/
void interface_display_string(char string[]);


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


#endif

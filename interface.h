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
#define WIN 'W'
#define LOSE 'L'
#define WAIT "WAIT FOR OTHER PLAYER"


/** Display a string on the display.
 *  @param string is the string you want to display.*/
void interface_display_string(const char* string);


/** Displays a single chracter on the display.
 *  @param character is the character you want to display.*/
void interface_display_character(const char character);


/** Initializes the display.
 *  @param new_pacer_rate is the pacer rate which is
 *         for initializing the tinygl module.*/
void interface_init(const int new_pacer_rate);


/** Updates the display.*/
void interface_update(void);


#endif

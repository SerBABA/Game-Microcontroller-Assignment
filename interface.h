/** @file interface.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 8 October 2020
 *  @brief Interface Driver.
*/
#ifndef INTERFACE_H
#define INTERFACE_H



/** Display a string on the display.
 *  @param string is the string you want to display.*/
void interface_display_string(char string[]);


/** Displays a single chracter on the display.
 *  @param character is the character you want to display.*/
void interface_display_character(char character);


/** Initializes the display.
 *  @param new_pacer_rate is the pacer rate which is
 *         for initializing the tinygl module.*/
void interface_init(int new_pacer_rate);


/** Updates the display.*/
void interface_update(void);


#endif

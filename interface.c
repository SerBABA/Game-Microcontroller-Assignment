/** @file interface.c
 *  @author Dan Ronen, Elvis Chen
 *  @date 8 October 2020
 *  @brief Interface Driver.
*/
#include "system.h"
#include "tinygl.h"
#include "display.h"
#include "../fonts/font5x7_1.h"

#define MESSAGE_RATE 10


static int pacer_rate = 0;


/** Initializes the display.
 *  @param new_pacer_rate is the pacer rate which is
 *         for initializing the tinygl module.*/
void interface_init(const int new_pacer_rate)
{
    pacer_rate = new_pacer_rate;
    tinygl_init (pacer_rate);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
}

/** Updates the display.*/
void interface_update(void)
{
    tinygl_update();
}


/** Displays a single chracter on the display.
 *  @param character is the character you want to display.*/
void interface_display_character(const char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


/** Display a string on the display.
 *  @param string is the string you want to display.*/
void interface_display_string(const char string[])
{
    tinygl_text(string);
}

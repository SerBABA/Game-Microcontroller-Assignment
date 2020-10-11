/** @file interface.c
 *  @author Dan Ronen, Elvis Chen
 *  @date 8 October 2020
 *  @brief Interface Driver.
*/
#include "system.h"
#include "tinygl.h"
#include "display.h"
#include "../fonts/font5x7_1.h"
#include "interface.h"

#define MESSAGE_RATE 10

static int pacer_rate = 0;
static tinygl_point_t pos;


/** Initializes the display.
 *  @param new_pacer_rate is the pacer rate which is
 *         for initializing the tinygl module.*/
void interface_init(int new_pacer_rate)
{
    pacer_rate = new_pacer_rate;
    tinygl_init (pacer_rate);
    tinygl_font_set (&font5x7_1);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);    
    //~ tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_text_speed_set (MESSAGE_RATE);
}


void interface_clear(void)
{
    tinygl_clear();
}


/** Updates the display.*/
void interface_update(void)
{
    tinygl_update();
}


/** Displays a single chracter on the display.
 *  @param character is the character you want to display.*/
void interface_display_character(char character)
{
    tinygl_draw_char(character, pos);
}


/** Display a string on the display.
 *  @param string is the string you want to display.*/
void interface_display_string(char string[])
{
    tinygl_text(string);
}

/** @file interface.c
 *  @author Dan Ronen, Elvis Chen
 *  @date 8 October 2020
 *  @brief Interface Driver.
*/
#include <string.h>

#include "system.h"
#include "tinygl.h"
#include "display.h"
#include "../fonts/font5x7_1.h"
#include "interface.h"
#include "timeout.h"


#define MESSAGE_RATE 20

static uint16_t pacer_rate = 0;
static tinygl_point_t pos;


/** Initializes the display.
 *  @param new_pacer_rate is the pacer rate which is
 *         for initializing the tinygl module.*/
void interface_init(uint16_t new_pacer_rate)
{
    pacer_rate = new_pacer_rate;
    tinygl_init (pacer_rate);
    tinygl_font_set (&font5x7_1);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
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

bool interface_set_string(char* curr_string, char* prev_string)
{
    if (strcmp(curr_string, prev_string) != 0) {
        interface_clear();
        interface_display_string(curr_string);
        return true;
    } else {
        return false;
    }
}

bool interface_set_character(char curr_char, char prev_char)
{
    if(curr_char != prev_char) {
        interface_clear();
        interface_display_character(curr_char);
        return true;
    } else {
        return false;
    }
}


char* interface_display_round_result(uint8_t result)
{
    char* curr_string = NULL;

    if (result == WIN_CODE) {
        curr_string = WINNER;
    } else if (result == LOSE_CODE) {
        curr_string = LOSER;
    } else if (result == TIE_CODE) {
        curr_string = TIE;
    } else {
        curr_string = NULL;
    }
    return curr_string;
}


char* interface_display_game_result(bool our_victory)
{
    if (our_victory) {
        return GAME_WIN;
    }
    return GAME_LOSE;
}


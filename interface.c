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

// Defines the rate at which the message goes through the screen.
#define MESSAGE_RATE 20 

static uint16_t pacer_rate = 0; // The rate that the timeout functions are called.

static tinygl_point_t pos;      // This is needed for displaying character, such that we don't
                                // have flickering.

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


/** Clears the interface elements.*/
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


/** Sets the string we want to display on the screen.
 *  @param curr_string (current string) is the new string we want to display. or the current one.
 *  @param prev_string (previous string) is the string that is already displaying on the screen.
 *  @return true if the interface has updated. Otherwise false.*/
bool interface_set_string(char* curr_string, char* prev_string)
{
    if (strcmp(curr_string, prev_string) != 0) {    // This compares the previous string and
        interface_clear();                          // the current (new) string, which if they
        interface_display_string(curr_string);      // differ we need to update the display.
        return true;
    } else {
        return false;
    }
}


/** Sets the character to display on the screen.
 *  @param curr_char (current character) is the current (new) string we want to display.
 *  @param prev_char (previous character) is the previously set character on the screen.
 *  @return true if the interface was updated. Otherwise false.*/
bool interface_set_character(char curr_char, char prev_char)
{
    if(curr_char != prev_char) {                // This is similar to the interface_set_string()
        interface_clear();                      // function, which only displays the new character
        interface_display_character(curr_char); // if it different then the last character.
        return true;
    } else {
        return false;
    }
}


/** This returns the curr_string, which describes the correct round result.
 *  @param result is the round result code.
 *  @return a pointer to the string that associated with that result code. 
 *  If it is unknown we return null.*/
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


/** This is used to delay the transition between states
 *  @param interface_rate is the rate at which the function is called.
 *  @return true if the transition should occur. Otherwise false.*/
bool interface_transition(uint16_t interface_rate)
{
    interface_delay_init(interface_rate);
    if (interface_delay()) {
        return true;
    } else {
        return false;
    }
}


/** Gets the pointer to the correct game result. Depending if you won or not.
 *  @param our_victory defines if we won the game or the other player won.
 *  @return the character pointer to the correct game result string.*/
char* interface_display_game_result(bool our_victory)
{
    if (our_victory) {
        return GAME_WIN;
    }
    return GAME_LOSE;
}


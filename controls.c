/** @file controls.c
 *  @author Dan Ronen, Evlis Chen
 *  @date 8 October 2020
 *  @brief Drives the controls.
 * */
#include <stdbool.h>
#include "system.h"
#include "button.h"
#include "navswitch.h"
#include "controls.h"

// This defines the lower bound of the index range.
#define LOWER_INDEX_BOUND 0
// This defines how much we increment each time we cycle through our options.
#define OPTIONS_INCREMENT 1

/** Has the cycle up option has been pressed.
 *  @return true if the cycle up option was chosen. Otherwise false.*/
bool cycle_up_event_p(void)
{
    return navswitch_push_event_p (NAVSWITCH_NORTH);
}


/** Has the cycle down option has been pressed.
 *  @return true if the cycle down option was chosen. Otherwise false.*/
bool cycle_down_event_p(void)
{
    return navswitch_push_event_p (NAVSWITCH_SOUTH);
}


/** has the continue button been pressed.
 *  @return true if the continue button was pressed.*/
bool continue_button_event_p (void)
{
    return button_push_event_p(BUTTON1);
}


/** Has the choice selection option has been pressed.
 *  @return true if the selection option button was pressed. Otherwise false.*/
bool select_choice_push_event_p(void)
{
    return navswitch_push_event_p (NAVSWITCH_PUSH);
}


/** Updates the controls modules.*/
void controls_update(void)
{
    navswitch_update();
    button_update();
}

/** Cycles between available options you can choose from.
 *  @param our_choice_index is the index within the avialble options range.
 *  @param options_count is the number of available options.*/
void cycle_choices(int8_t* our_choice_index, uint8_t options_count)
{
    if (cycle_up_event_p()) {
        *our_choice_index = (*our_choice_index+OPTIONS_INCREMENT) % options_count;

    } else if (cycle_down_event_p()) {                  // Depending on the direction chosen 
        *our_choice_index -= OPTIONS_INCREMENT;         // the index will increment in that direction
        if (*our_choice_index < LOWER_INDEX_BOUND) {
            *our_choice_index += options_count;
        }
    }
}


/** Initializes the controls. */
void controls_init(void)
{
    navswitch_init ();
    button_init();
}



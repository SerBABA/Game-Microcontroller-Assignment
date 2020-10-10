/** @file controls.c
 *  @author Dan Ronen, Evlis Chen
 *  @date 8 October 2020
 *  @brief Drives the controls.
 * */
#include <stdbool.h>
#include "system.h"
#include "button.h"
#include "navswitch.h"

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


/** Initializes the controls. */
void controls_init(void)
{
    navswitch_init ();
    button_init();
}



/** @file controls.c
 *  @author Dan Ronen, Evlis Chen
 *  @date 8 October 2020
 *  @brief Drives the controls.
 * */
#include <stdbool.h>
#include "button.h"
#include "navswitch.h"


bool

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

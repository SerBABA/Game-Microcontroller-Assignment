/** @file controls.h
 *  @author Dan Ronen, Evlis Chen
 *  @date 8 October 2020
 *  @brief Drives the controls.
 * */
#ifndef CONTROLS_H
#define CONTROLS_H


/** has the continue button been pressed.
 *  @return true if the continue button was pressed.*/
bool continue_button_event_p (void);


/** Has the choice selection option has been pressed.
 *  @return true if the selection option button was pressed. Otherwise false.*/
bool select_choice_push_event_p(void);


/** Cycles between available options you can choose from.
 *  @param our_choice_index is the index within the avialble options range.
 *  @param options_count is the number of available options.*/
void cycle_choices(int8_t* our_choice_index, uint8_t options_count);


/** Updates the controls modules.*/
void controls_update(void);


/** Initializes the controls. */
void controls_init(void);


#endif

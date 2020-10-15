/** @file timeout.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 11 October 2020
 *  @brief Provides timeout capabilities.
 * */
#ifndef TIMEOUT_H
#define TIMEOUT_H


typedef enum timeout_type {
    IR_TIMEOUT,
    INTERFACE_DELAY,
    EMPTY,
} timeout_type_t;


/** This initiates the interface delay.
 *  @param interface_rate is the rate at which the delay function will be called.*/
void interface_delay_init(uint16_t interface_rate);


/** This acts as the delay between interfaces.
 *  @return true only when the delay has been finished. occur.*/
bool interface_delay(void);


/** This initiates the timeout period between the IR resending messages.
 *  @param ir_rate is the rate at which the function is going to be called.*/
void ir_receiver_timeout_init(uint16_t ir_rate);     


/** IR receivers' timeout function.
 *  @return true if timeout reached and running. Otherwise false.*/
bool ir_receiver_timeout(void);


/** Initializes the timeout module.
 *  @param new_pacer_rate is the pacer rate defined for the game.*/
void timeout_init(uint16_t new_timeout_rate);


#endif

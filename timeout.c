/** @file timeout.c
 *  @author Dan Ronen, Elvis Chen
 *  @date 11 October 2020
 *  @brief Provides timeout capabilities.
 * */
#include <stdbool.h>
#include <stdlib.h>
#include "system.h"
#include "timeout.h"

// This defines the maximum amount of time the timeout can wait for.
#define MAX_PERIOD_TIMEOUT 10
// The timeout values are in seconds
#define INTERFACE_MAX_DELAY 7
// Defines the intial tick value the timeout should be.
#define BASE_TICK_VALUE 0

// Used to define whether or not the timeout is running.
static bool run = false;
// Used to keep track of the current timeout tick.
static uint16_t timeout_tick = BASE_TICK_VALUE;
// Used to define the maximum current timeout ticks for a timeout to occur.
static uint16_t current_timeout_period = 0;
// The rate at which the timeout is called.
static uint16_t timeout_rate = 0;
// Keeps track of the current timeout type.
static timeout_type_t current_timeout_type = EMPTY;


/** Used to determine whether or not the timeout was reached.
 *  @return true if the timeout has been reached. Otherwise false.*/
bool timeout_reached(void)
{
    return timeout_tick >= current_timeout_period;
}


/** Stops the timeout counter*/
static void stop_timeout_counter(void) {
    run = false;
}


/** Sets the current_timeout_period, which determines how long we are going
 *  to wait for.
 *  @param timeout_seconds is the new current_timeout_period (in seconds) we want to
 *  wait for.*/
void set_current_timeout_period(uint16_t timeout_seconds) {
    current_timeout_period = timeout_seconds * timeout_rate;
}


/** Used to determine whether or not the timeout is running.
 *  @return true if timout counter is running. Otherwise false.*/
bool timeout_is_running(void)
{
    return run;
}


/** Starts the timeout counter.*/
void start_timeout_counter(void)
{
    run = true;
}


/** Resets the timeout counter.*/
void clear_timeout_counter(void)
{
    timeout_tick = BASE_TICK_VALUE;
    stop_timeout_counter();
    current_timeout_type = EMPTY;
}


/** This initiates the interface delay.
 *  @param interface_rate is the rate at which the delay function will be called.*/
void interface_delay_init(uint16_t interface_rate)
{
    if (current_timeout_type != INTERFACE_DELAY) {
        clear_timeout_counter();
        current_timeout_type = INTERFACE_DELAY;
        timeout_rate = interface_rate;
        set_current_timeout_period(INTERFACE_MAX_DELAY);
        start_timeout_counter();
    }
}                                                            
                                                                    

/** This is timeout updating the tick.
 *  @return true if the timeout has been reached. Otherwise false.*/
bool timeout_update(void)
{
    if (timeout_is_running()) {
        if (timeout_reached()) {        // Once the timeout is reached.
            clear_timeout_counter();
            return true;
        } else {
            timeout_tick++;
        }
    } else {
        start_timeout_counter();
    }
    return false;
}


/** This acts as the delay between interfaces.
 *  @return true only when the delay has been finished. occur.*/
bool interface_delay(void)
{
    if (current_timeout_type == INTERFACE_DELAY) {  // This returns only true when the delay has been finished
        return timeout_update();                    // and we are in the INTERFACE_DELAY type of timeout.
    }
    return false;
}


/** This initiates the timeout period between the IR resending messages.
 *  @param ir_rate is the rate at which the function is going to be called.*/
void ir_receiver_timeout_init(uint16_t ir_rate)
{
    if (current_timeout_type != IR_TIMEOUT) {
        clear_timeout_counter();
        current_timeout_type = IR_TIMEOUT;                          // The period is randomised in order to make sure we don't    
        timeout_rate = ir_rate;                                     // keep sending and expecting to receive at the same time.
        set_current_timeout_period(rand() % MAX_PERIOD_TIMEOUT);    // It appears that it is a half-duplex system which shares
        start_timeout_counter();                                    // the buffer with the sending and receiving of data over IR.   
    }                                                               // This is my assumption at least.
}       

/** IR receivers' timeout function.
 *  @return true if timeout reached and running. Otherwise false.*/
bool ir_receiver_timeout(void)
{
    if (current_timeout_type == IR_TIMEOUT) {   // This returns only true when the delay has been finished
        return timeout_update();                // and we are in the TIMEOUT_DELAY type of timeout.
    }
    return false;

}


/** Initializes the timeout module.
 *  @param new_pacer_rate is the pacer rate defined for the game.*/
void timeout_init(uint16_t new_timeout_rate)
{
    timeout_rate = new_timeout_rate;
}

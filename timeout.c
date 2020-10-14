/** @file timeout.c
 *  @author Dan Ronen, Elvis Chen
 *  @date 11 October
 *  @brief Provides timeout capabilities.
 * */
#include <stdbool.h>
#include <stdlib.h>
#include "system.h"
#include "timeout.h"

#define MAX_PERIOD_TIMEOUT 10

static bool run = false;
static uint16_t timeout_tick = 0;
static uint16_t max_timeout_period = 0;
static uint16_t pacer_rate = 0;
static timeout_type_t current_timeout_type = EMPTY;


/** Used to determine whether or not the timeout was reached.
 *  @return true if the timeout has been reached. Otherwise false.*/
bool timeout_reached(void)
{
    return timeout_tick >= max_timeout_period;
}


/** Sets the max_timeout_period, which determines how long we are going
 *  to wait for.
 *  @param timeout_seconds is the new max_timeout (in seconds) we want to
 *  wait for.*/
void set_timeout_max_period(uint16_t timeout_seconds) {
    max_timeout_period = timeout_seconds * pacer_rate;
}


/** Used to determine whether or not the timeout is running.
 *  @return true if timout counter is running. Otherwise false.*/
bool timeout_is_running(void)
{
    return run;
}


/** Resets the timeout counter.*/
void clear_timeout_counter(void)
{
    timeout_tick = 0;
    stop_timeout_counter();
    current_timeout_type = EMPTY;
}


/** Starts the timeout counter.*/
void start_timeout_counter(void)
{
    run = true;
}



bool interface_transition(uint16_t interface_rate)
{
    interface_delay_init(interface_rate);
    if (interface_delay()) {
        return true;
    } else {
        return false;
    }
}


/** Stops the timeout counter*/
void stop_timeout_counter(void) {
    run = false;
}


void interface_delay_init(uint16_t interface_rate)
{
    if (current_timeout_type != INTERFACE_DELAY) {
        clear_timeout_counter();
        current_timeout_type = INTERFACE_DELAY;
        pacer_rate = interface_rate;
        set_timeout_max_period(INTERFACE_MAX_DELAY);
        start_timeout_counter();
    }
}


bool interface_delay(void)
{
    if (current_timeout_type == INTERFACE_DELAY) {
        return timeout_update();
    }
    return false;
}


void ir_receiver_timeout_init(uint16_t ir_rate)
{
    if (current_timeout_type != IR_TIMEOUT) {
        clear_timeout_counter();
        current_timeout_type = IR_TIMEOUT;
        pacer_rate = ir_rate;        
        set_timeout_max_period(rand() % MAX_PERIOD_TIMEOUT);
        start_timeout_counter();
    }
}


bool timeout_update(void)
{
    if (timeout_is_running()) {
        if (timeout_reached()) {
            timeout_tick = 0;
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

/** IR receivers' timeout function.
 *  @return true if timeout reached and running. Otherwise false.*/
bool ir_receiver_timeout(void)
{
    if (current_timeout_type == IR_TIMEOUT) {
        return timeout_update();
    }
    return false;

}


/** Initializes the timeout module.
 *  @param new_pacer_rate is the pacer rate defined for the game.*/
void timeout_init(uint16_t new_pacer_rate)
{
    pacer_rate = new_pacer_rate;
}

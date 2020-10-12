/** @file timeout.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 11 October
 *  @brief Provides timeout capabilities.
 * */
#ifndef TIMEOUT_H
#define TIMEOUT_H

/** Used to determine whether or not the timeout was reached.
 *  @return true if the timeout has been reached. Otherwise false.*/
bool timeout_reached(void);


/** Sets the max_timeout_period, which determines how long we are going
 *  to wait for.
 *  @param timeout_seconds is the new max_timeout (in seconds) we want to
 *  wait for.*/
void set_timeout_max_period(uint16_t timeout_seconds);


/** Used to determine whether or not the timeout is running.
 *  @return true if timout counter is running. Otherwise false.*/
bool timeout_is_running(void);


/** Resets the timeout counter.*/
void reset_timeout_counter(void);


/** IR receivers' timeout function.
 *  @return true if timeout reached and running. Otherwise false.*/
bool ir_receiver_timeout(bool wait_received_letter);


/** Starts the timeout counter.*/
void start_timeout_counter(void);


/** Stops the timeout counter*/
void stop_timeout_counter(void) ;


/** Increments the timeout counter by one tick.*/
void timeout_update(void);


/** Initializes the timeout module.
 *  @param new_pacer_rate is the pacer rate defined for the game.*/
void timeout_init(uint16_t new_pacer_rate);


#endif

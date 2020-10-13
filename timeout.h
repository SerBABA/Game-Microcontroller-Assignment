/** @file timeout.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 11 October
 *  @brief Provides timeout capabilities.
 * */
#ifndef TIMEOUT_H
#define TIMEOUT_H

/* The timeout values are in seconds*/
#define IR_MAX_TIMEOUT 5
#define INTERFACE_MAX_DELAY 6


typedef enum timeout_type {
    IR_TIMEOUT,
    INTERFACE_DELAY,
    EMPTY,
} timeout_type_t;


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
void clear_timeout_counter(void);



void ir_receiver_timeout_init(uint16_t ir_rate);



bool ir_receiver_timeout(void);


/** Starts the timeout counter.*/
void start_timeout_counter(void);


/** Stops the timeout counter*/
void stop_timeout_counter(void) ;


bool interface_transition(uint16_t interface_rate);

bool timeout_update(void);


void interface_delay_init(uint16_t interface_rate);


bool interface_delay(void);

/** Initializes the timeout module.
 *  @param new_pacer_rate is the pacer rate defined for the game.*/
void timeout_init(uint16_t new_pacer_rate);


#endif

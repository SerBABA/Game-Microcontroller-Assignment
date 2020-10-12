#ifndef GAME_H
#define GAME_H


typedef enum game_state{
    waiting_to_start,
    selecting_option,
    waiting_on_response,
    showing_results,
    scores,
    reset,
} game_state_t;

#endif

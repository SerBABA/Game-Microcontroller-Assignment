/** @file game.h
 *  @author Dan Ronen, Elvis Chen
 *  @date 15 October 2020
 *  @brief This is the main game file.
 * */
#ifndef GAME_H
#define GAME_H

// This is used to keep track of the current game state.
typedef enum game_state{
    WAITING_TO_START,
    SELECTING_CHOICE,
    WAITING_ON_RESPONSE,
    SHOWING_RESULTS,
    SHOWING_SCORES,
    RESET,
    VICTORY_SCREEN,
} game_state_t;


// This is used to store all the current state data.
typedef struct state_data_s {
    game_state_t current_game_state;
    char* prev_string;
    char prev_char;
    uint8_t our_score;
    uint8_t their_score;
    char our_choice;
    char their_choice;
    int8_t our_choice_index;
    bool got_response;
} state_data;


#endif

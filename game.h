#ifndef GAME_H
#define GAME_H


typedef enum game_state{
    //WAITING_TO_START,
    SELECTING_CHOICE,
    WAITING_ON_RESPONSE,
    SHOWING_RESULTS,
    SHOWING_SCORES,
    RESET,
    VICTORY_SCREEN,
} game_state_t;

typedef struct state_data_s {
    game_state_t current_game_state;
    char* curr_string;
    char* prev_string;
    char prev_char;
    uint8_t our_score;
    uint8_t their_score;
    char our_choice;
    char their_choice;
    int8_t our_choice_index;
} state_data;


#endif

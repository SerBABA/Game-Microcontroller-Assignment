#ifndef GAME_H
#define GAME_H


typedef enum game_state{
    WAITING_TO_START,
    SELECTING_CHOICE,
    WAITING_ON_RESPONSE,
    SHOWING_RESULTS,
    SHOWING_SCORES,
    RESET,
    VICTORY_SCREEN,
} game_state_t;

typedef struct state_data_s {
    game_state_t current_game_state;
    char* prev_string;
    char prev_char;
    uint8_t our_score;
    uint8_t their_score;
    char our_choice;
    char their_choice;
    int8_t our_choice_index;
} state_data;


bool is_our_win(char our_choice, char their_choice);


void choose_letter(char choice);


void receive_choice( const char options[], const uint8_t options_count);


void button_task(const char options[], const uint8_t options_count);


void game_init(void);


void reset_game(void);

#endif

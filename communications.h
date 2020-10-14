
#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

bool is_valid_option(char choice, const char options[], const uint8_t options_count);

void communications_init(uint16_t update_rate);

bool ir_recev_choice(char* their_choice, const char options[], const uint8_t options_count);

bool ir_recev_choice_and_timeout(char* their_choice, char our_choice, const char options[], const uint8_t options_count);


#endif

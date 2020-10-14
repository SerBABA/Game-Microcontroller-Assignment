
#include <stdbool.h>
#include "ir_uart.h"
#include "timeout.h"

static uint16_t ir_receiver_rate = 0;


bool is_valid_option(char choice, const char options[], const uint8_t options_count)
{
    uint8_t i = 0;

    for (i=0; i<options_count; i++) {
        if (options[i] == choice) {
            return true;
        }
    }
    return false;
}


void communications_init(uint16_t update_rate)
{
    ir_receiver_rate = update_rate;
}


void clear_ir_buffer(void)
{
    if (ir_uart_read_ready_p()) {
        while (ir_uart_read_ready_p()) {
            ir_uart_getc();
            continue;
        }
    }
}


bool ir_recev_choice(char* their_choice, const char options[], const uint8_t options_count)
{
    if (ir_uart_read_ready_p()) {
        while (ir_uart_read_ready_p()) {
            *their_choice = ir_uart_getc();
        }
        return is_valid_option(*their_choice, options, options_count);
    }
    return false;
}


bool ir_recev_choice_and_timeout(char* their_choice, char our_choice, const char options[], const uint8_t options_count)
{
    ir_receiver_timeout_init(ir_receiver_rate);
    if (ir_recev_choice(their_choice, options, options_count)) {
        return true;
    } else if (ir_receiver_timeout() && ir_uart_write_ready_p()) {
        ir_uart_putc(our_choice);
    }
    return false;
}


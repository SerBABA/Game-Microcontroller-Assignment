
#include <stdbool.h>
#include "ir_uart.h"
#include "timeout.h"

bool receive_choice(char their_choice, const char options[], const uint8_t options_count)
{
    uint8_t i = 0;

    if (ir_uart_read_ready_p()) {
        their_choice = ir_uart_getc();
        for (i=0; i<options_count; i++) {
            if (their_choice == options[i]) {
                return true;
            }
        }
    }
    return false;
}

bool get_opponent_choice(char their_choice, char our_choice, const char options[], const uint8_t options_count, uint16_t ir_receiveing_rate)
{
    ir_receiver_timeout_init(ir_receiveing_rate);
    if (receive_choice(their_choice, options, options_count) ) {
        return true;
    }

    if (ir_receiver_timeout()) {
        ir_uart_putc(our_choice);
    }
    return false;

}

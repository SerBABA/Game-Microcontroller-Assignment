#include "system.h"
#include "button.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"
#include <stdbool.h>

#define PACER_RATE 500
#define MESSAGE_RATE 10
#define TIMEOUT_LENGTH 5000

bool ourWin(char ourChoice, char theirChoice) {

    if (ourChoice == 'P' && theirChoice == 'R') {
        return TRUE;
    } else if (ourChoice == 'R' && theirChoice == 'S') {
        return TRUE;
    } else if (ourChoice == 'S' && theirChoice == 'P') {
        return TRUE;
    } else {
        return FALSE;
    }

}


int main (void)
{
    char options[3] = {'P', 'S', 'R'};
    int amtOptions = 3;
    int ourChoice = 0;
    int i = 0;
    char theirChoice = 0;
    bool waitChosenLetter = TRUE;
    bool waitReceivedLetter = TRUE;

    system_init ();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    navswitch_init ();


    while (1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();

        // choose our letter
        if (waitChosenLetter) {
            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                ourChoice = (ourChoice + 1) % amtOptions;
            }

            if(navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                ourChoice -= 1;
                if (ourChoice < 0) {
                    ourChoice = amtOptions-1;
                }
            }

            // send letter // add error checks
            if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                ir_uart_putc(options[ourChoice]);
                waitChosenLetter = FALSE;
            }
        }


        // & timeout &
        // wait for letter
        if (waitReceivedLetter) {

            if (ir_uart_read_ready_p()) {
                theirChoice = ir_uart_getc();
                for (i=0;i<amtChoice;i++) {
                        if (theirChoice == options[i]) {
                            waitReceivedLetter = FALSE;
                        }
                }
            }

        }


        // compare and display result
        if (waitChosenLetter) {
            display_character(options[ourChoice]);
        } else if (waitReceivedLetter) {
            display_character("X");
        } else {
            if (options[ourChoice] == theirChoice) {
                display_character("T");
            } else if (ourWin(options[ourChoice], theirChoice)) {
                display_character("W");
            } else {
                display_character("L");
            }
            // waitReceivedLetter = TRUE;
            // waitChosenLetter = TRUE;
        }


    }
}

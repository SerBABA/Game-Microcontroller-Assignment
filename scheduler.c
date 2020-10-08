#include "system.h"
#include "led.h"
#include "pacer.h"

#include "timer.h"

#define TIMEOUT_RATIO 200
#define PACER_RATE 50

void timeout (void)
{
    system_init ();
    led_init ();
    uint8_t timeout_tick = 0;
    
    /* Initialise timer.  */
    
    timer_init ();
    pacer_init(PACER_RATE)
    
    while (1)
    {
		pacer_wait()
		
        timeout_tick++;
        if (button_tick >= pacer_rate/TIMEOUT_RATIO) {
			waitReceivedLetter = 1;
            waitChosenLetter = 1;
            ourChoice = 0;
            theirChoice = 0;
            
            timeout_tick = 0;
        }
    }
}

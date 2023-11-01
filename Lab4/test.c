#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(void) {
   init();  //initialize board hardware
   motor_init();
   u08 state = FALSE;

    while(TRUE)
    {
        if(get_btn())
        {
            state = FALSE;
            _delay_ms(200);
        }
                //stop or start the motors
        if (state == FALSE)
        {
            motor_init(); //stop motors
            if(get_btn())
            {
                state = TRUE;
                _delay_ms(200);
            }
        }
        else
        {
            motor(LEFT, 50);
            motor(RIGHT, 0);
        }

    }
    

   return 0;
}

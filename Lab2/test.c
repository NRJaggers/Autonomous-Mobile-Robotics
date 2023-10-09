#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(void) {
   init();  //initialize board hardware

   motor_init();
// while(1)
// {
//    forward(50);
//    _delay_ms(1000);

//    reverse(50);
//    _delay_ms(1000);

//    spin(75);
//    _delay_ms(1000);
// }

   return 0;
}

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(void) {
   init();  //initialize board hardware

   motor_init();

   reverse(1);

   _delay_ms(1000);

   return 0;
}

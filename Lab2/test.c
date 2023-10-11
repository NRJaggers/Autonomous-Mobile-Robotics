#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(void) {
   init();  //initialize board hardware
   motor_init();

   int16_t count_p = 0;
   int16_t count_n = 0;

   while(1)
   {
      count_p++;
      count_n--;
      clear_screen();
      print_snum(count_p);
      lcd_cursor(0,1);
      print_snum(count_n);
      _delay_ms(50);
      
   }
   

   return 0;
}

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(void) {
   init();  //initialize board hardware
   motor_init();
   u16 upper_sensor_value; 

    clear_screen();
    lcd_cursor(0,0);
    print_string("RANGE:");
    lcd_cursor(0,1);

   while(1)
   {
      upper_sensor_value = analog(ANALOG5_PIN); //pick pin 5 or 2 or 1
      print_num(upper_sensor_value); 
   }

   return 0;
}

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
        //get range sensor value and refresh
        upper_sensor_value = analog(ANALOG2_PIN); //pick pin 5 or 2 or 1
        lcd_cursor(0,1);
        print_string("   ");
        lcd_cursor(0,1);
        print_num(upper_sensor_value);
        _delay_us(400);
   }

   return 0;
}

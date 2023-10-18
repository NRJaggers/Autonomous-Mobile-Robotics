#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(void) {
   init();  //initialize board hardware
   motor_init();
   u16 left_sensor_value, right_sensor_value; 

   while(1)
   {
      left_sensor_value = analog(ANALOG4_PIN); //need new second sensor   
      right_sensor_value = analog(ANALOG3_PIN);

      lcd_cursor(7-2,0);
      print_num(left_sensor_value);
      lcd_cursor(7-2,1);
      print_num(right_sensor_value); 
   }
   

   return 0;
}

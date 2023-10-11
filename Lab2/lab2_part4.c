/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 2 Part 4

Description: 

Sensor: https://www.pololu.com/product/2458

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(){

    init();
    motor_init();
    
    u08 state = 1;
    u08 left_sensor_value, right_sensor_value;
    u08 left_motor_speed, right_motor_speed;    
    
    while(1){

        if(get_btn() == 1){
            state = state ^ 1;
            clear_screen();
            _delay_ms(200);
        }
        
        left_sensor_value = analog(ANALOG4_PIN); //need new second sensor
        right_sensor_value = analog(ANALOG3_PIN); //"Lower output voltage is an indication of greater reflection."

        if(state){
            //start here
            lcd_cursor(0,0);
            print_num(left_sensor_value);
            lcd_cursor(0,1);
            print_num(right_sensor_value);

        }
        else
        {    
            motor_init();
        }

    
    }
    return 0;
}

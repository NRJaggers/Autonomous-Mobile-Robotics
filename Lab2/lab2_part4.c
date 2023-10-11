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
    
    u08 left_sensor_value, right_sensor_value;
    int8_t error, diff;
    int8_t diff_past = 0;

    const u08 Kp = 10;
    const u08 Kd = 2;
    while(1){

        
        left_sensor_value = analog(ANALOG4_PIN); //need new second sensor
        right_sensor_value = analog(ANALOG3_PIN); //"Lower output voltage is an indication of greater reflection."

        //start here
        
        lcd_cursor(0,0);
        print_num(left_sensor_value);
        lcd_cursor(0,1);
        print_num(right_sensor_value); 
        
        
        diff = (left_sensor_value - right_sensor_value);
        
        error = (Kp * diff) - (Kd * (diff - diff_past)); // want error between 0 and 100
        /*
        lcd_cursor(0,0);
        print_num(error);
        */
        if(error > 0){
            motor(RIGHT, 25 - error);
        }

        else{
            motor(LEFT, 25 - error);
        }

        diff_past = diff; 
       
    }
    return 0;
}

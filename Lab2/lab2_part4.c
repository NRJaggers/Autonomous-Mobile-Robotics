/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 2 Part 4

Description: 

Sensor: https://www.pololu.com/product/2458
"Lower output voltage is an indication of greater reflection."

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(){

    init();
motor_init();
    
    u16 left_sensor_value, right_sensor_value; //idea?: testing if using 16 bits gives better results
    int16_t error = 0; 
    int16_t diff = 0; 
    int16_t diff_past = 0;
    int16_t derivative;
    int8_t speed;

    const u08 Kp = 3;
    const u08 Kd = 1;
    
    motor(RIGHT, 10);
    motor(LEFT, 10);
    while(1)
    {

        
        left_sensor_value = analog(ANALOG4_PIN); //need new second sensor
        
        right_sensor_value = analog(ANALOG3_PIN); //"Lower output voltage is an indication of greater reflection."
        /*
        lcd_cursor(0,0);
        print_num(left_sensor_value);
        lcd_cursor(0,1);
        print_num(right_sensor_value); 
        */
        
        if(left_sensor_value > right_sensor_value) //left on black, right on white 
        {
            diff = (left_sensor_value - right_sensor_value);
            derivative = diff - diff_past;
            if(derivative > 100) {derivative = 0;}
            error = (Kp * diff) - (Kd * (derivative));

            //increase speed of right , decrease left

            motor(LEFT, 10 - (error/ 15));
            motor(RIGHT, 10 + (error/ 15));

        }
       
        else //right on black , eft on white
        {
            diff = (right_sensor_value - left_sensor_value);
             derivative = diff - diff_past;
            if(derivative > 100) {derivative = 0;}
            error = (Kp * diff) - (Kd * (derivative));

            //increase speed of right , decrease left

            motor(LEFT, 10 + (error/ 15));
            motor(RIGHT, 10 -(error/ 15) );
        }
       
       
       
       // diff = (left_sensor_value - right_sensor_value);
        
    //    error = (Kp * diff) - (Kd * (diff - diff_past) / 2); // want error between 0 and 10
        /*
        if(error > 0){
            speed = 10 - error; 
            }
        else{
            speed = 10 + error;
            }
        
        if (speed < 0){
            speed = 0; }
        
        if((220 > left_sensor_value && left_sensor_value > 180) && 
            (220 > right_sensor_value && right_sensor_value > 180) && 
            (-10 < diff && diff < 10)){

            motor(LEFT, 10);
            motor(RIGHT, 10);
        }
        */
        
        lcd_cursor(0,0);
        print_snum(error);
        /*
        else if(error > 0){
            
            motor(RIGHT, speed);
            motor(LEFT, 10);
        }

        else{
            motor(LEFT, speed);
            motor(LEFT, 10);

        }
        */
        diff_past = diff; 
       
    }
    return 0;
}

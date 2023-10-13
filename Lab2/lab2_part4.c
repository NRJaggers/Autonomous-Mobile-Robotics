/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 2 Part 4

Description: Line following bot implemented through PID like controll system.

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
    int16_t diff_past = 0; // make an array?
    int16_t diff_past2 = 0;
    int16_t derivative;
    int16_t sum;
    u08 mode = RIGHT;
    const u08 Kp = 6;
    const u08 Kd = 1;
    const u08 Ki = 3;


  //  motor(RIGHT, 10);
  //  motor(LEFT, 10);
    while(1)
    {

        
        left_sensor_value = analog(ANALOG4_PIN); //need new second sensor
        
        right_sensor_value = analog(ANALOG3_PIN); //"Lower output voltage is an indication of greater reflection."
        
        diff = (left_sensor_value - right_sensor_value);
       
        // if(left_sensor_value > 190 && right_sensor_value < 188){
        //     motor(RIGHT, -10);
        //     motor(LEFT, 20);
        //      _delay_ms(250);
        // }
        // else if(left_sensor_value < 188 && right_sensor_value > 190){
        //     motor(RIGHT, 20);
        //     motor(LEFT, -10);
        //      _delay_ms(250);
        // }
        
        if(left_sensor_value > 191 && right_sensor_value > 191 && diff > -2 && diff < 2)
        {
            motor(RIGHT, 20);
            motor(LEFT, 20);
            _delay_ms(500);
        }

        else if(left_sensor_value > right_sensor_value) //left on black, right on white 
        {
            if(mode == RIGHT){ diff_past = 0;}
            diff = (left_sensor_value - right_sensor_value);
            derivative = diff - diff_past;
            if(derivative > 100) {derivative = 0;}
            
            sum = diff + diff_past + diff_past2;
            
            if(sum > 200){
               sum = 0;
            }

            error = (Kp * diff) - (Kd * derivative) + (Ki * sum);
            //increase speed of right , decrease left

            motor(LEFT, (20 - (error/2)));
            motor(RIGHT, (20 + (error/ 2)));
            mode = LEFT;
        }
       
        //else //right on black , left on white -- make this else if?
        else if(left_sensor_value < right_sensor_value)
        {
            if(mode == LEFT){ diff_past = 0;}
            diff = (right_sensor_value - left_sensor_value);
            
            derivative = diff - diff_past;
            
            if(derivative > 100) {derivative = 0;}
            
            sum = diff + diff_past + diff_past2;
            
            if(sum > 200){
                sum = 0;
            }
            
            error = (Kp * diff) - (Kd * derivative) + (Ki * sum);

            //increase speed of right , decrease left

            motor(LEFT, (20 + (error/ 2)));
            motor(RIGHT, (20 - (error/2)) );

            mode = RIGHT;
        }
        // else
        // {
        //     motor(LEFT, (20));
        //     motor(RIGHT, (20));
        // }
        
        
        lcd_cursor(0,0);
        print_snum(error);
       
        lcd_cursor(7-2,0);
        print_num(left_sensor_value);
        lcd_cursor(7-2,1);
        print_num(right_sensor_value); 
        

       
        diff_past = diff; 
        diff_past2 = 0;
    }
    return 0;
}

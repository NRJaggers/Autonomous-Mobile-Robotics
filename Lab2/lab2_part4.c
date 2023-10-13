/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 2 Part 4

Description: Line following bot implemented through PID like control system.
Must handle circles, intersections, and corners. 

Sensor: https://www.pololu.com/product/2458
"Lower output voltage is an indication of greater reflection."

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(){

    init(); //initialize board
    motor_init(); //initialize motors
    
    u16 left_sensor_value, right_sensor_value; //read analog sensor values
    
    //variabls for PID equation
    int16_t error = 0; 
    int16_t diff = 0; 
    int16_t diff_past = 0; 
    int16_t derivative;
    int16_t sum;
    
    //mode is current correction side
    u08 mode = RIGHT;
    
    //PID equation constants
    const u08 Kp = 6;
    const u08 Kd = 1;
    const u08 Ki = 3;

    while(1)
    {

        //read sesnor values
        left_sensor_value = analog(ANALOG4_PIN); 
        right_sensor_value = analog(ANALOG3_PIN); 
        
        //handles intersection case, if both sensors on black then continue forward
        diff = (left_sensor_value - right_sensor_value);
        if(left_sensor_value > 191 && right_sensor_value > 191 && diff > -2 && diff < 2)
        {
            motor(RIGHT, 20);
            motor(LEFT, 20);
            _delay_ms(500);
        }

        //left on black, right on white
        else if(left_sensor_value > right_sensor_value)  
        {
            if(mode == RIGHT){ diff_past = 0;} //reset if changing turing direction
            diff = (left_sensor_value - right_sensor_value); //get sensor difference
            derivative = diff - diff_past; //take derivative of current and past sensor differences
            if(derivative > 100) {derivative = 0;} //reset overloaded derivative term
            
            sum = diff + diff_past; //take integral of past and present
            
            if(sum > 200){sum = 0;} //reset accumulation of integral

            //PID equation
            error = (Kp * diff) - (Kd * derivative) + (Ki * sum);
            
            //increase speed of right , decrease left
            motor(LEFT, (20 - (error/2)));
            motor(RIGHT, (20 + (error/ 2)));
            mode = LEFT;
        }
       
        //right on black , left on white
        else if(left_sensor_value < right_sensor_value)
        {
            //follow same process as other case
            if(mode == LEFT){ diff_past = 0;}
            
            diff = (right_sensor_value - left_sensor_value);
            
            derivative = diff - diff_past;
            
            if(derivative > 100) {derivative = 0;}
            
            sum = diff + diff_past;
            
            if(sum > 200){
                sum = 0;
            }
            
            error = (Kp * diff) - (Kd * derivative) + (Ki * sum);

            //increase speed of right , decrease left
            motor(LEFT, (20 + (error/ 2)));
            motor(RIGHT, (20 - (error/2)) );
            mode = RIGHT;
        }
        
        //Print error and sensor values for debugging
        lcd_cursor(0,0);
        print_snum(error);
       
        lcd_cursor(7-2,0);
        print_num(left_sensor_value);
        
        lcd_cursor(7-2,1);
        print_num(right_sensor_value); 
        
        //Save past sensor difference
        diff_past = diff; 
    
    }

    return 0;
}

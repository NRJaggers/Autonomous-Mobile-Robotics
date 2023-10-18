/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 3 Part 1 

Description: 
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define DELAY 100 //refresh rate; controlled through delay time 

int main(){

    init(); //initialize board
    motor_init(); //initialize motors

    //Variables
    u16 left_sensor_value, right_sensor_value; //read analog sensor values
    u08 state = FALSE;

    struct motor_command speed;

    //Proportional Line Following Program
    while(TRUE)
    {
        if(get_btn())
        {
            state = FALSE;
            _delay_ms(200);
        }

        //read and print sensor values
        left_sensor_value = analog(ANALOG4_PIN); 
        right_sensor_value = analog(ANALOG3_PIN);

        lcd_cursor(7-2,0);
        print_num(left_sensor_value);
        
        lcd_cursor(7-2,1);
        print_num(right_sensor_value); 

        //stop or start the motors
        if (state == FALSE)
        {
            motor_init(); //stop motors
            if(get_btn())
            {
                state = TRUE;
                _delay_ms(200);
            }
        }
        else
        {
            speed = compute_proportional(left_sensor_value, right_sensor_value);

            motor(LEFT, speed.left_motor);
            motor(RIGHT, speed.right_motor);

            _delay_ms(DELAY);
        }

    }
    
}


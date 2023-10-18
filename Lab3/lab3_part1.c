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

struct motor_command 
{
    int8_t left_motor; // left motor speed 
    int8_t right_motor; // right motor speed 
};

struct motor_command compute_proportional(uint8_t left, uint8_t right)
{
    //Variables
    struct motor_command speed;
    int8_t error = 0;
    int8_t correction = 0;
    u08 mode;
    const u08 Kp = 1;

    error = (int8_t)left - (int8_t)right;

    // if (left > right)
    // {
    //     error = left - right;
    //     mode = LEFT;
    // }
    // else if (right > left)
    // {
    //     error = right - left;
    //     mode = LEFT;
    // }
    // else
    // {
    //     error = 0;
    //     mode = FWD; // not left or right
    // }

    correction = Kp*error;

    //clear_screen();
    lcd_cursor(0,0);
    print_snum(error);
    lcd_cursor(0,1);
    print_snum(correction);

    if (error > 40) //if positive (left greater than right)
    //if (mode == LEFT) //if positive (left greater than right)
    {
        //left on black, right on white
        //increase right speed
        speed.left_motor = BASE_SPEED - correction;
        speed.right_motor = BASE_SPEED + correction;
                            
        if (speed.left_motor<0)
            speed.left_motor=0;

    }
    else if (error < -40) //if negative (right greater than left)
    //else if (mode == RIGHT) //if negative (right greater than left)
    {
        //right on black, left on white
        //increase left speed
        speed.left_motor = BASE_SPEED + correction;
        speed.right_motor = BASE_SPEED - correction;

        if (speed.right_motor<0)
            speed.right_motor=0;

    }
    else // equal (aka 0 or less than threshold)
    {
        //both on black (or white), maintain base speed
        speed.left_motor = BASE_SPEED;
        speed.right_motor = BASE_SPEED;
    }

    return speed;
}


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

            _delay_ms(100);
        }

    }
    
}


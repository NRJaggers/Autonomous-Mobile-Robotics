#include "globals.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "functions.h"

// Testing include file
void test_include()
{
    print_string("Hello");
}

// LCD Functions
void print_snum(int16_t num)
{
    // Test  negative bit
    u08 negative = (num & 0x8000) >> 15;

    if(negative){
        
        //print negative sign and twos complement
        print_string("-");
        print_num(((num ^ 0xFFFF) + 1));

    }
    else{
    
    //print number as is (its positive)
    print_num(num);

    }
}

// Motor Functions
void motor_init()
{
    //stop motors
    set_servo(LEFT,SERVO_STOP);
    set_servo(RIGHT,SERVO_STOP);
}

void motor(uint8_t num, int8_t speed){
    
    // speed is percentage, max is +/- 100%
    if(speed < -100){speed = -100;}
    
    if(speed > 100){speed = 100;}

    //num == LEFT or RIGHT motor
    if(num){
        //RIGHT
        set_servo(num,127-0.3*speed);
    }
    
    else{
        //LEFT
        set_servo(num,127+0.3*speed);
    }
 
}

void spin(int8_t speed)
{
    motor(LEFT, speed);
    motor(RIGHT, -speed);
}

void forward(int8_t speed)
{
    motor(LEFT, speed);
    motor(RIGHT, speed);
}

void reverse(int8_t speed)
{
    motor(LEFT, -speed);
    motor(RIGHT, -speed);
}

//Motor Functions with print
void motor_print(uint8_t num, int16_t speed){

    if(speed < -100){speed = -100;}
    
    if(speed > 100){speed = 100;}

    //num == LEFT or RIGHT
    if(num){
        //RIGHT
        set_servo(num,127-0.3*speed);
    }
    
    else{
        //LEFT
        set_servo(num,0.3*speed+127);
    }

    clear_screen();
        
    u08 negative = (speed & 0x8000) >> 15;

    if(negative){

        print_string("-");
        print_num(((speed ^ 0xFFFF) + 1));

    }
    else{
    
    print_num(speed);

    }

}

void spin_print(int8_t speed)
{
    motor_print(LEFT, speed);
    motor_print(RIGHT, -speed);
}

void forward_print(int8_t speed)
{
    motor_print(LEFT, speed);
    motor_print(RIGHT, speed);
}

void reverse_print(int8_t speed)
{
    motor_print(LEFT, -speed);
    motor_print(RIGHT, -speed);
}

// Neural Network functions

struct motor_command compute_proportional(uint8_t left, uint8_t right)
{
    //Variables
    struct motor_command speed;
    int8_t error = 0;
    int8_t correction = 0;
    //u08 mode;
    const float Kp = 0.25;

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

    correction = (Kp*error);

    //clear_screen();
    lcd_cursor(0,0);
    print_snum(error);
    lcd_cursor(0,1);
    print_snum(correction);

    if ((error > ERROR_THRESH) | (error < -ERROR_THRESH)) //if positive (left greater than right)
    //if (mode == LEFT) //if positive (left greater than right)
    {
        //left on black, right on white
        //increase right speed
        speed.left_motor = BASE_SPEED - correction;
        speed.right_motor = BASE_SPEED + correction;
                            
        if (speed.left_motor<0)
            speed.left_motor=0;

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

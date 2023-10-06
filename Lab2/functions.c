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

// Motor Functions
void motor_init()
{
    set_servo(LEFT,SERVO_STOP);
    set_servo(RIGHT,SERVO_STOP);
}

void motor(uint8_t num, int8_t speed){

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
        
    u08 negative = ((u08)speed & 0x80) >> 7;

    if(negative){

        print_string("-");

        //Not sure which is better
        //u16 print_neg = ((speed ^ 0xFFFF) + 1);
        //((speed ^ 1) + 1) & 0x00FF

        print_num(print_neg); // needs work

    }
    else{
    
    print_num(speed);

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

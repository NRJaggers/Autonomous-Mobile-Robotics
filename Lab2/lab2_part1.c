/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 2 Part 1 

Description: Design a motor function and a program with gradual acceleration,
             deceleration. Print the speed on the screen.

*/
#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define ACCEL_FWD   1
#define DCCEL_FWD   2
#define ACCEL_REV   3
#define DCCEL_REV   4

int main(){

    init();
    motor_init();
    
    u08 state = 1;  
    u08 direction = ACCEL_FWD;
    int8_t speed = 0;

    while(1){
    
        if(get_btn() == 1){

            state = state ^ 1;
            _delay_ms(200);

        }

        if(state){

            switch(direction){
                //accelerate forward
                case ACCEL_FWD:
                    if(speed < 100)
                        speed++;
                    else
                        direction = DCCEL_FWD;
                    break;
                //deccelerate forward
                case DCCEL_FWD:
                    if(speed > 0)
                        speed--;
                    else
                        direction = ACCEL_REV;
                    break;
                //accelerate reverse
                case ACCEL_REV:
                    if(speed > -100)
                        speed--;
                    else
                        direction = DCCEL_REV;
                    break;

                //deccelerate reverse
                case DCCEL_REV:
                    if(speed < 0)
                        speed++;
                    else
                        direction = ACCEL_FWD;
                    break;
                
                default:
                    direction = ACCEL_FWD;
                    break;
            }

            forward_print(speed);
            _delay_ms(10);

        }
        
        else
        {    
            motor_init();
        }
    }

    return 0;
}
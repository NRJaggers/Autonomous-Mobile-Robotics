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

int main(){

    init();
    motor_init();
    
    u08 state = 0;  // could maybe combine two flags into 1, where 0 is off and 1-4 is
                    // accelerate forward, decelerate forward, accelerate backward, decelerate backward
    u08 direction = 1;
    u08 speed = 0;

    while(1){
    
        if(get_btn() == 1){

            state = state ^ 1;

            _delay_ms(100);

        }

        if(state){

            // gradually spins the motors to full speed forward
            // gradually slows the motors to a stop
            // does the same in the reverse motor direction and continuously repeats
           
            // use direction flag, go forward and increment. once full speed decrement
            // then switch direction and repeat process. might need another flag like accelerate

            //This switch could prob be reduced down to two states, I am leaving as is right now for
            //development and readability purposes
            switch(direction){
                //accelerate forward
                case 1:
                    if(speed < 100)
                        speed++;
                    else
                        direction = 2;
                    break;
                //deccelerate forward
                case 2:
                    if(speed > 0)
                        speed--;
                    else
                        direction = 3;
                    break;
                //accelerate reverse
                case 3:
                    if(speed > -100)
                        speed--;
                    else
                        direction = 4;
                    break;
                //deccelerate reverse
                case 4:
                    if(speed < 0)
                        speed++;
                    else
                        direction = 1;
                    break;
                
                default:
                    direction = 1;
                    break;
            }

            forward(speed);
            _delay_ms(10);

        }
        
        else
        {    
            motor_init();
        }
    }

    return 0;
}
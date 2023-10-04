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

int main(){

    init();
    
    u08 state = 0;

    while(1){
    
        if(get_btn() == 1){

            state = state ^ 1;

            _delay_ms(100);

        }

        if(state){
           
            set_servo(0,137);

            set_servo(1,117);
        
        }
        
        else{
            
            set_servo(0,127);

            set_servo(1,127);
        
        }
    }

    return 0;
}
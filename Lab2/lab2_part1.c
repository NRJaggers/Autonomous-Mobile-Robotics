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
    
    u08 state = 0;
    u08 direction = 0;

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
            
            motor(0,-50);

            motor(1,-50);

            _delay_ms(1000);


            motor(0,50);

            motor(1,50);

            _delay_ms(1000);

        }
        
        else
        {    
            motor_init();
        }
    }

    return 0;
}


    if(speed < -100){speed = -100;}
    
    if(speed > 100){speed = 100;}

    if(num){
        
        set_servo(num,0.3*speed+127);
          
    }
    
    else{
        
        set_servo(num,127-0.3*speed);

    }

    clear_screen();
        
    u08 negative = speed & 0x80 >> 7;
    
    if(negative){

        print_string("-");

        print_num((speed ^ 1) + 1);

    }
    else{
    
    print_num(speed);

    }

}
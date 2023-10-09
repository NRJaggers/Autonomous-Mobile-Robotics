/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 2 Part 2 

Description: Implement Braitenberg vehicles 2a (fear) and 2b (aggression).
            Press button to toggle between modes. Display mode on LCD.
            Stopped to full speed depending on amount of light recieved.

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

int main(){

    init();
    motor_init();
    
    u08 sensor_value;

    while(1){
    
        sensor_value = analog(0);
        print_num(sensor_value);
    /*
        if(get_btn() == 1){

            state = state ^ 1;

            _delay_ms(100);

        }

        if(state){

            //start here


        }
        
        else
        {    
            motor_init();
        }
    
    */
    }
    return 0;
}
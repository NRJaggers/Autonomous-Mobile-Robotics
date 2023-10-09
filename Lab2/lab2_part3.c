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
    
    u08 left_sensor_value, right_sensor_value;
    u08 left_motor_speed, right_motor_speed;    
    
    u08 mode = ATTRACTION; //0 is FEAR, 1 is AGGRESSION
    clear_screen();
    print_string("ATTRACT");
    
    while(1){

        if(get_btn() == 1){
            
            mode = mode ^ 1;
            
            clear_screen();
            
            if(mode == FEAR){print_string("ATTRACT");}
            else{print_string("FEAR");}
            
            _delay_ms(200);

        }
        
        left_sensor_value = analog(LEFT);
        right_sensor_value = analog(RIGHT);
       
        // convert 0 - 255 to 0 - 100
        left_motor_speed = 100 - ((100 * left_sensor_value) / 255);
        right_motor_speed = 100 - ((100 * right_sensor_value) / 255);
        
        motor(LEFT ^ mode, left_motor_speed);
        motor(RIGHT ^ mode, right_motor_speed);
    
    }
    return 0;
}
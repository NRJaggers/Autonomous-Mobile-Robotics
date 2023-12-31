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

    init(); //initialize board
    
    //save left/right sensor values and speed
    u08 left_sensor_value, right_sensor_value;
    u08 left_motor_speed, right_motor_speed;    
    
    u08 mode = FEAR; //0 is FEAR, 1 is AGGRESSION
    
    //print initialized mode
    clear_screen(); 
    print_string("FEAR");
    
    while(1){

        if(get_btn() == 1){ //change modes on button press
            
            mode = mode ^ 1; //toggle modes with XOR
            
            clear_screen();
            
            //print mode on screen
            if(mode == FEAR){print_string("FEAR");}
            
            else{print_string("AGR0");}
            
            _delay_ms(200); //delay so button press works

        }
        
        //read analog values
        left_sensor_value = analog(LEFT);
        right_sensor_value = analog(RIGHT);
       
        // convert 0 - 255 to 0 - 100 (sensor value to speed)
        left_motor_speed = (100 * left_sensor_value) / 255;
        right_motor_speed = (100 * right_sensor_value) / 255;
        
        //set motor speed, toggle side based on mode
        motor(LEFT ^ mode, left_motor_speed);
        motor(RIGHT ^ mode, right_motor_speed);
    
    }
    return 0;
}
/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Final Project

Description: Knock over cans, stay inside the square.
Six cans in two minutes. Knock the other robot out of
the area as well. 

*/
#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define DIST_SENSOR_UPPER_THRESH 170
#define DIST_SENSOR_LOWER_THRESH 20

int search_for_cans(){

}
int main(){
    int range; 

    typedef enum {READ_SENSORS, MOVEMENT, CORRECTION} state_Robot;

    state_Robot state = READ_SENSORS;

    while(1){
        
        switch(state){

            case READ_SENSORS:
                range = read_range_finder();

                while(1){
                    
                    for(int i = 0; i < 30){
                        turn_right_degrees(1);
                        if(LEFT || RIGHT SENSOR == BLACK)
                            //force a correction
                            //break both loops
                        range = read_range_finder();
                        if(RANGE_LOWER_THRESHOLD < range  && range < RANGE_UPPER_THRESHOLD){
                            state = MOVEMENT;
                            break; // change state
                        }
                    }

                    turn_left_degrees(30);

                    for(int i = 0; i < 30){
                        turn_left_degrees(1);
                        if(LEFT || RIGHT SENSOR == BLACK)
                           //force a correction
                           //break loops

                        range = read_range_finder();
                        if(RANGE_LOWER_THRESHOLD < range  && range < RANGE_UPPER_THRESHOLD){
                            state = MOVEMENT;
                            break; // change state
                        }
                    }

                    //randomly readjust and move
                    turn_right_degrees(random value);
                   
                    move(); // a few units
                    
                    
                }

                break;

            // case MOVEMENT:
            //     while(1){
            //         move_robot();
            //         if(LEFT || RIGHT SENSOR == BLACK){
            //             //stop
            //             motor_init();
            //             reverse(); //move backwards
            //             turn_90();
            //             turn_90();
            //             state = READ_SENSORS;
            //             break;
            //         }
            //     }
                

            //     break;
        }



    }

    return 0;

}
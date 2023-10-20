/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 3 Part 2

Description: 
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define DATA_POINTS 100
int main(){

    init();
    
    typedef enum {STD_MODE, DATA_MODE, TRAIN_MODE, NN_MODE} state_Robot;

    state_Type state = STD_MODE;
    
    float sensor_values[DATA_POINTS][2];
    float motor_values[DATA_POINTS][2];

    float parameters[17]; //hidden layer (2 input + bias)* 3 nodes + (3 input + bias) * 2 nodes
    int epochs = 0;
    int epochs_max = 100;

    int *sensor_point = sensor_values;
    int *sensor_point_start = sensor_values;
    
    int *motor_point = motor_values;
    int *motor_point_start = motor_values;

    while(1){
        switch(state){

            case STD_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Propor-");
                lcd_cursor(0,1);
                print_string("tional");

                if(get_btn()){
                    state = TRAIN_MODE;
                    break;
                }

            case DATA_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Data");

                if(get_btn()){
                    state = TRAIN_MODE;
                    break;
                }

            case TRAIN_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Training");
                
                while(epochs < epochs_max){
                    for(i=0;i<DATA_POINTS;i++){
                        //update parameters
                    }
                    epochs++;
                }

                state = NN_MODE;
                break;
                

            case NN_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Neural");

                if(get_btn()){
                    state = TRAIN_MODE;
                    break;
                }
        }
    }

}




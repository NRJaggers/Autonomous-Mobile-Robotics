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
#include <math.h>

#define DATA_POINTS 100

double sigmoid(double x){
    return (1 / (1 + exp(x)));
}

double d_sigmoid(double x){
    double s = sigmoid(x);
    return s* (1 - s);
}



struct MotorValues { 
    float left;
    float right;  // String
};

struct NeuralData{
    float sensor_values[DATA_POINTS][2];
    float motor_values[DATA_POINTS][2];
    float parameters[17]; //hidden layer (2 input + bias)* 3 nodes + (3 input + bias) * 2 nodes
};



// struct MotorValues compute_neural_network(u08 left_sensor, u08 right_sensor, struct NeuralData d1){
    
//     struct MotorValues m1;
    
//     float h1 = (d1.parameters[0] * (float)left_sensor) + (d1.parameters[1] * (float)right_sensor) + d1.parameters[2];
//     float h2 = (d1.parameters[3] * (float)left_sensor) + (d1.parameters[4] * (float)right_sensor) + d1.parameters[5];
//     float h3 = (d1.parameters[6] * (float)left_sensor) + (d1.parameters[7] * (float)right_sensor) + d1.parameters[8];

//     ml.left = (d1.parameters[9] * h1) + (d1.parameters[10] * h2) + (d1.parameters[11] * h3) + d1.parameters[12];
//     m1.right = (d1.parameters[13] * h1) + (d1.parameters[14] * h2) + (d1.parameters[15] * h3) + d1.parameters[16];

//     return m1;
// }

// train_neural_network(){

// }

// compute_proportional(){

// }


int main(){

    init(); //initialize board
    motor_init(); //initialize motors 
    
    // state machine modes
    typedef enum {STD_MODE, DATA_MODE, TRAIN_MODE, NN_MODE} state_Robot;

    state_Robot state = STD_MODE;
    
    //struct NN_data NN_values;

    int epochs = 0;
    int epochs_max = 100;

    // int *sensor_point = sensor_values;
    // int *sensor_point_start = sensor_values;
    
    // int *motor_point = motor_values;
    // int *motor_point_start = motor_values;

    u16 left_sensor_value, right_sensor_value; //read analog sensor values
    struct motor_command speed;

    while(1){
        //read and print sensor values
        left_sensor_value = analog(ANALOG4_PIN); 
        right_sensor_value = analog(ANALOG3_PIN);

        switch(state){

            case STD_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Propor-");
                lcd_cursor(0,1);
                print_string("tional");

                speed = compute_proportional(left_sensor_value, right_sensor_value);
                motor(LEFT, speed.left_motor);
                motor(RIGHT, speed.right_motor);

                if(get_btn()){
                    state = DATA_MODE;
                    motor_init();
                    _delay_ms(BTN_DELAY);
                }

                break;

            case DATA_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Data");

                if(get_btn()){
                    state = TRAIN_MODE;
                    _delay_ms(BTN_DELAY);
                }
                break;

            case TRAIN_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Training");
                
                while(epochs < epochs_max){
                    for(u08 i=0;i<DATA_POINTS;i++){
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
                    _delay_ms(BTN_DELAY);
                }

                break;
        }
    }

}




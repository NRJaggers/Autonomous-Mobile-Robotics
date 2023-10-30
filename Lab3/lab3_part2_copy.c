/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 3 Part 2

Description: Create a program that steps between the following modes: 
 - Proportional controller for robot
 - Data gather mode, collects sensor values.
 - Choose epochs for training
 - Train robot
 - NN controller for robot
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"
#include <math.h>
#include <stdlib.h> 



int main(){

    init(); //initialize board
    motor_init(); //initialize motors 
    
    // state machine modes
    typedef enum {STD_MODE, DATA_MODE, CHOOSE_EPOCHS, TRAIN_MODE, NN_MODE} state_Robot;

    state_Robot state = STD_MODE;

    u08 left_sensor_value, right_sensor_value; //read analog sensor values
    struct motor_command speed; // proportional controller output values
    struct NeuralData trainingData; // sensor values for NN training
    struct MotorValues m_speed; //  compute NN output values

    int index = 0; // training data index
    int epochs; // epochs count for training
    float x; // accelerometer sensor value
  
    float parameters[PARAMS]; // Weights for computation and training

    while(1){

        switch(state){ // state machine for controlling modes

            case STD_MODE:
                // print proportional mode
                clear_screen();
                lcd_cursor(0,0);
                print_string("Propor-");
                lcd_cursor(0,1);
                print_string("tional");
                _delay_ms(1000);

                //stay in propotion mode
                while(!get_btn()){
                    //read sensor values
                    left_sensor_value = analog(ANALOG4_PIN); 
                    right_sensor_value = analog(ANALOG3_PIN);

                    //print sensor values (for debugging)
                    clear_screen();
                    lcd_cursor(0,0);
                    print_string("Propor-");
                    lcd_cursor(0,1);
                    print_num(left_sensor_value);
                    lcd_cursor(5,1);
                    print_num(right_sensor_value);
                    _delay_ms(15); //stops flickering

                    //compute proporitional correction and set motors
                    speed = compute_proportional(left_sensor_value, right_sensor_value);
                    motor(LEFT, speed.left_motor);
                    motor(RIGHT, speed.right_motor);

                    //change mode to NN data collection if button pressed
                    if(get_btn()){
                        state = DATA_MODE;
                        motor_init(); //stop motors
                        _delay_ms(BTN_DELAY);
                        break;
                    }
                }

                break;

            case DATA_MODE:
                //print data mode
                clear_screen();
                lcd_cursor(0,0);
                print_string("Data ");

                //read sensor values
                left_sensor_value = analog(ANALOG4_PIN); 
                right_sensor_value = analog(ANALOG3_PIN);            

                //collect data until max
                if(index >= DATA_POINTS){
                    //print data index max
                    print_num(DATA_POINTS);
                    
                    //change state to epoch choice if button press
                    if(get_btn()){
                        state = CHOOSE_EPOCHS;
                        _delay_ms(BTN_DELAY);
                    }
                }
                else{
                    // print index and store sensor values
                    print_num(index);
                    trainingData.left_sensor_values[index] = left_sensor_value;
                    trainingData.right_sensor_values[index] = right_sensor_value;

                }

                //print sensor values on screen (for debugging)
                lcd_cursor(0,1);
                print_num(left_sensor_value);
                lcd_cursor(5,1);
                print_num(right_sensor_value);

                //incrment index
                index++;
                _delay_ms(100); //delay for human movement factor
                break;

            case CHOOSE_EPOCHS:

                //read accelerometer value
                x = get_accel_x(); // get x-axis
                
                // convert accelerometer value to epoch count
                if(100 > x && x > 0){epochs = floor(50 *x + 1500)/SCALE;} 
                else if(255 > x && x > 190){epochs = (floor((500 * (x - 190)) / 65) + 1000)/SCALE;}
                
                // print training epoch count
                clear_screen();
                lcd_cursor(0,0);
                print_string("Epochs: ");
                lcd_cursor(0,1);
                print_num(epochs);
                _delay_ms(15); //flickering
                
                //if button press, enter training mode
                if(get_btn()){
                    state = TRAIN_MODE;
                    _delay_ms(BTN_DELAY);
                }
                break;

            case TRAIN_MODE:
                
                // print training message
                clear_screen();
                lcd_cursor(0,0);
                print_string("Training");

                //training function, pass epochs, learning rate, training data, and parameters           
                train_neural_network(epochs, ALPHA, &trainingData, parameters);
                
                //print done when complete
                lcd_cursor(0,1);
                print_string("Done!");
                _delay_ms(BTN_DELAY);

                //enter NN path following mode
                state = NN_MODE;
    
                break;
                
            case NN_MODE:

                //clear screen and set motors to zero
                clear_screen();
                motor_init();
               
                // stay in mode until button press
                while(!get_btn()){
                    
                    //get sensor values and calculate NN motor speeds
                    left_sensor_value = analog(ANALOG4_PIN); 
                    right_sensor_value = analog(ANALOG3_PIN);
                    m_speed = compute_neural_network(left_sensor_value, right_sensor_value, parameters);
                    
                    //print motor speeds (for debugging)
                    clear_screen();
                    lcd_cursor(0,0);
                    print_string("Neural");
                    lcd_cursor(0,1);
                    print_num((u16)100*m_speed.left);
                    lcd_cursor(5,1);
                    print_num((u16)100*m_speed.right);

                    //set motors (0-100)
                    motor(LEFT, (u08)(PERCENT * m_speed.left));
                    motor(RIGHT, (u08)(PERCENT * m_speed.right));
                    _delay_ms(10); //delay for flickering fix
                    
                    //for button press, change mode to retrain network
                    if(get_btn()){
                        motor_init(); //stop motors 
                        index = 0; //reset epoch index
                        state = CHOOSE_EPOCHS; //repick epoch count
                        _delay_ms(BTN_DELAY);
                        break;
                    }
                }
        }
    }
    return 0;
}

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
#include <stdlib.h> 



int main(){

    init(); //initialize board
    motor_init(); //initialize motors 
    
    // state machine modes
    typedef enum {STD_MODE, DATA_MODE, CHOOSE_EPOCHS, TRAIN_MODE, NN_MODE} state_Robot;

    state_Robot state = STD_MODE;

    u08 left_sensor_value, right_sensor_value; //read analog sensor values
    struct motor_command speed;
    struct NeuralData trainingData;
    struct MotorValues m_speed;
    
    int index = 0;
    int epochs; 
 //   u08 data_gathered = 0;
    float x;
  
    float parameters[PARAMS];

    while(1){
        //read and print sensor values
        // left_sensor_value = analog(ANALOG4_PIN); 
        // right_sensor_value = analog(ANALOG3_PIN);

        switch(state){

            case STD_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Propor-");
                lcd_cursor(0,1);
                print_string("tional");

                while(!get_btn()){
                    left_sensor_value = analog(ANALOG4_PIN); 
                    right_sensor_value = analog(ANALOG3_PIN);
                    clear_screen();
                    lcd_cursor(0,0);
                    print_string("Propor-");
                    lcd_cursor(0,1);
                    print_num(left_sensor_value);
                    lcd_cursor(5,1);
                    print_num(right_sensor_value);
                    _delay_ms(15);
                    speed = compute_proportional(left_sensor_value, right_sensor_value);
                    motor(LEFT, speed.left_motor);
                    motor(RIGHT, speed.right_motor);

                    if(get_btn()){
                        state = DATA_MODE;
                        motor_init();
                        _delay_ms(BTN_DELAY);
                        break;
                    }
                }

                break;

            case DATA_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Data ");
              //  (!data_gathered) ? print_num(index) : print_num(DATA_POINTS);
                left_sensor_value = analog(ANALOG4_PIN); 
                right_sensor_value = analog(ANALOG3_PIN);
                
                
                // speed = compute_proportional(left_sensor_value, right_sensor_value);
               
                // if(index >= DATA_POINTS - 1){
                //     index = 0;
                //     data_gathered = 1; 
                // }

                //save proportional data and increment index
               

                // trainingData.left_sensor_values[index] = 255* (rand() / RAND_MAX);
                // trainingData.right_sensor_values[index] = 255* (rand() / RAND_MAX);

                

                // trainingData.left_motor_values[index] = speed.left_motor;
                // trainingData.right_motor_values[index] = speed.right_motor;

                

                if(index >= DATA_POINTS){
                    // for(int i = 0; i < PARAMS; i++){
                    //     parameters[i] = (float)rand() / RAND_MAX;
                    // }
                    //clear_screen();
                    //print_num((u16) (trainingData.parameters[10] * 1000));
                    print_num(DATA_POINTS);
                    
                    if(get_btn()){
                        state = CHOOSE_EPOCHS;
                        _delay_ms(BTN_DELAY);
                    }
                }
                else{
                    print_num(index);
                    trainingData.left_sensor_values[index] = left_sensor_value;
                    trainingData.right_sensor_values[index] = right_sensor_value;

                }
               
                lcd_cursor(0,1);
                print_num(left_sensor_value);

                lcd_cursor(5,1);
                print_num(right_sensor_value);

                index++;

                _delay_ms(100); //10 total seconds of data gathering time
                
                
                break;

            case CHOOSE_EPOCHS:
            
                
                
                x = get_accel_x(); // get x-axis

                if(100 > x && x > 0){epochs = floor(50 *x + 500);} // set top row
                if(100 > x && x > 0){epochs = floor(50 *x + 1500)/SCALE;} // set top row
        
                else if(255 > x && x > 190){epochs = floor((500 * (x - 190)) / 65) + 0;} //set bottom row screen
                else if(255 > x && x > 190){epochs = (floor((500 * (x - 190)) / 65) + 1000)/SCALE;} //set bottom row screen
                
                clear_screen();
                lcd_cursor(0,0);
                print_string("Epochs: ");
                lcd_cursor(0,1);
                print_num(epochs);
                _delay_ms(50); //flickering
                if(get_btn()){
                    state = TRAIN_MODE;
                    _delay_ms(BTN_DELAY);
                }
                
                break;
            case TRAIN_MODE:
              //  motor_init();
                

             //   if(get_btn()){
                    clear_screen();
                    lcd_cursor(0,0);
                    print_string("Training");
                                        
                    train_neural_network(epochs, ALPHA, &trainingData, parameters);
                    
                    lcd_cursor(0,1);
                    print_string("Done!");
                    _delay_ms(BTN_DELAY);

                    state = NN_MODE;
             //   }
        
                break;
                
            case NN_MODE:
                clear_screen();
                motor_init();
               
                while(!get_btn()){
                   
                    left_sensor_value = analog(ANALOG4_PIN); 
                    right_sensor_value = analog(ANALOG3_PIN);
                    m_speed = compute_neural_network(left_sensor_value, right_sensor_value, parameters);
                    
                    clear_screen();
                    lcd_cursor(0,0);
                    print_string("Neural");
                    
                    lcd_cursor(0,1);
                    print_num((u16)100*m_speed.left);
                    lcd_cursor(5,1);
                    print_num((u16)100*m_speed.right);

            
                    motor(LEFT, (u08)(PERCENT * m_speed.left));
                    motor(RIGHT, (u08)(PERCENT * m_speed.right));
                    _delay_ms(10);
                    
                    if(get_btn()){
                        motor_init();

                        index = 0;
                        // for(int i = 0; i < PARAMS; i++){
                        //     parameters[i] = (float)rand() / RAND_MAX;
                        // }

                        state = CHOOSE_EPOCHS;

                        _delay_ms(BTN_DELAY);
                        break;
                    }
                }
                // break;
        }
    }
    return 0;
}

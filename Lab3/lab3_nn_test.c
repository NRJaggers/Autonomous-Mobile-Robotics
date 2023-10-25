/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 3 Part 2

Description: 
*/

//#include "globals.h"
#include "functions.h"
#include <math.h>
#include <stdlib.h> 

// Fake robot functions
#define u08 uint8_t
#define u08 uint8_t

void clear_screen() {};
void lcd_cursor(int,int){};
void print_string(string){};
void print_num(int){};
void _delay_ms(int){};
void get_btn(){};


#define DATA_POINTS 50// more than 50 seems to lead to memeory problems; only 4k for variables
#define PARAMS 17
#define ALPHA 0.05
#define SCALE 10
#define PERCENT 100
#define BIAS_CONST -1

double sigmoid(double x){
    return (1 / (1 + exp(x)));
};

double d_sigmoid(double x){
  //  double s = sigmoid(x);
    return x* (1 - x);
};

struct MotorValues { 
    float left;
    float right;  // String
    float h1; 
    float h2;
    float h3;
};

struct NeuralData{
    float left_sensor_values[DATA_POINTS];
    float right_sensor_values[DATA_POINTS]; //left 1 right 2
    // float left_motor_values[DATA_POINTS];                           //*** THIS HAS POTENTIAL TO BE REPLACED
    // float right_motor_values[DATA_POINTS]; //left 1 right 2
    float parameters[PARAMS]; //hidden layer (2 input + bias)* 3 nodes + (3 input + bias) * 2 nodes
};

struct TempOutputValues{
    float left[DATA_POINTS];
    float right[DATA_POINTS];
};

//why not motor_command? also only should need two input values, can d1 be global? switch motor values to nural data for h1,2,3?
struct MotorValues compute_neural_network(u08 left_sensor, u08 right_sensor, struct NeuralData d1){

    //Tip from Dr.Seng
    //Inside compute_neural_network(), scale the sensor readings to values between 0 and 1

    struct MotorValues m1;

    m1.h1 = sigmoid((d1.parameters[0] * (float)left_sensor) + (d1.parameters[1] * (float)right_sensor) + d1.parameters[2]);
    m1.h2 = sigmoid((d1.parameters[3] * (float)left_sensor) + (d1.parameters[4] * (float)right_sensor) + d1.parameters[5]);
    m1.h3 = sigmoid((d1.parameters[6] * (float)left_sensor) + (d1.parameters[7] * (float)right_sensor) + d1.parameters[8]);

    //sigmoid results in value from 0-1.0, therefore multiply by 100
    m1.left = sigmoid((d1.parameters[9] * m1.h1) + (d1.parameters[10] * m1.h2) + (d1.parameters[11] * m1.h3) + d1.parameters[12]) * PERCENT;
    m1.right = sigmoid((d1.parameters[13] * m1.h1) + (d1.parameters[14] * m1.h2) + (d1.parameters[15] * m1.h3) + d1.parameters[16]) * PERCENT;

    return m1;
}

struct NeuralData train_neural_network(int epochs_max, float alpha,  struct NeuralData nD){
    
    int epochs = 0;
    //struct TempOutputValues tV; 
    float dE[PARAMS];
    struct MotorValues mV;
    struct motor_command target;
    

    while(epochs < epochs_max){
        clear_screen();
        lcd_cursor(0,0);
        print_string("Epoch:");
        lcd_cursor(0,1);
        print_num(epochs);
        
        for(int i = 0 ; i < DATA_POINTS; i++){
            
            mV =  compute_neural_network(nD.left_sensor_values[i], nD.right_sensor_values[i],nD);

            //generate target values
            target = compute_proportional(nD.left_sensor_values[i], nD.right_sensor_values[i]);

        //update output layer
            float outleftTemp = (mV.left - target.left_motor) * d_sigmoid(mV.left);
            //update w10
            dE[9] = outleftTemp * mV.h1;
            //update w11
            dE[10] = outleftTemp * mV.h2;
            //update w12
            dE[11] = outleftTemp * mV.h3;
            //update w13
            dE[12] = outleftTemp * BIAS_CONST; // may work with positive 1 too
            
            float outrightTemp = (mV.right - target.right_motor) * d_sigmoid(mV.right);
            //update w14
            dE[13] = outrightTemp * mV.h1;
            //update w15
            dE[14] = outrightTemp * mV.h2;
            //update w16
            dE[15] = outrightTemp * mV.h3;
            //update w17
            dE[16] = outrightTemp * BIAS_CONST; // may work with positive 1 too
            
        //update hidden layer
            float c1Temp = (mV.left - target.left_motor) * d_sigmoid(mV.left);
            
            float c2Temp = (mV.right - target.left_motor) * d_sigmoid(mV.right);
           
            float h1Temp = d_sigmoid(mV.h1);

            float h2Temp = d_sigmoid(mV.h2);
            
            float h3Temp = d_sigmoid(mV.h3);

            //update w1
            dE[0] = (c1Temp*nD.parameters[10-1] + c2Temp*nD.parameters[14-1]) * h1Temp * nD.left_sensor_values[i];
            //update w2
            dE[1] = (c1Temp*nD.parameters[10-1] + c2Temp*nD.parameters[14-1]) * h1Temp * nD.right_sensor_values[i];
            //update w3
            dE[2] = (c1Temp*nD.parameters[10-1] + c2Temp*nD.parameters[14-1]) * h1Temp * BIAS_CONST;

            //update w4
            dE[3] = (c1Temp*nD.parameters[11-1] + c2Temp*nD.parameters[15-1]) * h2Temp * nD.left_sensor_values[i];
            //update w5
            dE[4] = (c1Temp*nD.parameters[11-1] + c2Temp*nD.parameters[15-1]) * h2Temp * nD.right_sensor_values[i];
            //update w6
            dE[5] = (c1Temp*nD.parameters[11-1] + c2Temp*nD.parameters[15-1]) * h2Temp * BIAS_CONST;

            //update w7
            dE[6] = (c1Temp*nD.parameters[15-1] + c2Temp*nD.parameters[16-1]) * h3Temp * nD.left_sensor_values[i];
            //update w8
            dE[7] = (c1Temp*nD.parameters[15-1] + c2Temp*nD.parameters[16-1]) * h3Temp * nD.right_sensor_values[i];;
            //update w9
            dE[8] = (c1Temp*nD.parameters[15-1] + c2Temp*nD.parameters[16-1]) * h3Temp * BIAS_CONST;

            for(int j = 0 ; j < PARAMS; j++){
                nD.parameters[j] = nD.parameters[j] - (alpha * dE[j]);
            }
        } 
        epochs++;
    }

    return nD;

}


int main(){
    
    u16 left_sensor_value, right_sensor_value; //read analog sensor values
    struct motor_command speed;
    struct NeuralData trainingData;
    
    struct MotorValues m_speed;
    int index = 0;
    int epochs; 
    u08 data_gathered = 0;
  

    while(1){
       

                
                

                    clear_screen();
                    lcd_cursor(0,0);
                    print_string("Training");
                    
                    trainingData = train_neural_network(epochs, 0.001, trainingData);
                    
                    trainingData = train_neural_network(epochs, ALPHA, trainingData);
                    
                    lcd_cursor(0,1);
                    print_string("Done!");
                    _delay_ms(BTN_DELAY);

                    state = NN_MODE;
                    _delay_ms(BTN_DELAY);
                }
        
                break;
                
            case NN_MODE:
                clear_screen();
                lcd_cursor(0,0);
                print_string("Neural");


                m_speed = compute_neural_network(left_sensor_value, right_sensor_value, trainingData);

                motor(LEFT, m_speed.left);
                motor(RIGHT, m_speed.right);

                if(get_btn()){
                    state = TRAIN_MODE;
                    _delay_ms(BTN_DELAY);
                }

                break;
        }
    }

}




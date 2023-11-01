#include "globals.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "functions.h"
#include <math.h>
#include <stdlib.h> 

// Testing include file
void test_include()
{
    print_string("Hello");
}

// LCD Functions
void print_snum(int16_t num)
{
    // Test  negative bit
    u08 negative = (num & 0x8000) >> 15;

    if(negative){
        
        //print negative sign and twos complement
        print_string("-");
        print_num(((num ^ 0xFFFF) + 1));

    }
    else{
    
    //print number as is (its positive)
    print_num(num);

    }
}

// Motor Functions
void motor_init()
{
    //stop motors
    set_servo(LEFT,SERVO_STOP);
    set_servo(RIGHT,SERVO_STOP);
}

void motor(uint8_t num, int8_t speed){
    
    // speed is percentage, max is +/- 100%
    if(speed < -100){speed = -100;}
    
    if(speed > 100){speed = 100;}

    //num == LEFT or RIGHT motor
    if(num){
        //RIGHT
        set_servo(num,127-0.3*speed);
    }
    
    else{
        //LEFT
        set_servo(num,127+0.3*speed);
    }
 
}

void spin(int8_t speed)
{
    motor(LEFT, speed);
    motor(RIGHT, -speed);
}

void forward(int8_t speed)
{
    motor(LEFT, speed);
    motor(RIGHT, speed);
}

void reverse(int8_t speed)
{
    motor(LEFT, -speed);
    motor(RIGHT, -speed);
}

//Motor Functions with print
void motor_print(uint8_t num, int16_t speed){

    if(speed < -100){speed = -100;}
    
    if(speed > 100){speed = 100;}

    //num == LEFT or RIGHT
    if(num){
        //RIGHT
        set_servo(num,127-0.3*speed);
    }
    
    else{
        //LEFT
        set_servo(num,0.3*speed+127);
    }

    clear_screen();
        
    u08 negative = (speed & 0x8000) >> 15;

    if(negative){

        print_string("-");
        print_num(((speed ^ 0xFFFF) + 1));

    }
    else{
    
    print_num(speed);

    }

}

void spin_print(int8_t speed)
{
    motor_print(LEFT, speed);
    motor_print(RIGHT, -speed);
}

void forward_print(int8_t speed)
{
    motor_print(LEFT, speed);
    motor_print(RIGHT, speed);
}

void reverse_print(int8_t speed)
{
    motor_print(LEFT, -speed);
    motor_print(RIGHT, -speed);
}

// Neural Network functions
struct motor_command compute_proportional(uint8_t left, uint8_t right)
{
    //Variables
    struct motor_command speed;
    int8_t error = 0;
    int8_t correction = 0;
    const float Kp = 0.25;

    //calculate error
    error = left - right;
    correction = (Kp*error);

    if ((error > ERROR_THRESH) | (error < -ERROR_THRESH)) //if positive (left greater than right)
    {
        //left on black, right on white
        //increase right speed
        speed.left_motor = BASE_SPEED - correction;
        speed.right_motor = BASE_SPEED + correction;
                            
        if (speed.left_motor<0)
            speed.left_motor=0;

        if (speed.right_motor<0)
            speed.right_motor=0;

    }
    else // equal (aka 0 or less than threshold)
    {
        //both on black (or white), maintain base speed
        speed.left_motor = BASE_SPEED;
        speed.right_motor = BASE_SPEED;
    }

    return speed;
}

float sigmoid(double x){
    return (float)(1 / (1 + exp(-x)));
};

float d_sigmoid(float s){
    //Assuming input coming in is already sigmoided
    return s* (1 - s);
};


struct MotorValues compute_neural_network(float left_sensor, float right_sensor, float *parameters){
    
    //scale the sensor readings to values between 0 and 1
    float left_scaled = ((float)left_sensor) / SENSOR_MAX;
    float right_scaled = ((float)right_sensor) / SENSOR_MAX;
    struct MotorValues m1;

    //compute hidden layer
    m1.h1 = sigmoid((parameters[0] * left_scaled) + (parameters[1] * right_scaled) + (parameters[2] * BIAS_CONST));
    m1.h2 = sigmoid((parameters[3] * left_scaled) + (parameters[4] * right_scaled) + (parameters[5] * BIAS_CONST));
    m1.h3 = sigmoid((parameters[6] * left_scaled) + (parameters[7] * right_scaled) + (parameters[8] * BIAS_CONST));

    //Compute output layer
    //sigmoid results in value from 0-1.0, therefore multiply by 100
    m1.left = sigmoid((parameters[9] * m1.h1) + (parameters[10] * m1.h2) + (parameters[11] * m1.h3) + (parameters[12] * BIAS_CONST));
    m1.right = sigmoid((parameters[13] * m1.h1) + (parameters[14] * m1.h2) + (parameters[15] * m1.h3) + (parameters[16] * BIAS_CONST));

    return m1;
}

void train_neural_network(int epochs_max, float alpha,  struct NeuralData *nD, float *parameters){
    
    //Variables to train network and compute error
    int epochs = 0;
    float dE[PARAMS];
    struct MotorValues mV;
    struct motor_command target;
    

    struct MotorValues mTest;
    struct motor_command tTest;
    
    float error;
    float error_l;
    float error_r;

    //initialize parameters with random values
    for(int i = 0; i < PARAMS; i++){
        parameters[i] = (float)rand() / RAND_MAX;
    }

    while(epochs < epochs_max){
        
        for(int i = 0 ; i < DATA_POINTS; i++){
            //generate network and target values
            mV =  compute_neural_network(nD->left_sensor_values[i], nD->right_sensor_values[i],parameters);
            target = compute_proportional(nD->left_sensor_values[i], nD->right_sensor_values[i]);
           
            
            //update output layer
            float outleftTemp = ((mV.left) - (((float)target.left_motor)/100)) * d_sigmoid(mV.left);

            //update w10
            dE[9] = outleftTemp * mV.h1;
            //update w11
            dE[10] = outleftTemp * mV.h2;
            //update w12
            dE[11] = outleftTemp * mV.h3;
            //update w13
            dE[12] = outleftTemp * BIAS_CONST;
            
            float outrightTemp = ((mV.right) - (((float)target.right_motor)/100)) * d_sigmoid(mV.right);

            //update w14
            dE[13] = outrightTemp * mV.h1;
            //update w15
            dE[14] = outrightTemp * mV.h2;
            //update w16
            dE[15] = outrightTemp * mV.h3;
            //update w17
            dE[16] = outrightTemp * BIAS_CONST;
            
            //update hidden layer
            float c1Temp = ((mV.left) - (((float)target.left_motor)/100)) * d_sigmoid(mV.left);
            
            float c2Temp = ((mV.right) - (((float)target.right_motor)/100)) * d_sigmoid(mV.right);
           
            float h1Temp = d_sigmoid(mV.h1);

            float h2Temp = d_sigmoid(mV.h2);
            
            float h3Temp = d_sigmoid(mV.h3);

            //update w1
            dE[0] = ((c1Temp*parameters[9]) + (c2Temp*parameters[13])) * h1Temp * (nD->left_sensor_values[i]/SENSOR_MAX);
            //update w2
            dE[1] = ((c1Temp*parameters[9]) + (c2Temp*parameters[13])) * h1Temp * (nD->right_sensor_values[i]/SENSOR_MAX);
            //update w3
            dE[2] = ((c1Temp*parameters[9]) + (c2Temp*parameters[13])) * h1Temp * BIAS_CONST;

            //update w4
            dE[3] = ((c1Temp*parameters[10]) + (c2Temp*parameters[13])) * h2Temp * (nD->left_sensor_values[i]/SENSOR_MAX);
            //update w5
            dE[4] = ((c1Temp*parameters[10]) + (c2Temp*parameters[13])) * h2Temp * (nD->right_sensor_values[i]/SENSOR_MAX);
            //update w6
            dE[5] = ((c1Temp*parameters[10]) + (c2Temp*parameters[13])) * h2Temp * BIAS_CONST;

            //update w7
            dE[6] = ((c1Temp*parameters[11]) + (c2Temp*parameters[15])) * h3Temp * (nD->left_sensor_values[i]/SENSOR_MAX);
            //update w8
            dE[7] = ((c1Temp*parameters[11]) + (c2Temp*parameters[15])) * h3Temp * (nD->right_sensor_values[i]/SENSOR_MAX);
            //update w9
            dE[8] = ((c1Temp*parameters[11]) + (c2Temp*parameters[15])) * h3Temp * BIAS_CONST;

            //update all weights
            for(int j = 0 ; j < PARAMS; j++){
                parameters[j] = parameters[j] - (alpha * dE[j]);
            }

        } 
        
        error = 0;
        for(int i = 0; i< DATA_POINTS; i++){
            //generate network and target values
            mTest =  compute_neural_network(nD->left_sensor_values[i], nD->right_sensor_values[i],parameters);
            tTest = compute_proportional(nD->left_sensor_values[i], nD->right_sensor_values[i]);

            error_l = fabs((mTest.left) - (((float)tTest.left_motor)/100)); // network - target
            error_r = fabs((mTest.right) - (((float)tTest.right_motor)/100)); // network - target

            //calculate MSE (really just MS rn)
            error += (error_r + error_l) *(error_r + error_l);
        }
        clear_screen();
        lcd_cursor(0,0);
        print_string("Ep: ");
        lcd_cursor(4,0);
        print_num(epochs);

        //print MSE (error is really tiny so multiplying by more than needed to see error value)
        lcd_cursor(0,1);
        print_num((u16)(error*1000));

        epochs++;
    }

}
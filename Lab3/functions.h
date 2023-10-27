/*
Name: Nathan Jaggers and Weston Keitz   

Description: Include file for functions we develop for bumblebee

Notes and Ideas:

*/

//Global defines
#include<stdint.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// general definitions
#define FALSE  0
#define TRUE 1
#define BTN_DELAY 200 // button delay for 200 ms

// Motor defines
#define LEFT  0 // Left wheel if looking top town at bumblebee with usb port in the back
#define RIGHT 1 // Right wheel if looking top town at bumblebee with usb port in the back
#define FWD 2

// Braitenberg cases
#define FEAR 0 //2A
#define AGGRESSION 1 //2B
#define ATTRACTION 0 //3A
#define SHY 1 //3B

#define SERVO_STOP 127  //stop value for servo
#define SERVO_FSP 97    //full speed positive direction
#define SERVO_FSN 157   //full speed negative direction

// Neural Network Lab componets
#define BASE_SPEED 30 //cruising speed for bot
#define ERROR_THRESH 5 // Threshold for error between sensors before control activates
#define BIAS_CONST 1
#define SCALE 10
#define PERCENT 100
#define SENSOR_MAX 255

#define DATA_POINTS 100  // more than 50 seems to lead to memeory problems; only 4k for variables
#define PARAMS 17       //hidden layer (2 input + bias)* 3 nodes + (3 input + bias) * 2 nodes
#define ALPHA 0.07


// Testing include file
void test_include();

// LCD Functions
void print_snum(int16_t num);

// Motor Functions
void motor_init();
void motor(uint8_t num, int8_t speed);
void spin(int8_t speed);
void forward(int8_t speed);
void reverse(int8_t speed);

// Motor Functions with print
void motor_print(uint8_t num, int16_t speed);
void spin_print(int8_t speed);
void forward_print(int8_t speed);
void reverse_print(int8_t speed);

// Neural Network functions
struct motor_command 
{
    int16_t left_motor; // left motor speed
    int16_t right_motor; // right motor speed
};

struct MotorValues { 
    float left; // make u8 or uint_8? shouldnt need to be float
    float right; // make u8 or uint_8? shouldnt need to be float
    float h1; 
    float h2;
    float h3;
};

struct NeuralData{
    float left_sensor_values[DATA_POINTS]; // make u8 or uint_8? shouldnt need to be float
    float right_sensor_values[DATA_POINTS]; // make u8 or uint_8? shouldnt need to be float
    float parameters[PARAMS]; 
};

struct motor_command compute_proportional(uint8_t left, uint8_t right);
float sigmoid(double x);
float d_sigmoid(float s);
struct MotorValues compute_neural_network(uint8_t left_sensor, uint8_t right_sensor, struct NeuralData d1);
struct NeuralData train_neural_network(uint16_t epochs_max, float alpha,  struct NeuralData nD);

#endif

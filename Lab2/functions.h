/*
Name: Nathan Jaggers and Weston Keitz   

Description: Include file for functions we develop for bumblebee

Notes and Ideas:

*/

//Global defines
#include<stdint.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Motor defines
#define LEFT  0 // Left wheel if looking top town at bumblebee with usb port in the back
#define RIGHT 1 // Right wheel if looking top town at bumblebee with usb port in the back

#define SERVO_STOP 127  //stop value for servo
#define SERVO_FSP 97    //full speed positive direction
#define SERVO_FSN 157   //full speed negative direction

// Testing include file
void test_include();

// Motor Functions
void motor_init();
void motor(uint8_t num, int16_t speed);
void spin(int8_t speed);
void forward(int8_t speed);
void reverse(int8_t speed);




#endif

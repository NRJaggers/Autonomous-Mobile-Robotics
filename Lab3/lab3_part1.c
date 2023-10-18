/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 3 Part 1 

Description: 
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

struct motor_command {
    int8_t left_motor_speed;
    int8_t right_motor_speed;
};

struct motor_command compute_proportional(uint8_t left, uint8_t right);

int main(){
    
}


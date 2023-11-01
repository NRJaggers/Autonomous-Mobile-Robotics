/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 4 Part 2

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
    
    return 0;
}

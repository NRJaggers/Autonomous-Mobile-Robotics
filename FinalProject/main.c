/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Final Project

Description: Knock over cans, stay inside the square.
Six cans in two minutes. Knock the other robot out of
the area as well. 

*/
#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"


int main(){
    init();
    motor_init();

    while(1){
        forward(BASE_SPEED);
    }



    

    return 0;

}
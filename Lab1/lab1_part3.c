/*

Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 1 Part 3

Description: Pong.

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define INPUT 0
#define OUTPUT 1
#define FALSE 0
#define TRUE 1


int main(){
   
    init();
   
    clear_screen(); // clear the screen for bugs

    lcd_cursor(2,0);

    print_string("Pong!");

    //select direction for pins;
    digital_dir(4,OUTPUT);
    digital_dir(5,OUTPUT);
    digital_dir(6,OUTPUT);
    digital_dir(7,OUTPUT);
    digital_dir(8,OUTPUT);
    digital_dir(13,INPUT);

    //select output for pins
    digital_out(4,TRUE);
    digital_out(5,TRUE);
    digital_out(6,TRUE);
    digital_out(7,TRUE);
    digital_out(8,TRUE);

    u08 state = TRUE;
    while(1)
    {
        if(digital(13)==0)
            {
                state = state ^ 1;
                digital_out(4,state);
                digital_out(5,state);
                digital_out(6,state);
                digital_out(7,state);
                digital_out(8,state);
            }
    }


    return 0;

}
/*

Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 1 Part 4

Description: Screen tilt.

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <math.h>

int main(){
   
    init();
   
    clear_screen(); // clear the screen for bugs

    float x, y; // axes 
    
    u08 row = 0; //initialize row
    u08 col = 4; // initialize column

    char class[4] = "416"; // screen message
    
    while(1){
        
        x = get_accel_x(); // get x-axis
        
        y = get_accel_y(); // get y-axis

        if(100 > x && x > 0){row = 0;} // set top row
        
        else if(255 > x && x > 190){row = 1;} //set bottom row screen

        
        if(101 > y && y > 0){col = (u08) floor((4 * (64 - y)) / 70 );} // set top row
        
        else if(255 > y && y > 192){col = (u08) floor(((2*(255-y)) / 70) + 4); } //set bottom row screen

        if(col > 5){col = 5;}


        clear_screen(); //clear old message

        lcd_cursor(col,row); //set cursor for message

        print_string(class); //print message

        _delay_ms(100);
    }

    return 0;

}
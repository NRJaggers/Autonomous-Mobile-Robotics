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

    float x; // x-axis
   
    float y; // y-axis 
    
    u08 row, col; // index for screen

    char class[3] = "416"; // screen message
    
    while(1){
        
        x = get_accel_x(); // get x-axis
        
        y = get_accel_y(); // get y-axis

        if(y > 0){row = 0} // set top row
        
        else{row = 1} //set bottom row screen

        col = (u08) floor(13 * (x + 180) / 360); //set column for screen (start 0 end 13)

        clear_screen(); //clear old message

        lcd_cursor(col, row); //set cursor for message

        print_str(&class); //print message
   
    }

    return 0;

}
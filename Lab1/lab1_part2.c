/*

Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 1 Part 2

Description: Scroll.

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

int main(){

    init(); 

    char name_str[] = "NATHA"; //string
    
    u08 len_name = strlen(name_str); //get string length
    
    clear_screen(); //clear the screen for bugs
    
    u08 x = 0; //check if zero or one index, screen col index
    
    u08 z = 1; 

    while(1){
        
        if(x > 7 - len_name + 1){ //check if string will not fit on end of screen
  
            lcd_cursor(x, 0); //set cursor 
            
            print_string(name_str);  

            lcd_cursor(0, 0); //set cursor  
            
            print_string(name_str+((len_name-z)));

            z++;

            if(z == len_name){
           
                x = 0;
                
                z = 1;
        
            }
        }
        
        else{ //if string fits on string without break

            lcd_cursor(x, 0); //set cursor 

            print_string(name_str); //print whole string

        }

        _delay_ms(500); //delay half a second

        clear_screen(); //clear string

        x++; //increment column
        
        
    }

    return 0;

}
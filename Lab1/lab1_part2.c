/*

Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 1 Part 2

Description: Scroll name horizontally. 
Change name with onboard button press.

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

int main(){

    init(); 

    char name0[] = "Nathan";

    char name1[] = "Weston";

    char *name_str = name0; //string
    
    u08 len_name = strlen(name_str); //get string length
    
    clear_screen(); //clear the screen for bugs
    
    u08 x = 0; //check if zero or one index, screen col index
    
    u08 z = 1; 
    
    u08 choose_name = 0; 

    while(1){

        if(get_btn() == 1){
         
            choose_name = choose_name ^ 1;

            switch(choose_name){
                case 0: 
                    name_str = name0;
                    break;
                case 1:
                    name_str = name1;
                    break;
            }

            u08 len_name = strlen(name_str); //get string length

        }
        
        if(x > 7 - len_name + 1){ //check if string will not fit on end of screen
  
            lcd_cursor(x, 0); //set cursor 
            
            print_string(name_str);  

            lcd_cursor(0, 0); //set cursor  
            
            print_string(name_str+((len_name-z)));

            z++;

            x++; //increment column

            if(z == len_name){
           
                x = 0;
                
                z = 1;
        
            }
        }
        
        else{ //if string fits on string without break

            lcd_cursor(x, 0); //set cursor 

            print_string(name_str); //print whole string
            
            x++; //increment column
        }

        _delay_ms(500); //delay half a second

        clear_screen(); //clear string
        
    }

    return 0;

}

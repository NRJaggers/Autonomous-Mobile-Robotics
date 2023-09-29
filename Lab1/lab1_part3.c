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

    //select direction for pins;
    digital_dir(4,OUTPUT); // right most LED
    digital_dir(5,OUTPUT);
    digital_dir(6,OUTPUT);
    digital_dir(7,OUTPUT);
    digital_dir(8,OUTPUT); // left most LED
    digital_dir(13,INPUT); // input for button

    while(TRUE)
    {
        //set up game
        u16 speed = 100;
        u08 direction = 0; // 0 for right, 1 for left
        u08 current_led = 8; // keep track of lit led

        //set state for starting LED
        digital_out(4,FALSE);
        digital_out(5,FALSE);
        digital_out(6,FALSE);
        digital_out(7,FALSE);
        digital_out(8,TRUE);

        //create start message
        clear_screen();
        lcd_cursor(3,0);
        print_string("3");
        _delay_ms(1000);

        clear_screen();
        lcd_cursor(3,0);
        print_string("2");
        _delay_ms(1000);

        clear_screen();
        lcd_cursor(3,0);
        print_string("1");
        _delay_ms(1000);

        clear_screen();
        lcd_cursor(2,0);
        print_string("Pong!");

        //play game
        while(TRUE)
        {
            for (int i = 0; i < speed; i++)
            {
                _delay_ms(10); //I need to restructure and I think I need to put the check for press in this loop
            }

            //turn off current led, turn on next, and check next direction
            digital_out(current_led,FALSE);
            
            if (direction)
                current_led +=1;
            else
                current_led -=1;

            digital_out(current_led,TRUE);

            if (current_led == 4 || current_led == 8)
            {
                if (digital(13)!=0)
                    break;
                else
                    speed /= 2;
                
                direction = direction ^ 1;
            }
        }
        
        //print end game stats
        clear_screen();
        lcd_cursor(0,0);
        print_string("Speed");
        lcd_cursor(0,1);
        print_num(speed*10);
        print_string("ms");

        //restart program through button press or loops
        while (digital(13)!=0){}; //wait for press
    }

    return 0;
}
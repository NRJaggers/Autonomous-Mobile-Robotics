/*

Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 1 Part 1 

Description: Fade onboard led0 then led1 with no flickering.

Turn off compiler optimizations. Edit makefile: 

lab: $(LAB) $(wildcard $(BOARD_LIB)/*.c)
	avr-gcc -I$(BOARD_LIB) -DF_CPU=$(CLOCK_RATE) -Wall -mmcu=atmega645a -o main.elf $(LAB) $(wildcard $(BOARD_LIB)/*.c)
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int main(){
    
    init();

    u08 LED_curr = 0; //start with led0 on

    u16 loops = 1000; //check that this will not cause overflow, related to the power of lim2
        
    u16 current; //fade time

    u16 i; //delay loop index
    while(1){
  
        current = 0;        

        while(current < loops){ //fade on
            
            led_off(LED_curr); //turn off led
           
            i = 0; //delay, decrease to fade on
            
            while(i<loops-current){i++;}
            
            led_on(LED_curr); //turn on led
            
            i = 0; //delay, increase to fade on
            
            while(i<current){i++;}    
            
            current = current + 1; //increment delay amount

        }

        _delay_ms(100); //keep led on for 100 ms

        current = 0; //reset delay increment
        
        while(current < loops){ //fade off
           
            led_off(LED_curr); //turn led off
            
            i = 0; //delay, increase loop to fade out
            
            while(i<current){i++;}

            
            led_on(LED_curr); //turn led on
            
            i = 0; //delay, decrease loop to fade out
            
            while(i<loops-current){i++;}

            current = current + 1; //increment delay amount

        }

        
        led_off(LED_curr); //keep LED off

        _delay_ms(100);

        LED_curr = LED_curr ^ 1; //change LED



    
    }

    return 0;

}



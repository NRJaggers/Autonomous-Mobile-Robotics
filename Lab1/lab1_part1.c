/*

Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 1 Part 1 

Description: Fade onboard led0 then led1 with no flickering.

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int main(){
    
    init();

    LED_curr = 0; //start with led0 on

    while(1){

        float lim1 = 1; //increases then decreases

        float lim2 = 2^31; //decreases then increases

        u08 loops = 31 //check that this will not cause overflow, related to the power of lim2
       
        //flicker on
        for(u08 i = 0; i < loops; i++){

            led_on(LED_curr);

            for(u16 j = 0; i < lim1; j++){} //delay 1
            
            lim1 = lim1 * 2; //double
            
            led_off(LED_curr);

            for(u16 j = 0; i < lim2; j++){} //delay 2
            
            lim2 = lim2 / 2; //helf

        }

        //flicker off
        for(u08 i = 0; i < loops; i++){

            led_on(LED_curr);

            for(u16 j = 0; i < lim1; j++){} //delay 1
            
            lim1 = lim1 / 2; //divide now, lim1 will start at 2^31
            
            led_off(LED_curr);

            for(u16 j = 0; i < lim2; j++){}
            
            lim2 = lim2 * 2; //multiply, lim2 will start at 2^0

        }

        //change LED
        if(LED_curr == 1){LED_curr = 0;}

        else{LED_curr = 1;}
    
    }

    return 0;

}



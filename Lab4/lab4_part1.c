/*
Name: Nathan Jaggers and Weston Keitz

Assignment Number: Lab 4 Part 1 

Description: 

Notes:  Need to tape encoder disk on wheel
        Figure out how to associate ticks from wheel to degrees traveled around
        the circle. 
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

//function given by Dr.Seng
void init_encoder() {

    // enable encoder interrupts

    EIMSK = 0;

    EIMSK |= _BV(PCIE1) | _BV(PCIE0);


    PCMSK1 |= _BV(PCINT13); //PB5 - digital 5

    PCMSK0 |= _BV(PCINT6);  //PE6 - digital 4


    // enable pullups

    PORTE |= _BV(PE6);

    PORTB |= _BV(PB5);

}


volatile uint16_t left_encoder = 0;

volatile uint16_t right_encoder = 0;


ISR(PCINT0_vect) {

   left_encoder++;  //increment left encoder

}


ISR(PCINT1_vect) {

   right_encoder++;  //increment right encoder

}

int main(){

    init(); //initialize board
    motor_init(); //initialize motors
    init_encoder(); //initialize encoder

    while(TRUE)
    {
        //print encoder values
        lcd_cursor(0,0);
        print_num(left_encoder);
        lcd_cursor(5,0);
        print_num(right_encoder);
    }


}
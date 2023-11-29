#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

volatile uint16_t left_encoder = 0;

volatile uint16_t right_encoder = 0;

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
    
    while(1){
        clear_screen();
        
        // lcd_cursor(0,0);
        // print_string("ENCODER");
        // lcd_cursor(0,1);
        // print_num(left_encoder);

        // u08 left_sensor_value = analog(ANALOG4_PIN); 
        // lcd_cursor(0,0);
        // print_string("L: ");
        // print_num(left_sensor_value);

        // u08 right_sensor_value = analog(ANALOG3_PIN); 
        // lcd_cursor(0,1);
        // print_string("R: ");
        // print_num(right_sensor_value);

        u08 distance_sensor_value = analog(ANALOG2_PIN); 
        lcd_cursor(0,0);
        print_string("Distance");
        lcd_cursor(0,1);
        print_num(distance_sensor_value);
        
        _delay_ms(30);
        // forward(25);


    }
    

    return 0; 

}
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


int main(void) {
    init();  //initialize board hardware
    motor_init();
    init_encoder();
    u16 upper_sensor_value; 

    clear_screen();
    lcd_cursor(0,0);
    print_string("RANGE:");
    lcd_cursor(0,1);
    motor(RIGHT, 30);

   while(1)
   {
        //get range sensor value and refresh
        upper_sensor_value = analog(ANALOG2_PIN); //pick pin 5 or 2 or 1
        lcd_cursor(0,1);
        print_string("   ");
        lcd_cursor(0,1);
        print_num(upper_sensor_value);

        //print ticks
        lcd_cursor(3,1);
        print_num(left_encoder);
        //print_num(right_encoder);

        //_delay_us(400);
   }

   return 0;
}

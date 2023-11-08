/*
Notes:
      ---Data---
      Speed:      Ticks:      Distance (in):
      #           #           #                         
      25          30          5
      25          30          5.25
      25          30          5
      25          30          5
      25          30          5
      25          30          5
      25          30          5.25
      25          30          5.25
      25          30          5
      25          30          5

      25          10          1.625
      25          10          1.75
      25          10          1.625
      25          10          1.625
      25          10          1.5

      25          70          12
      25          70          11.75
      25          70          11.5
      25          70          11.75
      25          70          11.75
      25          70          11.5
      25          70          11.5
      25          70          12
      25          70          11.75
      25          70          11.5
*/


#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"
#include <time.h>

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

void test_sensors()
{
   init_encoder();
   u16 upper_sensor_value; 

   clear_screen();
   lcd_cursor(0,0);
   print_string("RANGE:");
   lcd_cursor(0,1);
   motor(RIGHT, 0);

   while(1)
   {
        //get range sensor value and refresh
        upper_sensor_value = analog(ANALOG2_PIN); //pick pin 5 or 2 or 1
        lcd_cursor(0,1);
        print_string("   ");
        lcd_cursor(0,1);
        print_num(upper_sensor_value);

        //print ticks
        lcd_cursor(4,1);
        print_num(left_encoder);
        //print_num(right_encoder);

        //_delay_us(400);
   }

}

void test_motion_noise(u16 ticks)
{
   left_encoder = 0;

   lcd_cursor(0,0);
   print_string("Ticks:");

   forward(BASE_SPEED);
   while (left_encoder < ticks)
   {
      lcd_cursor(0,1);
      print_num(left_encoder);
   };
   motor_init();
   lcd_cursor(0,1);
   print_num(left_encoder);
}

//direction is LEFT or RIGHT #define (0 or 1)
//if this doesn't work in main program, maybe switch to ticks
void turn_90(u08 direction)
{
   //Assumes you will use same speed as base speed
   if(direction == LEFT)
   {
      spin(BASE_SPEED);
      _delay_ms(LDUR);
      motor_init();
   }

   else if (direction == RIGHT)
   {
      spin(-BASE_SPEED);
      _delay_ms(RDUR);
      motor_init();
   }
   else
   {
      clear_screen();
      lcd_cursor(0,0);
      print_string("Spin Dir");
      lcd_cursor(0,1);
      print_string("Error");
   }

}

int main(void) {
      init();  //initialize board hardware
      motor_init();
      _delay_ms(1000);
      init_encoder();

      //test_sensors();
      test_motion_noise(70);

      //left then right test
      // turn_90(LEFT);
      // _delay_ms(1000);
      // turn_90(RIGHT);

      //right then left test
      // turn_90(RIGHT);
      // _delay_ms(1000);
      // turn_90(LEFT);

   return 0;
}

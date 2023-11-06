/*
Name: Nathan Jaggers and Weston Keitz

Assignment Number: Lab 4 Part 1 

Description: 

Notes:  Need to tape encoder disk on wheel
        Figure out how to associate ticks from wheel to degrees traveled around
        the circle. 

        //write code to run robot around loop
        //count ticks for several runs around loop
        //then calculate ticks/loops 
        //then calculate (ticks/loops)/360 = ~ticks/degree.
        //save value in comments here.

        need proportional controller code to traverse loop

        FSM to control different states?

        measure time it takes to traverse around a portion of the circle (like 
        maybe a quarter of the circle) at a given speed and use that to inform 
        time it will take to make it around the loop several times

        maybe start with it being slower so you can get accurate number of 
        ticks?

        then again capturing behavior of number of ticks at operating speed 
        is prob most important.

        ---Predata--- For quarter circle
        Speed:      Time Count:
        #           #
        

        ---Data---
        Speed:      Time Count:       Ticks:      Loops:
        #           #                 #           #
        
        maybe add more code here to input loops taken and have another state
        that goes around circle and shows ticks, calculated robots current 
        angle (assume starting at 0) and maybe range finder reading  
*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define COUNTMAX 65535 // I hope this is enough, adjust based on experimental runs

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

    // state machine modes
    typedef enum {INIT, PROP, STOP, DEGR, STAT} state_Robot;
    state_Robot state = INIT;

    u08 left_sensor_value, right_sensor_value; //read analog sensor values
    u16 time_count, time_count_max = COUNTMAX;
    u16 degree = 0;
    float loops = 0, ratio = 1;
    struct motor_command speed; // proportional controller output values

    while(TRUE)
    {
        switch(state){ // state machine for controlling modes

            //Initialize and/or reset robot for tick-degree characterization
            case INIT:
                motor_init(); //initialize motors - make sure they are stopped

                //reset encoder values (they are global and incremented by ISR)
                left_encoder = 0;
                right_encoder = 0;

                //print state and encoder values
                clear_screen();
                lcd_cursor(0,0);
                print_string("INIT");
                lcd_cursor(0,1);
                print_num(left_encoder); //Digital input 4
                lcd_cursor(5,1);
                print_num(right_encoder); //Digital input 5

                //change mode to proportional controller line following
                if(get_btn()){
                    state = PROP;
                    _delay_ms(BTN_DELAY);
                }

                break;

            //Start proportional control and collect tick & time data
            case PROP:
                //initialize time count to zero
                time_count = 0;
                while(state == PROP)
                {
                    //read sensor values
                    left_sensor_value = analog(ANALOG4_PIN); 
                    right_sensor_value = analog(ANALOG3_PIN);

                    //print state and encoder values
                    lcd_cursor(0,0);
                    print_string("PROP:");
                    print_num(time_count);
                    lcd_cursor(0,1);
                    print_num(left_encoder); //Digital input 4
                    lcd_cursor(5,1);
                    print_num(right_encoder); //Digital input 5
                    //_delay_ms(15); // stops flickering (don't know that we need this
                                // if we are not clearing screen)

                    //compute proporitional correction and set motors
                    speed = compute_proportional(left_sensor_value, right_sensor_value);
                    motor(LEFT, speed.left_motor);
                    motor(RIGHT, speed.right_motor);

                    //change mode to STOP if button pressed or if time limit reached
                    //(limit should be associated with how long till x loops
                    // around the circle) 
                    if((get_btn())|(time_count >= time_count_max)){
                        state = STOP;
                        motor_init(); //stop motors
                        _delay_ms(BTN_DELAY);
                    }

                    time_count++;
                }
                
                break;

            case STOP: //add functionality to choose next state
                //Stop and hold state to observe recorded values
                lcd_cursor(0,0);
                print_string("STOP:");
                print_num(time_count);
                lcd_cursor(0,1);
                print_num(left_encoder); //Digital input 4
                lcd_cursor(5,1);
                print_num(right_encoder); //Digital input 5

                //change mode to INIT if pressed
                if(get_btn()){
                    state = INIT;
                    _delay_ms(BTN_DELAY);

                    //change to choose next state

                    //then if state is DEGR, choose how many loops completed
                }

                break;

            case DEGR:
                //Wait untill button press (put robot into position)
                while(!get_btn()){/*wait*/};
                _delay_ms(BTN_DELAY);
                degree = 0;
                time_count = 0;
                clear_screen();

                //Start Robot at degree 0 in circle and let it run around
                //Here you can test for accuracy in tick to degree
                while(state == DEGR)
                {
                    //read sensor values
                    left_sensor_value = analog(ANALOG4_PIN); 
                    right_sensor_value = analog(ANALOG3_PIN);

                    //print state and encoder values
                    lcd_cursor(0,0);
                    print_string("DEGR:");
                    print_num(degree);
                    lcd_cursor(0,1);
                    print_num(left_encoder); //Digital input 4
                    lcd_cursor(5,1);
                    print_num(right_encoder); //Digital input 5
                    //_delay_ms(15); // stops flickering (don't know that we need this
                                // if we are not clearing screen)

                    //compute proporitional correction and set motors
                    speed = compute_proportional(left_sensor_value, right_sensor_value);
                    motor(LEFT, speed.left_motor);
                    motor(RIGHT, speed.right_motor);

                    //calculate degrees from ticks
                    degree = (float)left_encoder * ratio;

                    //change mode to STOP if button pressed or if time limit reached
                    //(limit should be associated with how long till x loops
                    // around the circle) 
                    if((get_btn())|(time_count >= time_count_max)){
                        state = STAT;
                        motor_init(); //stop motors
                        _delay_ms(BTN_DELAY);
                    }

                    time_count++;
                }

                break;

                case STAT: //add functionality to choose next state
                //Stop and hold state to observe recorded values
                lcd_cursor(0,0);
                print_string("STAT:");
                print_num(degree);
                lcd_cursor(0,1);
                print_num(left_encoder); //Digital input 4
                lcd_cursor(5,1);
                print_num(right_encoder); //Digital input 5

                //change mode to INIT if pressed
                if(get_btn()){
                    state = INIT;
                    _delay_ms(BTN_DELAY);
                }

                break;

        }

    }

}
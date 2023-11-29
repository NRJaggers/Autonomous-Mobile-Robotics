#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define BLACK_THRESH 150
#define WHITE_THRESH 20
#define DIST_SENSOR_UPPER_THRESH 170
#define DIST_SENSOR_LOWER_THRESH 50

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

    u08 distance_sensor_value, right_sensor_value, left_sensor_value;
    
    typedef enum {READ_SENSORS, MOVEMENT, CORRECTION} state_Robot;
    state_Robot state = READ_SENSORS;
    
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

        // u08 distance_sensor_value = analog(ANALOG2_PIN); 
        // lcd_cursor(0,0);
        // print_string("Distance");
        // lcd_cursor(0,1);
        // print_num(distance_sensor_value);

        switch(state){

            case READ_SENSORS:
                clear_screen();
                lcd_cursor(0,0);
                print_string("READ");
                // for(int i = 0; i < 30){
                while(1){
                    
                    distance_sensor_value = analog(ANALOG2_PIN); 
                    right_sensor_value = analog(ANALOG3_PIN); 
                    left_sensor_value = analog(ANALOG4_PIN); 
                    
                    if(left_sensor_value > BLACK_THRESH || right_sensor_value > BLACK_THRESH){
                        state = CORRECTION;
                        break;
                    }
                        
                    // else{
                    //     turn_90(RIGHT,90); // turn 1 degree
                    //     delay_ms(10);
                    // }
                
                    if(DIST_SENSOR_LOWER_THRESH < distance_sensor_value 
                    && distance_sensor_value < DIST_SENSOR_UPPER_THRESH){
                        state = MOVEMENT;
                        break; // change state
                    }
                
                }
            break;

            case CORRECTION:
                clear_screen();
                lcd_cursor(0,0);
                print_string("correct");
                while(1){
                    right_sensor_value = analog(ANALOG3_PIN); 
                    left_sensor_value = analog(ANALOG4_PIN);
                    if(left_sensor_value < WHITE_THRESH && right_sensor_value < WHITE_THRESH){
                        state = READ_SENSORS;
                        break;
                    }
                }
            break;

            case MOVEMENT:
                clear_screen();

                lcd_cursor(0,0);
                print_string("CANFOUND");
                while(1){

                    distance_sensor_value = analog(ANALOG2_PIN); 
        
                    if(distance_sensor_value < DIST_SENSOR_LOWER_THRESH){
                        state = READ_SENSORS;
                        break;
                    }

                }
            break;
        }
        // turn_90(LEFT,3); //30 degrees
        // turn_90(LEFT,3); //30 degrees

        // for(int i = 0; i < 30){
        //     turn_left_degrees(1);
        //     if(LEFT || RIGHT SENSOR == BLACK)
        //         //force a correction
        //         //break loops

        //     range = read_range_finder();
        //     if(RANGE_LOWER_THRESHOLD < range  && range < RANGE_UPPER_THRESHOLD){
        //         state = MOVEMENT;
        //         break; // change state
        //     }
        // }

        //randomly readjust and move
        // turn_right_degrees(random value);
        
       
       
        _delay_ms(30);
        // forward(25);


    }
    

    return 0; 

}
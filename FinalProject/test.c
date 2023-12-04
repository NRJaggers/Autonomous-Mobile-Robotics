#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define BLACK_THRESH 160
#define WHITE_THRESH 60
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

void encoder_turn_degree(u08 direction, u08 degree){ //must be in increments of 3 degrees
    left_encoder = 0;
    
    if(direction == LEFT){spin(SCAN_SPEED);}
    else if(direction == RIGHT){spin(-SCAN_SPEED);}
   
    while(left_encoder <= (uint16_t)(degree / 3)){
        lcd_cursor(0,0);
        print_string("ENCODER");
        lcd_cursor(0,1);
        print_num(left_encoder);
    };
    motor_init();

}

int main(){

    init(); //initialize board
    motor_init(); //initialize motors
    init_encoder(); //initialize encoder

    u08 distance_sensor_value, right_sensor_value, left_sensor_value;
    
    typedef enum {READ_SENSORS, MOVEMENT, CORRECTION} state_Robot;
    state_Robot state = READ_SENSORS;

    // encoder_turn_degree(RIGHT,180);
    // while(1){
    //     lcd_cursor(0,0);
    //     print_string("ENCODER");
    //     lcd_cursor(0,1);
    //     print_num(left_encoder);}
    
    while(1){

        clear_screen();

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

        // switch(state){

        //     case READ_SENSORS:
        //         clear_screen();
        //         lcd_cursor(0,0);
        //         print_string("READ");
        //         // for(int i = 0; i < 30){
        //         while(1){
                    
        //             distance_sensor_value = analog(ANALOG2_PIN); 
        //             right_sensor_value = analog(ANALOG3_PIN); 
        //             left_sensor_value = analog(ANALOG4_PIN); 
        //             lcd_cursor(0,0);
        //             print_string("L ");
        //             print_num(left_sensor_value);

        //             lcd_cursor(0,1);
        //             print_string("R ");
        //             print_num(right_sensor_value);
                    
        //             if(left_sensor_value > BLACK_THRESH || right_sensor_value > BLACK_THRESH){
        //                 state = CORRECTION;
        //                 break;
        //             }
                
        //             if(DIST_SENSOR_LOWER_THRESH < distance_sensor_value 
        //             && distance_sensor_value < DIST_SENSOR_UPPER_THRESH){
        //                 state = MOVEMENT;
        //                 break; // change state
        //             }

        //             else{
        //                 encoder_turn_degree(RIGHT,3); // turn 1 degree
        //                 _delay_ms(100);
        //                 count++;
        //             }
                
        //         }
        //     break;

        //     case CORRECTION:
        //         clear_screen();
        //         lcd_cursor(0,0);
        //         print_string("correct");
        //         while(1){
        //             right_sensor_value = analog(ANALOG3_PIN); 
        //             left_sensor_value = analog(ANALOG4_PIN);
                    
        //             if(left_sensor_value > WHITE_THRESH){
        //                 encoder_turn_degree(RIGHT,180);
        //             } 
        //             else{
        //                 encoder_turn_degree(LEFT,180);
        //             }

        //             lcd_cursor(0,1);
        //             print_num(left_sensor_value);
                    
        //             lcd_cursor(4,1);
        //             print_num(right_sensor_value);

        //             state = READ_SENSORS;
        //             break;
                    
        //         }
        //     break;

        //     case MOVEMENT:

        //         clear_screen();
        //         lcd_cursor(0,0);
        //         print_string("CANFOUND");

        //         lcd_cursor(0,1);
        //         print_string("D: ");
        //         forward(BASE_SPEED);

        //         while(1){

        //             distance_sensor_value = analog(ANALOG2_PIN); 
        //             right_sensor_value = analog(ANALOG3_PIN); 
        //             left_sensor_value = analog(ANALOG4_PIN); 
                    
        //             lcd_cursor(0,1);
        //             print_num(distance_sensor_value);

        //             if(distance_sensor_value < DIST_SENSOR_LOWER_THRESH){
        //                 state = READ_SENSORS;
        //                 break;
        //             }

        //             if(left_sensor_value > BLACK_THRESH || right_sensor_value > BLACK_THRESH){
        //                 state = CORRECTION;
        //                 break;
        //             }

        //         }
        //     break;
        // }

    }

    return 0; 

}
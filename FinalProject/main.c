/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Final Project

Description: Knock over cans, stay inside the square.
Six cans in two minutes. Knock the other robot out of
the area as well. 

*/
#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"

#define BLACK_THRESH 150
#define WHITE_THRESH 60
#define DIST_SENSOR_UPPER_THRESH 170
#define DIST_SENSOR_LOWER_THRESH 35 //when there is less obstructions and good lighting, ~20-25 is good
                                    //otherwise try like 35

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

void encoder_turn_degree(u08 direction, u08 degree, u08 speed){ //must be in increments of 3 degrees
    left_encoder = 0;
    
    //using slower speed for scanning
    if(direction == LEFT){spin(speed);}
    else if(direction == RIGHT){spin(-speed);}
   
    while(left_encoder <= (uint16_t)(degree / 3)){};
    motor_init();

}

int main(){

    init();             //initialize board
    motor_init();       //initialize motors
    init_encoder();     //initialize encoder

    u08 distance_sensor_value, right_sensor_value, left_sensor_value;
    
    //create and initialize states
    typedef enum {READ_SENSORS, MOVEMENT, CORRECTION} state_Robot;
    state_Robot state = READ_SENSORS;
    
    //Show bot is ready for competition
    clear_screen();
    lcd_cursor(0,0);
    print_string("READY");

    while(!get_btn()){};

    while(1){
        
        //when switching between states, stop the motors
        motor_init();

        switch(state){

            case READ_SENSORS:
                clear_screen();
                lcd_cursor(0,0);
                print_string("READ");
                while(1){

                    //check boundary sensors and make sure you are still in the 
                    //box. If so, scan and if not change to correct state

                    right_sensor_value = analog(ANALOG3_PIN); 
                    left_sensor_value = analog(ANALOG4_PIN); 

                    if(left_sensor_value > BLACK_THRESH || right_sensor_value > BLACK_THRESH){
                        state = CORRECTION;
                        break;
                    }       
                    else{
                        encoder_turn_degree(RIGHT,3,BASE_SPEED); // turn 1 degree
                    }

                    //read current object distance and print value
                    distance_sensor_value = analog(ANALOG2_PIN);

                    lcd_cursor(0,1);
                    print_string("   ");
                    lcd_cursor(0,1);
                    print_num(distance_sensor_value);
                
                    //if current distance is within bounds, pursue object
                    if(DIST_SENSOR_LOWER_THRESH < distance_sensor_value 
                    && distance_sensor_value < DIST_SENSOR_UPPER_THRESH){
                        state = MOVEMENT;
                        //encoder_turn_degree(RIGHT,10);
                        break; // change state
                    }
                
                }
            break;

            case CORRECTION:
                clear_screen();
                lcd_cursor(0,0);
                print_string("CORRECT");
                
                //first back up and try 180 correction
                reverse(FAST_SPEED);
                _delay_ms(100);
                motor_init();

                if(left_sensor_value > WHITE_THRESH){
                    encoder_turn_degree(RIGHT,170,FAST_SPEED);
                } 
                else{
                    encoder_turn_degree(LEFT,170,FAST_SPEED);
                }

                right_sensor_value = analog(ANALOG3_PIN); 
                left_sensor_value = analog(ANALOG4_PIN);


               while(left_sensor_value > BLACK_THRESH || right_sensor_value > BLACK_THRESH){
                    
                    //otherwise try smaller spin correction
                    reverse(FAST_SPEED);
                    _delay_ms(100);
                    motor_init();

                if(left_sensor_value > WHITE_THRESH){
                    encoder_turn_degree(RIGHT,45,FAST_SPEED);
                } 
                else{
                    encoder_turn_degree(LEFT,45,FAST_SPEED);
                }

                    right_sensor_value = analog(ANALOG3_PIN); 
                    left_sensor_value = analog(ANALOG4_PIN);
                    
              }

                //once corrected, go back to reading
                state = READ_SENSORS;

            break;

            case MOVEMENT:

                clear_screen();
                lcd_cursor(0,0);
                print_string("CANFOUND");

                forward(FAST_SPEED);

                while(1){

                    //check boundary sensors and make sure you are still in the 
                    //box. If so, scan and if not change to correct state

                    right_sensor_value = analog(ANALOG3_PIN); 
                    left_sensor_value = analog(ANALOG4_PIN);

                    if(left_sensor_value > BLACK_THRESH || right_sensor_value > BLACK_THRESH){
                        state = CORRECTION;
                        break;
                    }

                    //read current object distance and print value
                    distance_sensor_value = analog(ANALOG2_PIN);
                    
                    lcd_cursor(0,1);
                    print_string("   ");
                    lcd_cursor(0,1);
                    print_num(distance_sensor_value);

                    //if object is lost, return to read sensors state
                    if(distance_sensor_value < DIST_SENSOR_LOWER_THRESH){
                        //opportunity to add some recovery here before going back to read
                        state = READ_SENSORS;
                        break;
                    }

                }
            break;
        }
    }
    return 0; 

}
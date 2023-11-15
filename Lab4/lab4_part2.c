/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 4 Part 2

Description:

Notes:  make function to calculate std_dev of particles? 
        then function for mean which represents location?
        
        make MCL a function?
            - generate n particles
            - take 1 sensor reading
            - for each particle:
                ~ advance the particle (and add noise)
                ~ categorize particle (tower or free space)
                ~ compute weight for particle
            - resample particles

Questions:  Should probablility of tower and free space functions add to 1?
            How are a,b,c,d determined? Experimentally with sensor and hardcode?



*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"
#include <math.h>
#include <stdlib.h> 
#include <string.h>




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


void advance(float degrees){

    left_encoder = 0;

    //variables for proportional controller
    struct motor_command speed;
    u08 left_sensor_value, right_sensor_value; 

   while ((int)(left_encoder*0.8) < degrees)
   {
        //read sensor values
        left_sensor_value = analog(ANALOG4_PIN); 
        right_sensor_value = analog(ANALOG3_PIN);

        //follow line and count ticks
        speed = compute_proportional(left_sensor_value, right_sensor_value); //may need to adjust proportional term so bot turns harder
        motor(LEFT, speed.left_motor);
        motor(RIGHT, speed.right_motor);

        lcd_cursor(0,1);
        clear_screen();
        print_num(left_encoder);
   };

   motor_init();

}

int main(){

    init(); //initialize board
    motor_init(); //initialize motors 

    int ir_value; 

    float sum;
    float random; 
    float random_count;

    int sampling_total = (int) (PARTICLE_COUNT * 0.95);

    //initialize particle positions
    float particles[PARTICLE_COUNT];
    float probabilities[PARTICLE_COUNT];
    float new_particles[PARTICLE_COUNT];
    int classify[PARTICLE_COUNT];

    //initialize particle positions randomly
    for(int i = 0; i < PARTICLE_COUNT; i++){
        particles[i] = 360 * (float) rand() / RAND_MAX;
    }

    struct map towers;

    //create map
    towers.location[0] = 0; 
    towers.location[1] = 90; 
    towers.location[2] = 225; 
    towers.target = 1; 

    //change these values
    struct trapezoid block;

    block.a = 7;
    block.b = 9;
    block.c = 12;
    block.d = 30;

    struct trapezoid free_space;

    free_space.a = 25;
    free_space.b = 40;
    free_space.c = 50;
    free_space.d = 55;

    while(1){

        ir_value = read_range_finder();
        
        sum = 0;
        
        //classify particles as tower or free space, assign probabilites
        for(int i = 0; i < PARTICLE_COUNT; i++){
            
            //classify
            classify_particles(&particles[i], &classify[i], towers);

            //assign probabilities
            if(classify[i] == BLOCK_TOWER){
                prob_given_tower_or_free((float)ir_value, block, &probabilities[i]);
            }

            else{prob_given_tower_or_free((float)ir_value, free_space, &probabilities[i]);}
            
            //get running sum
            sum += probabilities[i];
        }

        //normalize probabilities
        for(int i = 0; i < PARTICLE_COUNT; i++){
            probabilities[i] = probabilities[i] / sum;
        }

        //resample 95% of dataset
        for(int i = 0; i < sampling_total; i++){
            random = (float) rand() / RAND_MAX;
            random_count = 0;
            int j = 0;
           
            while(random_count < random){
                random_count += probabilities[j];
                j++;
            }

            new_particles[i] = particles[j];  
        }
   
        // add noisy particles
        for(int i = sampling_total; i < PARTICLE_COUNT; i++){
            new_particles[i] = (float) (360 * rand() / RAND_MAX); 
        }

        // move robot
        advance(MOTION_DEGREES);

        //set mean to zero
        float mean = 0; 

        // update location , add gaussian noise
        for(int i = sampling_total; i < PARTICLE_COUNT; i++){
            new_particles[i] += new_particles[i] + MOTION_DEGREES +  gaussian_sample(0, MOTION_NOISE_DEV);
            mean += new_particles[i];
        }

        //calculate mean and standard deviation
        mean = mean / PARTICLE_COUNT;
        float variance_sum = 0;
        float temp;
        float std_dev;
        
        //calculate variance
        for(int i = 0; i < PARTICLE_COUNT; i++){
            temp = (mean - new_particles[i]) * (mean - new_particles[i]);
            variance_sum += temp;
        }

        variance_sum = variance_sum / PARTICLE_COUNT;
        std_dev = sqrtf(variance_sum);
        float travel_dist;
        
        //check if particles are grouped together, localization protocol
        if(std_dev < MOTION_NOISE_DEV * 3){
            
            float location = mean; // location will be in degrees

           
            //IF ANGLE OF GOAL TOWER IS GREATER THAN CURRENT POSITION
            if(location <= towers.location[towers.target]){
                travel_dist =  (towers.location[towers.target] - location);            
            }
            
            
            //IF ANGLE OF GOAL TOWER IS GREATER THAN CURRENT POSITION
            else{

                travel_dist = 360 - (towers.location[towers.target] - location);
                
                if(travel_dist > 360){
                    travel_dist = 360 + (towers.location[towers.target] - location);
                }
            }
            
            //move to target
            advance(travel_dist);

            //turn and barrel that shit down
            turn_90(RIGHT);
            forward(100);
            _delay_ms(1000);

            //stop motors
            motor_init();

            //print complete
            print_string("Done!");

            //end program
            return 0;

        }
         
    }
    
    return 0;
}

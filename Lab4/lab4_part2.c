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

#define PI 3.141592654
#define PARTICLE_COUNT 100
#define NUM_TOWERS 3 
#define BLOCK_ANGLE 1.5
#define FREE 0
#define BLOCK_TOWER 1
#define MOTION_NOISE_DEV 0.1
#define MOTION_DEGREES 3

//Box-Muller Transform: function to create sample from gaussian curve
float gaussian_sample(float shift, float scale){
    
    float u1 = (float) rand() / RAND_MAX;
    
    float u2 = (float) rand() / RAND_MAX;

    float z = sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2);

    //add shift = 0 and multiply scale = 1 for normal distribution
    return (z * scale) + shift;

}

//complete this function
int read_ir(void){
    return sensor;
}

struct map{
    float location[NUM_TOWERS];
    int target;
}

struct trapezoid{
    float a;
    float b;
    float c;
    float d;
}

void prob_given_tower_or_free(float sensor, struct trapezoid type, float *probability){
   
    float u = 2 /(type.c + type.d - type.a - type.b);
    
    if(type.a <= x && x < type.b){probability = u * (sensor - type.a) / (type.b - type.a);}
    
    else if(type.b <= x && x < type.c){probability = u;}

    else if(type.c <= x && x < type.d){probability = u * (type.d - sensor) / (type.c - type.d);}

    else{probability = 0;}

}

void classify_particles(float *particle, float *classify, struct tower towers){
    
    for(int j = 0; j < NUM_TOWERS; j++){

        if((towers.location[j] - BLOCK_ANGLE < particle)
        || (towers.location[j] + BLOCK_ANGLE > particle)){
            
            if(towers.location[j] - BLOCK_ANGLE < 0){
                if(towers.location[j] - BLOCK_ANGLE + 360 < particle){
                    classify = BLOCK_TOWER;
                }
            }

            else if(towers.location[j] - BLOCK_ANGLE > 360){
                if(towers.location[j] - BLOCK_ANGLE - 360 > particle){
                    classify = BLOCK_TOWER;
                }
            }

            else{
                classify = BLOCK_TOWER;
            }

        }

        else{
            classify = FREE;
        }
            
    }
}

void advance(float degrees){
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

    for(int i = 0; i < PARTICLE_COUNT; i++){
        particles[i] = 360 * (float) rand() / RAND_MAX;
    }

    struct tower towers;

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

    float degrees;

    while(1){

        ir_value = read_ir();
        
        sum = 0;
        
        for(int i = 0; i < PARTICLE_COUNT; i++){
            
            //classify
            classify_particles(&particle[i], &classify[i], towers);

            //assign probabilities
            if(classify[i] == BLOCK_TOWER){
                prob_given_tower_or_free(ir_value, block, &probabilities[i]);
            }

            else{prob_given_tower_or_free(ir_value, free_space, &probabilities[i]);}
            
            //get running sum
            sum += probabilities[i];
        }

        //normalize probabilities
        for(int i = 0; i < PARTICLE_COUNT; i++){
            probabilities[i] = probabilities[i] / sum;
        }

        //resample
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

       

        for(int i = sampling_total; i < PARTICLE_COUNT; i++){
            new_particles[i] = (float) (360 * rand() / RAND_MAX); 
        }

        // move robot
        advance(MOTION_DEGREES);

        float mean = 0; 

        // update location , add gaussian noise
        for(int i = sampling_total; i < PARTICLE_COUNT; i++){
            new_particles[i] += new_particles[i] + MOTION_DEGREES +  gaussian_sample(0, MOTION_NOISE_DEV);
            mean += new_particles[i];
        }

        //calculate mean and standard deviation
        mean = mean / PARTICLE_COUNT;
        float sum2 = 0;
        float temp;
        float std_dev;
        for(int i = 0; i < PARTICLE_COUNT; i++){
            temp = (mean - new_particles[i]) * (mean - new_particles[i]);
            sum2 += temp;

        }

        sum2 = sum2 / PARTICLE_COUNT;

        std_dev = sqrtf(sum2);
        float travel_dist;
        if(std_dev < MOTION_NOISE_DEV * 3){
            float location = mean; // location will be in degrees

            // need a case for crossing the 360
           
            if(location > towers.location[towers.target]){
                travel_dist = location - towers.location[towers.target];
            }
            if(location < towers.location[towers.target]){
                // need a case for crossing the 360
                if()
                travel_dist = 360 - (location - towers.location[towers.target]);
            }
            while(1){
                advance()
            }
        }




        

        
    }
    
    return 0;
}

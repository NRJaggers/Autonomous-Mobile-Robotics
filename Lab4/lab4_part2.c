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

//Box-Muller Transform: function to create sample from gaussian curve
float gaussian_sample(float shift, float scale){
    
    float u1 = (float) rand() / RAND_MAX;
    
    float u2 = (float) rand() / RAND_MAX;

    float z = sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2);

    //add shift = 0 and multiply scale = 1 for normal distribution
    return (z * scale) + shift;

}

// based on sensor reading distance
float prob_tower_given_distance(float sensor, float a, float b, float c, float d){
    
    float u = 2 /(c + d - a - b);
    
    if(a <= x && x < b){return u * (sensor - a) / (b - a);}
    
    else if(b <= sensor_val && sensor_val < c){return u;}

    else if(c <= x && x < d){return u * (d - sensor) / (c - d);}

    else{return 0;}

}

float prob_tower_given_angle(float particle, float a, float b, float c, float d){
    
    float u = 2 /(c + d - a - b);
    
    if(a <= x && x < b){return u * (sensor - a) / (b - a);}
    
    else if(b <= x && x < c){return u;}

    else if(c <= x && x < d){return u * (d - sensor) / (c - d);}

    else{return 0;}

}

// float prob_free_space_given_location(float sensor, float a, float b, float c, float d){
    
//     return 1 - prob_tower_given_location(sensor, a, b, c, d);
    
// }

//complete this function
u08 read_ir(void){
    return sensor;
}

struct tower{
    float location[NUM_TOWERS];
    u08 target;
}

//fix
// void prob_sensor_value_given_location(float sensor, struct tower tower_inst, float *particles, float *probabilities){
    
//     float sum = 0; 
   
//     for(int j = 0; j < PARTICLE_COUNT, j++){
       
//         for(int i = 0; i < NUM_TOWERS; i++){
            
//             float temp = tower_inst.location[i];
            
//             probabilities[j] += prob_tower_given_location(sensor, temp - 2, temp - 1, temp + 1, temp + 2);
    
//         }
//     }
// }

void prob_sensor_value_given_tower(float sensor, struct tower tower_inst, float *particles, float *probabilities){
   
    float u = 2 /(c + d - a - b);
    
    if(a <= x && x < b){return u * (sensor - a) / (b - a);}
    
    else if(b <= x && x < c){return u;}

    else if(c <= x && x < d){return u * (d - sensor) / (c - d);}

    else{return 0;}

}

void prob_sensor_value_given_free_space(float sensor, struct tower tower_inst, float *particles, float *probabilities){
    
    return 1 - prob_sensor_value_given_tower(sensor,tower_inst, &particles, &probabilities);
    
}

int main(){

    init(); //initialize board
    motor_init(); //initialize motors 

    u08 ir_value; 
    
    //initialize particle positions
    float particles[PARTICLE_COUNT];
    float probabilities[PARTICLE_COUNT];

    for(int i = 0; i < PARTICLE_COUNT; i++){
        particles[i] = 360 * (float) rand() / RAND_MAX;
    }

    struct tower towers;

    //create map
    towers.location[0] = 0; 
    towers.location[1] = 90; 
    towers.location[2] = 225; 
    towers.target = 1; 

    float dist1, dist2, dist3, dist4;

    while(1){

        ir_value = read_ir();

        for(int i = 0; i < PARTICLE_COUNT; i++){
            prob_tower_given_location(particle[i], float a, float b, float c, float d)
        }
        

        
    }
    
    return 0;
}

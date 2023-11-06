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


//complete this function
u08 read_ir(void){
    return sensor;
}

struct map{
    float location[NUM_TOWERS];
    u08 target;
}

struct trapezoid{
    float a;
    float b;
    float c;
    float d;
}


void prob_given_tower_or_free(float sensor, struct trapezoid type){
   
    float u = 2 /(type.c + type.d - type.a - type.b);
    
    if(type.a <= x && x < type.b){return u * (sensor - type.a) / (type.b - type.a);}
    
    else if(type.b <= x && x < type.c){return u;}

    else if(type.c <= x && x < type.d){return u * (type.d - sensor) / (type.c - type.d);}

    else{return 0;}

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

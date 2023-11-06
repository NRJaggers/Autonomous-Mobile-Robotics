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

#define PI 3.141592654


//Box-Muller Transform: function to create sample from gaussian curve
float gaussian_sample(float shift, float scale){
    
    float u1 = (float) rand() / RAND_MAX;
    
    float u2 = (float) rand() / RAND_MAX;

    float z = sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2);

    //add shift = 0 and multiply scale = 1 for normal distribution
    return (z * scale) + shift;

}

//function for calculating probablilty of getting sensor_val given that the 
//robot/particle position is at a tower 
float prob_tower_given_sensor(float sensor_val, float a, float b, float c, float d){
    
    float u = 2 /(c + d - a - b);
    
    if(a <= sensor_val && sensor_val < b){return u * (sensor_val - a) / (b - a);}
    
    else if(b <= sensor_val && sensor_val < c){return u;}

    else if(c <= sensor_val && sensor_val < d){return u * (d - sensor_val) / (c - d);}

    else{return 0;}

}

//function for calculating probablilty of getting sensor_val given that the 
//robot/particle position is at a tower
float prob_free_space_given sensor(float sensor_val, float a, float b, float c, float d){
    
    return 1 - prob_tower_given_sensor(sensor_val, a, b, c, d); //yeah I see what you mean by your question now
    //we should ask him in class if they add up to 1 or if we are supposed to have independent
    //functions and curves.
    
}

int main(){

    init(); //initialize board
    motor_init(); //initialize motors 

    while(1){
        
    }
    
    return 0;
}

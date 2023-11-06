/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 4 Part 2

Description:

*/

#include "globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "functions.h"
#include <math.h>
#include <stdlib.h> 

#define PI 3.141592654
#define PARTICLE_COUNT 100
#define NUM_TOWERS 3 

//add = 0 and scale = 1 for normal distribution
float gaussian_sample(float shift, float scale){
    
    float u1 = (float) rand() / RAND_MAX;
    
    float u2 = (float) rand() / RAND_MAX;

    float z = sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2);

    return (z * scale) + shift;

}


float prob_tower_given_location(float sensor_val, float a, float b, float c, float d){
    
    float u = 2 /(c + d - a - b);
    
    if(a <= x && x < b){return u * (sensor_val - a) / (b - a);}
    
    else if(b <= x && x < c){return u;}

    else if(c <= x && x < d){return u * (d - sensor_val) / (c - d);}

    else{return 0;}

}

float prob_free_space_given_location(float sensor_val, float a, float b, float c, float d){
    
    return 1 - prob_tower_given_location(sensor_val, a, b, c, d);
    
}

//complete this function
u08 read_ir(void){
    return sensor;
}

struct towers{
    float location[NUM_TOWERS];
    u08 target;
}

float prob_tower_given_location(float angle0, float angle1, float angle2){

    

}

int main(){

    init(); //initialize board
    motor_init(); //initialize motors 

    u08 ir_value; 
    //initialize particle position
    float particles[PARTICLE_COUNT];

    for(int i = 0; i < PARTICLE_COUNT; i++){
        particles[i] = 360 * (float) rand() / RAND_MAX;
    }

    //create map
    towers.location[0] = 0; 
    towers.location[0] = 90; 
    towers.location[0] = 225; 
    towers.target = 1; 

    while(1){

        ir_value = read_ir();

        for(int i = 0; i < PARTICLE_COUNT; i++){
            prob_tower_given_location(particle[i], float a, float b, float c, float d)
        }
        

        
    }
    
    return 0;
}

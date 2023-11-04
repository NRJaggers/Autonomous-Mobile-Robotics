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


//add = 0 and scale = 1 for normal distribution
float gaussian_sample(float shift, float scale){
    
    float u1 = (float) rand() / RAND_MAX;
    
    float u2 = (float) rand() / RAND_MAX;

    float z = sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2);

    return (z * scale) + shift;

}

float prob_tower_given_sensor(float sensor_val, float a, float b, float c, float d){
    
    float u = 2 /(c + d - a - b);
    
    if(a <= x && x < b){return u * (sensor_val - a) / (b - a);}
    
    else if(b <= x && x < c){return u;}

    else if(c <= x && x < d){return u * (d - sensor_val) / (c - d);}

    else{return 0;}

}

float prob_free_space_given sensor(float sensor_val, float a, float b, float c, float d){
    
    return 1 - prob_tower_given_sensor(sensor_val, a, b, c, d);
    
}

int main(){

    init(); //initialize board
    motor_init(); //initialize motors 

    while(1){
        
    }
    
    return 0;
}

/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 4 Part 2 Simulation

Description: Test for Monty Carlo Localization algorithm.

Notes:  make function to calculate std_dev of particles? 
        then function for mean which represents location?
        
        make MCL a function?
            - generate n particles
            - advance robot
            - take 1 sensor reading
            - for each particle:
                ~ advance the particle (and add noise)
                ~ categorize particle (tower or free space)
                ~ compute weight for particle
            - resample particles

        should I make a struct for particles? (location, weight, space type?)

        maybe randomly generate tower locations? can't have them too close 
        though will need some extra conditions.

Questions:  Should probablility of tower and free space functions add to 1?
            How are a,b,c,d determined? Experimentally with sensor and hardcode?

*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.141592654
#define PARTICLE_COUNT 10
#define NUM_TOWERS 3 

typedef struct particle {
    __uint16_t location;
    float weight;
} particle;

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
float prob_tower_given_location(float sensor_val, float a, float b, float c, float d){
    
    float u = 2 /(c + d - a - b);
    
    if(a <= sensor_val && sensor_val < b){return u * (sensor_val - a) / (b - a);}
    
    else if(b <= sensor_val && sensor_val < c){return u;}

    else if(c <= sensor_val && sensor_val < d){return u * (d - sensor_val) / (c - d);}

    else{return 0;}

}

//function for calculating probablilty of getting sensor_val given that the 
//robot/particle position is in free space
float prob_free_space_given_location(float sensor_val, float a, float b, float c, float d){
    
    return 1 - prob_tower_given_location(sensor_val, a, b, c, d); //yeah I see what you mean by your question now
    //we should ask him in class if they add up to 1 or if we are supposed to have independent
    //functions and curves.
    
}

__uint16_t advance(__uint16_t location, __uint16_t degrees)
{
    //using noise in motion model
    __int8_t noise = gaussian_sample(0,1);
    __uint8_t new_location = (location + degrees + noise) % 360;
    return new_location; 
}

void init_part_array(particle array[], __uint16_t count)
{
    for(int i = 0; i < count; i++)
    {
        array[i].location = (rand() / RAND_MAX)*359;
        array[i].weight = 0;
    }
}
__uint8_t within_n(__uint16_t n, __uint16_t robot, __uint16_t tower)
{
    __uint8_t is_within = 0;

    //check if robot is within n degrees of tower (or any location)
    __int16_t tower_upper_bound = tower + n;
    if (tower_upper_bound > 359){tower_upper_bound -= 360;}
    __int16_t tower_lower_bound = tower - n;
    if (tower_lower_bound < 0){tower_upper_bound += 360;}

    //most cases
    if((robot<tower_upper_bound) && (robot>tower_lower_bound))
        is_within = 1;

    //too specific to zero need to adjust
    if((robot < n) || (robot > 359 - n))
        if (tower == 0)
            is_within = 1;

}

__uint8_t simulate_sensor(__uint16_t location, __uint16_t towers[], __uint8_t num_towers)
{
    __uint8_t sensor_val = 0;

    for (int i = 0; i < num_towers; i++)
    {
        if (within_n(5, location, towers[i]))
        {
            sensor_val = gaussian_sample(100,100);
        }
    }

    if(sensor_val == 0)
    {
        sensor_val = gaussian_sample(50,50);
    }
}

int main(){

    //initialization parameters
    //these variables describe the map of the circle and blocks
    __uint16_t tower_locations[NUM_TOWERS] = {0, 45, 180}; 
    __uint8_t  target_tower = 2;

    __uint16_t robot_location = (rand() / RAND_MAX)*359;

    particle particle_array[PARTICLE_COUNT];

    __uint8_t  sensor = 0;
    
    init_part_array(particle_array, PARTICLE_COUNT);

    //advance robot and take sensor reading
    robot_location = advance(robot_location, 5);
    sensor = simulate_sensor(robot_location, tower_locations, NUM_TOWERS);


    // - for each particle:
    //     ~ advance the particle (and add noise)
    //     ~ categorize particle (tower or free space)
    //     ~ compute weight for particle
    // - resample particles



    

    return 0;
}
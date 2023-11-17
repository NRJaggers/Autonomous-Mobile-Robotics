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

        if you make visualization, you will need to output results to a file
        because you are giving interface to consol. 

Questions:

*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <new>

//make porting to bot easizer
typedef __u_int u08;
typedef __uint16_t u16;

//define known parameters and constants
#define PI 3.141592654
#define PARTICLE_COUNT 10
#define BLOCK_ANGLE 2

//define abstract data types 
struct map{
    u16 r_location;
    u08 num_towers;
    u08 target;
    u16 *t_locations;
};

struct trapezoid{
    float a;
    float b;
    float c;
    float d;
};

typedef struct particle {
    u16 location;
    float weight;
} particle;

//define useful functinons
//Box-Muller Transform: function to create sample from gaussian curve
float gaussian_sample(float shift, float scale){
    
    //make sure u1 is not zero
    float u1 = (float) rand() / RAND_MAX;
    while (u1 == 0)
        u1 = (float) rand() / RAND_MAX;
    
    float u2 = (float) rand() / RAND_MAX;

    float z = sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2);

    //add shift = 0 and multiply scale = 1 for normal distribution
    return (z * scale) + shift;

}

//function for calculating probablilty of getting sensor_val given that the 
//robot/particle position is at a tower or freespace 
void prob_given_tower_or_free(float sensor, struct trapezoid type, float *probability){
   
    //max probability, top of trapizoid
    float u = 2 /(type.c + type.d - type.a - type.b);
    
    //determine probability of sensor reading based on where it falls on trapizoid
    if(type.a <= sensor && sensor < type.b){*probability = u * (sensor - type.a) / (type.b - type.a);}
    
    else if(type.b <= sensor && sensor < type.c){*probability = u;}

    else if(type.c <= sensor && sensor < type.d){
        *probability = u * (type.d - sensor) / (type.d - type.c);
        }

    else{*probability = 0;}
}

// void classify_particles(float* particle, int* classify, struct map towers){
    
//     for(int j = 0; j < NUM_TOWERS; j++){
        
//         if((fabs(towers.location[j] - *particle)) <= BLOCK_ANGLE){
//             *classify = BLOCK_TOWER;
//             break;
//         }

//         if(towers.location[j] < *particle){
//             if((towers.location[j] + (360 - *particle)) <= 1.5){
//                 *classify = BLOCK_TOWER;
//                 break;
//             }
//         }

//         if(towers.location[j] > *particle){
//             if((*particle + (360 - towers.location[j])) <= (float)BLOCK_ANGLE){
//                 *classify = BLOCK_TOWER;
//                 break;
//             }
//         }
        
//         else{*classify = FREE;}
    
//     }
// }

float min(float a, float b){
    if (a < b) 
        return a ;
    else
        return b;
}

float calc_mean(float *particle, float size){
    float totalx = 0;
    float totaly = 0;
    
    for(int i=0; i<size; i++){
        totalx += cosf(*particle * PI / 180);
        totaly += sinf(*particle * PI / 180);
        particle++;
    }

    totalx = totalx / size;
    totaly = totaly / size;

    float angle = atanf(totaly/totalx) * 180 / PI;
    
    if(totalx < 0 && totaly >=0)
        return angle + 180;
    else if(totalx < 0 && totaly < 0)
        return angle + 180;
    else if(totalx >= 0 && totaly < 0)
        return angle + 360;
    else // if in pos x and pos y quadrant
        return angle;
}

u16 advance(u16 location, u16 degrees)
{
    //using noise in motion model
    __int8_t noise = gaussian_sample(0,1);
    u08 new_location = (location + degrees + noise) % 360;
    return new_location; 
}

void init_part_array(particle array[], u16 count)
{
    for(int i = 0; i < count; i++)
    {
        array[i].location = (rand() / RAND_MAX)*359;
        array[i].weight = 0;
    }
}

float angular_dist(float angle1, float angle2)
{
    float adist;

    // Normalize angles to the range [0, 360)
    // Normalize angles to the range [0, 360)
    angle1 = fmod(angle1, 360.0);
    angle2 = fmod(angle2, 360.0);
        
    // Calculate the absolute difference between the angles
    float diff = fabs(angle1 - angle2);

    // Consider both the direct difference and the wrapped-around difference
    float wrappedDiff = 360 - diff;

    // Check if either difference is less than or equal to n
    if (diff < wrappedDiff)
        adist = diff;
    else
        adist = wrappedDiff;

    return adist;

}

u08 within_n(u16 n, u16 angle1, u16 angle2)
{
    u08 is_within = 0;

    // Normalize angles to the range [0, 360)
    angle1 = angle1 % 360;
    angle2 = angle2 % 360;
        
    // Calculate the absolute difference between the angles
    u16 diff = abs(angle1 - angle2);

    // Consider both the direct difference and the wrapped-around difference
    u16 wrappedDiff = 360 - diff;

    // Check if either difference is less than or equal to n
    if ((diff <= n) || (wrappedDiff <= n))
        is_within = 1;

    return is_within;

}

u08 f_within_n(float n, float angle1, float angle2)
{
    u08 is_within = 0;

    // Normalize angles to the range [0, 360)
    angle1 = fmod(angle1, 360.0);
    angle2 = fmod(angle2, 360.0);
        
    // Calculate the absolute difference between the angles
    float diff = fabs(angle1 - angle2);

    // Consider both the direct difference and the wrapped-around difference
    float wrappedDiff = 360.0 - diff;

    // Check if either difference is less than or equal to n
    if ((diff <= n) || (wrappedDiff <= n))
        is_within = 1;

    return is_within;
}

u08 simulate_sensor(struct map map)
{
    //***Note***
    //May want to play around with numbers and distribution of returned sensor readings

    u08 sensor_val = 0;
    float dist;

    //check robot location against tower location 
    for (int i = 0; i < map.num_towers; i++)
    {
        dist = angular_dist(map.r_location, map.t_locations[i]);
        
        //is location on a tower?
        if(dist < BLOCK_ANGLE ){
            
            sensor_val = (85 + gaussian_sample(0,5));
        }
        //is location near a tower
        else if ((dist >= BLOCK_ANGLE) && (dist <= BLOCK_ANGLE + 1)){
            sensor_val = (45 + gaussian_sample(0,5));
        }
    }

    //is location near freespace?
    if (sensor_val == 0){
        sensor_val = (5 + gaussian_sample(0,4));
    }

    return sensor_val;
}

struct map init_map()
{
    struct map map;

    /*
    ******fix this section here!
    change to c instead of c++. use chat gpt
    want to switch to scanf
    maybe or maybe not malloc
    */

    //get number of towers
    printf("Select number of towers (2-4)\nnum_towers:");
    std::cin >> map.num_towers;
    //scanf("%u", &map.num_towers);
    //printf("num_towers:%d",map.num_towers);
    
    //define new tower array and collect tower locations
    map.t_locations = new u16[map.num_towers];

    for(u08 i = 0; i < map.num_towers; i++)
    {
        printf("Select location of tower %d (0-359)\nt%d:",i+1,i+1);
        std::cin >> map.t_locations[i];
    }

    //pick target
    printf("Select the target tower (1-%d)\ntarget:",map.num_towers);
    std::cin >> map.target;

    //pick starting location for robot
    printf("Select robot location (0-359))\nrobot:");
    std::cin >> map.r_location;
    
    return map; 
}

int main(){

    //define variables
    struct map layout;
    
    struct trapezoid block;
    struct trapezoid free_space;

    free_space.a = -1;
    free_space.b = 0;
    free_space.c = 10;
    free_space.d = 50;

    block.a = 40;
    block.b = 80;
    block.c = 90;
    block.d = 160;

    //initialization parameters
    //these variables describe the map of the circle and blocks
    //get from user input: number of towers, locations, target and robot location
    printf("---MCL Simulation---\n");
    
    // layout = init_map();

    // u08 test = simulate_sensor(layout);
    // printf("%u",test);


    //other useful variables
    // particle particle_array[PARTICLE_COUNT];
    // u08  sensor = 0; //IR sensor measuring distance to objects
    
    // init_part_array(particle_array, PARTICLE_COUNT);

    // //advance robot and take sensor reading
    // robot_location = advance(robot_location, 5);
    // sensor = simulate_sensor(robot_location, tower_locations, NUM_TOWERS);


    // - for each particle:
    //     ~ advance the particle (and add noise)
    //     ~ categorize particle (tower or free space)
    //     ~ compute weight for particle
    // - resample particles



    

    return 0;
}
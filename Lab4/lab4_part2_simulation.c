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

#include <math.h>
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>


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
// int read_range_finder(void){
//     return analog(ANALOG2_PIN);;
// }

struct map{
    float location[NUM_TOWERS];
    int target;
};

struct trapezoid{
    float a;
    float b;
    float c;
    float d;
};

void prob_given_tower_or_free(float sensor, struct trapezoid type, float *probability){
   
    float u = 2 /(type.c + type.d - type.a - type.b);
    
    if(type.a <= sensor && sensor < type.b){*probability = u * (sensor - type.a) / (type.b - type.a);}
    
    else if(type.b <= sensor && sensor < type.c){*probability = u;}

    else if(type.c <= sensor && sensor < type.d){*probability = u * (type.d - sensor) / (type.c - type.d);}

    else{*probability = 0;}

    if(*probability < 0){*probability = 0;}

}

// verifited
void classify_particles(float* particle, int* classify, struct map towers){
    
    for(int j = 0; j < NUM_TOWERS; j++){
        
        if((fabs(towers.location[j] - *particle)) <= BLOCK_ANGLE){
            *classify = BLOCK_TOWER;
            break;
        }

        if(towers.location[j] < *particle){
            if((towers.location[j] + (360 - *particle)) <= 1.5){
                printf("DIST: %2.3f\n",towers.location[j] + (360 - *particle));
                *classify = BLOCK_TOWER;
                break;
            }
        }

        if(towers.location[j] > *particle){
            if((*particle + (360 - towers.location[j])) <= (float)BLOCK_ANGLE){
                *classify = BLOCK_TOWER;
                break;
            }
        }
        
        else{*classify = FREE;}
    
    }
}

void advance(float degrees){
}

int main(){

    printf("Start\n");
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
    for(int i = 0; i < PARTICLE_COUNT - 1; i++){
        
        // particles[i] = 360 * (float) rand() / RAND_MAX;
        particles[i] = (float)(i * 360 / PARTICLE_COUNT);
        printf("Particle %d: %2.3f\n",i,particles[i]);
    }

    particles[99] = 359;
    printf("Particle 99: %2.3f\n",particles[99]);


    int ir_values[50];

    for(int i=0; i < 50; i++){
        ir_values[i] = 45-i;
        if(i > 25){
            ir_values[i] = 5;
        }
    }
    

    struct map towers;

    //create map
    towers.location[0] = 0.5; 
    // towers.location[0] = 359.5; 
    towers.location[1] = 90; 
    towers.location[2] = 225; 
    towers.target = 1; 

    //change these values
    struct trapezoid block;

    struct trapezoid free_space;
    
    free_space.d = 0;
    free_space.d = 9;
    free_space.b = 12;
    free_space.a = 30;

    block.d = 25;
    block.c = 40;
    block.b = 50;
    block.a = 55;

    int count = 0;

    // while(count < 1){

        ir_value = ir_values[count];
        
        sum = 0;
        
        //classify particles as tower or free space, assign probabilites
        for(int i = 0; i < PARTICLE_COUNT; i++){
            
            //classify
            classify_particles(&particles[i], &classify[i], towers);
            
            //assign probabilities
            if(classify[i] == BLOCK_TOWER){
                printf("Particle %d: Tower\n",i);
                prob_given_tower_or_free(ir_value, block, &probabilities[i]);
            }

            else{
                printf("Particle %d: Free Space\n",i);
                prob_given_tower_or_free(ir_value, free_space, &probabilities[i]);
            }
            //get running sum
            sum += probabilities[i];
        }
        // printf("Sum : %2.3f\n",sum);
        
        //normalize probabilities
        for(int i = 0; i < PARTICLE_COUNT; i++){
            probabilities[i] = probabilities[i] / sum;
            // printf("Probability Particle %d: %2.3f\n",i,probabilities[i]);
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

            // printf("Sampled Particle %d: %d\n",i,j);

            
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
        for(int i = 0; i < PARTICLE_COUNT; i++){
            new_particles[i] += new_particles[i] + (float) MOTION_DEGREES +  gaussian_sample(0, (float)MOTION_NOISE_DEV);
            if(new_particles[i] > 360){
                new_particles[i] = new_particles[i] - 360;
            }
            mean += new_particles[i];
            // printf("Mean: %f\n",mean);
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

        memcpy(new_particles, particles, PARTICLE_COUNT*sizeof(float));
        for(int i = 0; i < PARTICLE_COUNT; i++){
            // printf("Updated Particle %d: %2.3f\n",i,particles[i]);
        }

        // printf("Standard Deviation: %2.3f Mean:%2.3f \n",std_dev,mean);
        
        //check if particles are grouped together, localization protocol
        
        if(std_dev < MOTION_NOISE_DEV * 3){
            
            printf("Done\n");

            return 0;

        }
    count++;   
    // }
    
    return 0;
}

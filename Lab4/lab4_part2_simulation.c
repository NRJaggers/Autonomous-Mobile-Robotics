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
#define MOTION_DEGREES 2.8

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
   
    //max probability, top of trapizoid
    float u = 2 /(type.c + type.d - type.a - type.b);
    
    //determine probability of sensor reading based on where it falls on trapizoid
    if(type.a <= sensor && sensor < type.b){*probability = u * (sensor - type.a) / (type.b - type.a);}
    
    else if(type.b <= sensor && sensor < type.c){*probability = u;}

    else if(type.c <= sensor && sensor < type.d){*probability = u * (type.d - sensor) / (type.d - type.c);}

    else{*probability = 0;}

   // if(*probability < 0){*probability = 0;}

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
    for(int i = 0; i < PARTICLE_COUNT; i++){
        
        particles[i] = 360 * (float) rand() / RAND_MAX;
        // particles[i] = (float)(i * 360 / PARTICLE_COUNT);
        printf("Particle %d: %2.3f\n",i,particles[i]);
    }


    int ir_values[35];
    
    ir_values[0] = 40; // 0 degrees
    ir_values[1] = 11; // 3 degrees
    ir_values[2] = 11; // 6 degrees
    ir_values[3] = 11; // 9 degrees
    ir_values[4] = 11; // 12 degrees
    ir_values[5] = 11; // 15 degrees
    ir_values[6] = 11; // 18 degrees
    ir_values[7] = 11; // 21 degrees
    ir_values[8] = 11; // 24 degrees
    ir_values[9] = 11; // 27 degrees
    ir_values[10] = 11; // 30 degrees
    ir_values[11] = 11; // 33 degrees
    ir_values[12] = 11; // 36 degrees
    ir_values[13] = 11; // 39 degrees
    ir_values[14] = 11; // 42 degrees
    ir_values[15] = 11; // 45 degrees
    ir_values[16] = 11; // 48 degrees
    ir_values[17] = 11; // 51 degrees
    ir_values[18] = 11; // 54 degrees
    ir_values[19] = 11; // 57 degrees
    ir_values[20] = 11; // 60 degrees
    ir_values[21] = 11; // 63 degrees
    ir_values[22] = 11; // 66 degrees
    ir_values[23] = 11; // 69 degrees
    ir_values[24] = 11; // 72 degrees
    ir_values[25] = 11; // 75 degrees
    ir_values[26] = 11; // 78 degrees
    ir_values[27] = 11; // 81 degrees
    ir_values[28] = 11; // 84 degrees
    ir_values[29] = 11; // 87 degrees
    ir_values[30] = 40; // 90 degrees
    ir_values[31] = 11; // 93 degrees
    ir_values[32] = 11; // 96 degrees
    ir_values[33] = 11; // 99 degrees
    ir_values[34] = 11; // 102 degrees


    struct map towers;

    //create map
    // towers.location[0] = 0.5; 
    // towers.location[0] = 359.5; 
    towers.location[0] = 0;
    towers.location[1] = 90; 
    towers.location[2] = 225; 
    towers.target = 1; 

    //change these values
    struct trapezoid block;

    struct trapezoid free_space;
    
    free_space.a = 0;
    free_space.b = 2;
    free_space.c = 10;
    free_space.d = 25;

    block.a = 20;
    block.b = 30;
    block.c = 50;
    block.d = 60;

    int count = 0;

    while(count < 35){

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

            printf("Sampled Particle %d: %d\n",i,j-1);

            
            new_particles[i] = particles[j - 1]; //must subtact b/c of while loop 
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
            new_particles[i] += new_particles[i] + (float)MOTION_DEGREES +  gaussian_sample(0, (float)MOTION_NOISE_DEV);
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

        memcpy(particles,new_particles, PARTICLE_COUNT*sizeof(float));

        for(int i = 0; i < PARTICLE_COUNT; i++){
           printf("%2.3f\n",particles[i]);
        }

        printf("Standard Deviation: %2.3f Mean:%2.3f \n",std_dev,mean);
        
        //check if particles are grouped together, localization protocol
        
        if(std_dev < MOTION_NOISE_DEV * 3){
            
            printf("Done\n");

            return 0;

        }
    count++;   
    }
    
    return 0;
}

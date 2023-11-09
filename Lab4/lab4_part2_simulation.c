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
#define MOTION_NOISE_DEV 0
#define MOTION_DEGREES 3

//Box-Muller Transform: function to create sample from gaussian curve
float gaussian_sample(float shift, float scale){
    
    float u1 = (float) rand() / RAND_MAX;
    
    float u2 = (float) rand() / RAND_MAX;

    float z = sqrtf(-2 * logf(u1)) * cosf(2 * PI * u2);

    //add shift = 0 and multiply scale = 1 for normal distribution
    return (z * scale) + shift;

}

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

    else if(type.c <= sensor && sensor < type.d){
        *probability = u * (type.d - sensor) / (type.d - type.c);
        }

    else{*probability = 0;}
}

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
        // printf("Particle %d: %2.3f\n",i,particles[i]);
    }


    int ir_values[80];
    
    ir_values[0] = 7; // 0 degrees
    ir_values[1] = 25; // 3 degrees
    ir_values[2] = 8; // 6 degrees
    ir_values[3] = 8; // 9 degrees
    ir_values[4] = 8; // 12 degrees
    ir_values[5] = 8; // 15 degrees
    ir_values[6] = 8; // 18 degrees
    ir_values[7] = 8; // 21 degrees
    ir_values[8] = 8; // 24 degrees
    ir_values[9] = 8; // 27 degrees
    ir_values[10] = 8; // 30 degrees

    ir_values[11] = 7; // 33 degrees
    ir_values[12] = 7; // 36 degrees
    ir_values[13] = 7; // 39 degrees
    ir_values[14] = 7; // 42 degrees
    ir_values[15] = 7; // 45 degrees
    ir_values[16] = 7; // 48 degrees
    ir_values[17] = 7; // 51 degrees
    ir_values[18] = 7; // 54 degrees
    ir_values[19] = 7; // 57 degrees
    ir_values[20] = 7; // 60 degrees

    ir_values[21] = 7; // 63 degrees
    ir_values[22] = 7; // 66 degrees
    ir_values[23] = 7; // 69 degrees
    ir_values[24] = 7; // 72 degrees
    ir_values[25] = 7; // 75 degrees
    ir_values[26] = 7; // 78 degrees
    ir_values[27] = 7; // 81 degrees
    ir_values[28] = 7; // 84 degrees
    ir_values[29] = 7; // 87 degrees
    ir_values[30] = 40; // 90 degrees

    ir_values[31] = 7; // 93 degrees
    ir_values[32] = 7; // 96 degrees
    ir_values[33] = 7; // 99 degrees
    ir_values[34] = 7; // 102 degrees
    ir_values[35] = 7; // 105 degrees
    ir_values[36] = 7; // 108 degrees
    ir_values[37] = 7; // 111 degrees
    ir_values[38] = 7; // 114 degrees
    ir_values[39] = 7; // 117 degrees
    ir_values[40] = 7; // 120 degrees

    ir_values[41] = 7; // 123 degrees
    ir_values[42] = 7; // 126 degrees
    ir_values[43] = 7; // 129 degrees
    ir_values[44] = 7; // 132 degrees
    ir_values[45] = 7; // 135 degrees
    ir_values[46] = 7; // 138 degrees
    ir_values[47] = 7; // 141 degrees
    ir_values[48] = 7; // 144 degrees
    ir_values[49] = 7; // 147 degrees
    ir_values[50] = 7; // 150 degrees

    ir_values[51] = 7; // 153 degrees
    ir_values[52] = 7; // 157 degrees
    ir_values[53] = 7; // 160 degrees
    ir_values[54] = 7; // 163 degrees
    ir_values[55] = 7; // 166 degrees
    ir_values[56] = 7; // 169 degrees
    ir_values[57] = 7; // 172 degrees
    ir_values[58] = 7; // 175 degrees
    ir_values[59] = 7; // 178 degrees
    ir_values[60] = 7; // 180 degrees

    ir_values[61] = 7; // 183 degrees
    ir_values[62] = 7; // 187 degrees
    ir_values[63] = 7; // 190 degrees
    ir_values[64] = 7; // 193 degrees
    ir_values[65] = 7; // 196 degrees
    ir_values[66] = 7; // 199 degrees
    ir_values[67] = 7; // 202 degrees
    ir_values[68] = 7; // 205 degrees
    ir_values[69] = 7; // 208 degrees
    ir_values[70] = 7; // 210 degrees

    ir_values[71] = 7; // 213 degrees
    ir_values[72] = 7; // 216 degrees
    ir_values[73] = 7; // 219 degrees
    ir_values[74] = 7; // 222 degrees
    ir_values[75] = 40; // 225 degrees
    ir_values[76] = 7; // 228 degrees
    ir_values[77] = 7; // 231 degrees
    ir_values[78] = 7; // 234 degrees
    ir_values[79] = 7; // 237 degrees





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
    
    //there should be a little of overlap between the freespace and block
    //trapizoids to catch edge cases where sensor is near edge of block
    free_space.a = 0;
    free_space.b = 2;
    free_space.c = 10;
    free_space.d = 25;

    block.a = 20;
    block.b = 80;
    block.c = 100;
    block.d = 105;

    int count = 0;

    while(count < 80){

        ir_value = ir_values[count];
        
        sum = 0;
        
        //classify particles as tower or free space, assign probabilites
        for(int i = 0; i < PARTICLE_COUNT; i++){
            
            //classify
            classify_particles(&particles[i], &classify[i], towers);
            
            //assign probabilities
            if(classify[i] == BLOCK_TOWER){
                // printf("Particle %d: Tower\n",i);
                prob_given_tower_or_free(ir_value, block, &probabilities[i]);
            }

            else{
                // printf("Particle %d: Free Space\n",i);
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
            
            new_particles[i] = particles[j - 1]; //must subtact b/c of while loop 

            // printf("Sampled Particle %d: %d Degrees: %2.3f \n",i,j-1,new_particles[i]);
        }
   
        // add noisy particles
        for(int i = sampling_total; i < PARTICLE_COUNT; i++){
            new_particles[i] = 360 * (float) rand() / RAND_MAX; 
            // printf("Random Noise: %2.3f \n",new_particles[i]);
        }

        // update location , add gaussian noise
        for(int i = 0; i < PARTICLE_COUNT; i++){
            particles[i] = new_particles[i] + (float)MOTION_DEGREES +  gaussian_sample(0, (float)MOTION_NOISE_DEV);
            
            if(particles[i] > 360){ 
                particles[i] = particles[i] - 360;
                // printf("%d,%2.3f\n",i,new_particles[i]);
            }
            // printf("Mean: %f\n",mean);
        }

        //calculate mean and standard deviation
        float mean_angle = calc_mean(&particles[0],(float)PARTICLE_COUNT);

        float variance_sum = 0;
        float temp;
        float std_dev;
        
        //calculate variance
        for(int i = 0; i < PARTICLE_COUNT; i++){
            // float displacement = min(fabs(particles[i] - mean_angle),360 - fabs(particles[i] - mean_angle));
            temp = fabs(mean_angle - particles[i]);
            while(temp >= 180)
                temp -= 180;
            variance_sum += temp*temp;
        }

        variance_sum = variance_sum / PARTICLE_COUNT;
        std_dev = sqrtf(variance_sum);
        
        float travel_dist;

        printf("Standard Deviation: %2.3f Mean:%2.3f \n",std_dev,mean_angle);
        
        //check if particles are grouped together, localization protocol
        
        if(std_dev < 40){
            
            printf("Localized\n");

        }
    count++;   
    }

    for(int i = 0; i < PARTICLE_COUNT; i++){
        printf("%2.3f\n",particles[i]);
    }
    
    return 0;
}

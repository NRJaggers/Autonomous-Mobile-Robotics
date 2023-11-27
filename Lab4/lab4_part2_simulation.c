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
#include <time.h>

#define PI 3.141592654
#define PARTICLE_COUNT 500
#define NUM_TOWERS 3 
#define BLOCK_ANGLE 2
#define FREE 0
#define BLOCK_TOWER 1
#define MOTION_NOISE_DEV 0
#define MOTION_DEGREES 2

//Box-Muller Transform: function to create sample from gaussian curve
float gaussian_sample(float shift, float scale){
    
    //make sure u1 is not zero
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
        
        if((fabs(towers.location[j] - *particle)) <= (float)BLOCK_ANGLE){
            *classify = BLOCK_TOWER;
            break;
        }

        if(towers.location[j] < *particle){
            if(((towers.location[j] + (360 - *particle))) <= (float)BLOCK_ANGLE){
                *classify = BLOCK_TOWER;
                break;
            }
        }

        if(towers.location[j] > *particle){
            if(((*particle + (360 - towers.location[j]))) <= (float)BLOCK_ANGLE){
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

float calc_mean(float *particle, float *real, float *imaginary, float size){
    float totalx = 0;
    float totaly = 0;
    
    for(int i=0; i<size; i++){
        totalx += cosf(*particle * PI / 180);
        totaly += sinf(*particle * PI / 180);
        particle++;
    }

    totalx = totalx / size;
    totaly = totaly / size;

    *real = totalx;
    *imaginary = totaly;

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

float standard_devation(float *particle, float *real, float *imaginary, float size){
    
    float variance_sum = 0;
    
    for(int i=0; i<size; i++){

        float devx = cosf(*particle * PI / 180) - *real;
        float devy = sinf(*particle * PI / 180) - *imaginary;

        variance_sum += devx*devx + devy*devy;

        particle++;
    }

    return sqrtf(variance_sum / PARTICLE_COUNT) * 180 / PI;

}

int main(){
    srand(time(NULL));
    printf("Start\n");
    int ir_value; 

    float sum;
    float random; 
    float random_count;

    int sampling_total = (int) (PARTICLE_COUNT * 0.99);

    //initialize particle positions
    float particles[PARTICLE_COUNT];
    float probabilities[PARTICLE_COUNT];
    float new_particles[PARTICLE_COUNT];
    int classify[PARTICLE_COUNT];

    //initialize particle positions randomly
    for(int i = 0; i < PARTICLE_COUNT; i++){
        // particles[i] = 360 * (float) rand() / RAND_MAX;
        particles[i] = 360.0 * i / PARTICLE_COUNT;
        printf("Particle %d: %2.3f\n",i,particles[i]);
        // printf("%2.3f\n",particles[i]);

    }

    float ir_values[360];

    struct map towers;

    //create map
    printf("Tower 0 angle: ");
    scanf("%f", &towers.location[0]);
    printf("%2f", towers.location[0]);

    printf("\nTower 1 angle: ");
    scanf("%f", &towers.location[1]);
    printf("%2f", towers.location[1]);

    printf("\nTower 2 angle: ");
    scanf("%f", &towers.location[2]);
    printf("%2f", towers.location[2]);

    printf("\nTarget tower: ");
    scanf("%d", &towers.target);
    printf("%d\n", towers.target);
    

    //change these values
    struct trapezoid block;

    struct trapezoid free_space;
    
    //there should be a little of overlap between the freespace and block
    //trapizoids to catch edge cases where sensor is near edge of block
    free_space.a = -1;
    free_space.b = 0;
    free_space.c = 10;
    free_space.d = 60;

    block.a = 30;
    block.b = 80;
    block.c = 90;
    block.d = 120;

    int count = 0;
    
    int start_pos;
    int sim_length;
    printf("Start angle: ");
    scanf("%d", &start_pos);
    printf("%d\n", towers.target);

    printf("Sim length: ");
    scanf("%d", &sim_length);
    printf("%d\n", sim_length);


    float real;
    float imaginary;

    //make sensor values based on map, TEST
    for(int i = 0; i < 360; i++){
        
        float dist = min( 
            
            min(abs(i - towers.location[0]) , abs(i - towers.location[0] - 360)),
        
        min( 
            min(abs(i - towers.location[1]) , abs(i - towers.location[1] - 360)),
        
            min(abs(i - towers.location[2]), abs(i - towers.location[2] - 360)) 
            )
        );

        if(dist < BLOCK_ANGLE ){
            
            ir_values[i] = (85 + gaussian_sample(0,5));
        }

        else if ((dist >= BLOCK_ANGLE) && (dist <= BLOCK_ANGLE + 1)){
            ir_values[i] = (int)(45 + gaussian_sample(0,5));
        }

        else{
            ir_values[i] = (5 + gaussian_sample(0,1));

        }
        // printf("Initialized angle %d: %2.3f\n",i,ir_values[i]);
    }

    while(count < sim_length){

        ir_value = ir_values[start_pos];
        
        sum = 0;
        
        //classify particles as tower or free space, assign probabilites
        for(int i = 0; i < PARTICLE_COUNT; i++){
            
            //classify
            classify_particles(&particles[i], &classify[i], towers);
            
            //assign probabilities
            if(classify[i] == BLOCK_TOWER){
                // printf("Particle %d: Tower\n",i);
                prob_given_tower_or_free(ir_value, block, &probabilities[i]);
                // printf("TOWER ANGLE: %2.3f , Probability: %2.3f\n", particles[i], probabilities[i]);
            }

            else{
                // printf("Particle %d: Free Space\n",i);
                prob_given_tower_or_free(ir_value, free_space, &probabilities[i]);
            }
            // printf("Particle Angle: %2.3f Probability: %2.3f\n", particles[i],probabilities[i]);
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
            // random = 1;
            // printf("Random Sum: %2.3f\n",random);
            random_count = 0;
            int j = 0;
           
            while(random_count <= random){
                random_count += probabilities[j];
                // printf("Random Prob: %2.3f\n",probabilities[j]);
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
            particles[i] = new_particles[i] + (float)MOTION_DEGREES;  
            // + gaussian_sample(0, 0.1);
            
            if(particles[i] >= 360){ 
                particles[i] = particles[i] - 360;
            }
        }

        //calculate mean and standard deviation
        float mean_angle = calc_mean(&particles[0],&real,&imaginary,(float)PARTICLE_COUNT);

        float std_devation = standard_devation(&particles[0],&real,&imaginary,(float)PARTICLE_COUNT);
        

        printf("Ref angle %d Mean Angle: %2.3f Standard Deviation: %2.3f\n",start_pos,mean_angle,std_devation);

        count++;
        start_pos+= MOTION_DEGREES;
        if (start_pos >= 360){

            start_pos = 0;

        }   

    }

    // for(int i = 0; i < PARTICLE_COUNT; i++){
    //     printf("%2.3f\n",particles[i]);
    // }
    
    return 0;
}

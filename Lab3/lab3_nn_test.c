/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 3 Part 2

Description:


Notes:
around line 147 with:   target = compute_proportional(nD.left_sensor_values[i], nD.right_sensor_values[i]);

maybe try divide target with 100 and compute with that. 

*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define BASE_SPEED 30 //cruising speed for bot
#define ERROR_THRESH 5 // Threshold for error between sensors before control activates
#define BIAS_CONST -1/255
#define SCALE 10
#define PERCENT 100
#define SENSOR_MAX 255

#define EPOCHS 100
#define DATA_POINTS 100  // more than 50 seems to lead to memeory problems; only 4k for variables
#define PARAMS 17       //hidden layer (2 input + bias)* 3 nodes + (3 input + bias) * 2 nodes
#define ALPHA 0.015

float sigmoid(double x){
    return (float)(1 / (1 + exp(-x)));
};

float d_sigmoid(float s){
  //  float x = sigmoid(s);
    //Assuming input coming in is already sigmoided
   // return x* (1 - x);
    return s* (1 - s);
};

struct motor_command {
    float left_motor; // left motor speed
    float right_motor; // right motor speed
};

struct MotorValues { 
    float left;
    float right;
    float h1; 
    float h2;
    float h3;
};

struct NeuralData{
    float left_sensor_values[DATA_POINTS]; // make u8 or uint_8? shouldnt need to be float
    float right_sensor_values[DATA_POINTS]; // make u8 or uint_8? shouldnt need to be float
    float parameters[PARAMS]; 
};

struct motor_command compute_proportional(float left, float right)
{
    //Variables
    struct motor_command speed;
    float error = 0;
    float correction = 0;
    const float Kp = 0.25;

    //calculate error
    error = left - right;
    correction = (Kp*error);

    if ((error > ERROR_THRESH) | (error < -ERROR_THRESH)) //if positive (left greater than right)
    {
        //left on black, right on white
        //increase right speed
        speed.left_motor = BASE_SPEED - correction;
        speed.right_motor = BASE_SPEED + correction;
                            
        if (speed.left_motor<0)
            speed.left_motor=0;

        if (speed.right_motor<0)
            speed.right_motor=0;

    }
    else // equal (aka 0 or less than threshold)
    {
        //both on black (or white), maintain base speed
        speed.left_motor = BASE_SPEED;
        speed.right_motor = BASE_SPEED;
    }

    return speed;
}

struct MotorValues compute_neural_network(float left_sensor, float right_sensor, struct NeuralData d1){
    
    //scale the sensor readings to values between 0 and 1
    float left_scaled = left_sensor / SENSOR_MAX;
    float right_scaled = right_sensor / SENSOR_MAX;
    struct MotorValues m1;

    m1.h1 = sigmoid((d1.parameters[0] * left_scaled) + (d1.parameters[1] * right_scaled) + (d1.parameters[2] * BIAS_CONST));
    m1.h2 = sigmoid((d1.parameters[3] * left_scaled) + (d1.parameters[4] * right_scaled) + (d1.parameters[5] * BIAS_CONST));
    m1.h3 = sigmoid((d1.parameters[6] * left_scaled) + (d1.parameters[7] * right_scaled) + (d1.parameters[8] * BIAS_CONST));

    //sigmoid results in value from 0-1.0, therefore multiply by 100
    m1.left = sigmoid((d1.parameters[9] * m1.h1) + (d1.parameters[10] * m1.h2) + (d1.parameters[11] * m1.h3) + (d1.parameters[12] * BIAS_CONST));
    m1.right = sigmoid((d1.parameters[13] * m1.h1) + (d1.parameters[14] * m1.h2) + (d1.parameters[15] * m1.h3) + (d1.parameters[16] * BIAS_CONST));

    return m1;
}

struct NeuralData train_neural_network(int epochs_max, float alpha,  struct NeuralData nD){
    
    int epochs = 0;
    //struct TempOutputValues tV; 
    float dE[PARAMS];
    struct MotorValues mV;
    struct motor_command target;

    struct MotorValues mTest;
    struct motor_command tTest;
    
    float error;
    float error_l;
    float error_r;

    while(epochs < epochs_max){
        
     //   printf("%d\n",epochs);
        
        for(int i = 0 ; i < DATA_POINTS; i++){
            
            mV =  compute_neural_network(nD.left_sensor_values[i], nD.right_sensor_values[i],nD);
            //generate target values
            target = compute_proportional(nD.left_sensor_values[i], nD.right_sensor_values[i]);
           
            
        //update output layer
         //   float outleftTemp = (mV.left - target.left_motor) * (mV.left)*(1-mV.left);
            float outleftTemp = ((mV.left) - (target.left_motor/100)) * d_sigmoid(mV.left);
            
        //    printf("%2.6f\n",sigmoid(mV.left));

            //update w10
            dE[9] = outleftTemp * mV.h1;
         //   printf("dE: %2.6f\n",dE[9]);
            //update w11
            dE[10] = outleftTemp * mV.h2;
            //update w12
            dE[11] = outleftTemp * mV.h3;
            //update w13
            dE[12] = outleftTemp * BIAS_CONST; // may work with positive 1 too
            
            float outrightTemp = ((mV.right) - (target.right_motor/100)) * d_sigmoid(mV.right);
         //   float outrightTemp = (mV.right - target.right_motor) * (mV.right)*(1-mV.right);
            //update w14
            dE[13] = outrightTemp * mV.h1;
            //update w15
            dE[14] = outrightTemp * mV.h2;
            //update w16
            dE[15] = outrightTemp * mV.h3;
            //update w17
            dE[16] = outrightTemp * BIAS_CONST; // may work with positive 1 too
            
        //update hidden layer
            float c1Temp = ((mV.left) - (target.left_motor/100)) * d_sigmoid(mV.left);
            
            float c2Temp = ((mV.right) - (target.right_motor/100)) * d_sigmoid(mV.right);
           
            float h1Temp = d_sigmoid(mV.h1);

            float h2Temp = d_sigmoid(mV.h2);
            
            float h3Temp = d_sigmoid(mV.h3);

            //update w1
            dE[0] = ((c1Temp*nD.parameters[9]) + (c2Temp*nD.parameters[13])) * h1Temp * (nD.left_sensor_values[i]/SENSOR_MAX);
            //update w2
            dE[1] = ((c1Temp*nD.parameters[9]) + (c2Temp*nD.parameters[13])) * h1Temp * (nD.right_sensor_values[i]/SENSOR_MAX);
            //update w3
            dE[2] = ((c1Temp*nD.parameters[9]) + (c2Temp*nD.parameters[13])) * h1Temp * BIAS_CONST;

            //update w4
            dE[3] = ((c1Temp*nD.parameters[10]) + (c2Temp*nD.parameters[13])) * h2Temp * (nD.left_sensor_values[i]/SENSOR_MAX);
            //update w5
            dE[4] = ((c1Temp*nD.parameters[10]) + (c2Temp*nD.parameters[13])) * h2Temp * (nD.right_sensor_values[i]/SENSOR_MAX);
            //update w6
            dE[5] = ((c1Temp*nD.parameters[10]) + (c2Temp*nD.parameters[13])) * h2Temp * BIAS_CONST;

            //update w7
            dE[6] = ((c1Temp*nD.parameters[11]) + (c2Temp*nD.parameters[15])) * h3Temp * (nD.left_sensor_values[i]/SENSOR_MAX);
            //update w8
            dE[7] = ((c1Temp*nD.parameters[11]) + (c2Temp*nD.parameters[15])) * h3Temp * (nD.right_sensor_values[i]/SENSOR_MAX);
            //update w9
            dE[8] = ((c1Temp*nD.parameters[11]) + (c2Temp*nD.parameters[15])) * h3Temp * BIAS_CONST;

            for(int j = 0 ; j < PARAMS; j++){
                nD.parameters[j] = nD.parameters[j] - (alpha * dE[j]);
            }

        } 
        
        
    
        for(int i = 0; i< PARAMS; i++){printf("NN Parameter %d:%.3f \n",i,nD.parameters[i]);}

        error = 0;
        // printf("---Epoch: %d---\n",epochs);
        for(int i = 0; i< DATA_POINTS; i++){
            mTest =  compute_neural_network(nD.left_sensor_values[i], nD.right_sensor_values[i],nD);

            //generate target values
            tTest = compute_proportional(nD.left_sensor_values[i], nD.right_sensor_values[i]);

            error_l = abs((mTest.left) - (tTest.left_motor/100)); // network - target
            error_r = abs((mTest.right) - (tTest.right_motor/100)); // network - target

            error += (error_r + error_l) *(error_r + error_l);
            // printf("Proportional: L:%f R:%f\n", tTest.left_motor, tTest.right_motor);
            // printf("Network:      L:%f R:%f\n", mTest.left*100, mTest.right*100);
            // printf("LErr: %5.3f RErr: %5.3f Error: %5.3f\n",error_l, error_r, error);

            
            //printf("\n");
        }
        
        
    //    printf("%5.4f\n", error / DATA_POINTS);
        epochs++;
    }

    return nD;

}


int main(){
    
    struct NeuralData trainingData;
    struct MotorValues m;    
    struct motor_command t;

    for(int i = 0 ; i < DATA_POINTS;i++){
        trainingData.left_sensor_values[i] = 255*(float)rand() / RAND_MAX;
        trainingData.right_sensor_values[i] = 255*(float)rand() / RAND_MAX;
    }
  
    for(int i = 0; i < PARAMS; i++){
        trainingData.parameters[i] = (float)rand() / RAND_MAX;
        // printf("%1.4f \n", trainingData.parameters[i]);
    }

    // printf("\n---Init Param---\n");
    // for(int k = 0 ; k < PARAMS; k++){
    //     printf("Param %d: %2.6f\n",k,trainingData.parameters[k]);
    // }
    
    trainingData = train_neural_network(1000, 0.01, trainingData);
    
    // printf("---Trained Param---\n");
    // for(int j = 0 ; j < PARAMS; j++){
    //     printf("Param %d: %2.6f\n",j,trainingData.parameters[j]);
    // }

    for(int j = 0 ; j < DATA_POINTS; j++){
        m = compute_neural_network(trainingData.left_sensor_values[j],trainingData.right_sensor_values[j], trainingData);
        t = compute_proportional(trainingData.left_sensor_values[j],trainingData.right_sensor_values[j]);

        
        printf("---- Data Point %d ----\n", j);
        // printf("NN Hidden Output 1:%.3f \n",m.h1);
        // printf("NN Hidden Output 2:%.3f \n",m.h2);
        // printf("NN Hidden Output 3:%.3f \n",m.h3);
        // printf("Hidden: %0.4f\n",trainingData.parameters[j]);
     //   printf("Inputs     L:%1.3f R%1.3f \n", trainingData.left_sensor_values[j],trainingData.right_sensor_values[j]);
        // printf("%f\n", t.right_motor);
        printf("NN Outputs L:%2.3f R%2.3f \n", m.left*100,m.right*100);
        printf("PP Outputs L:%2.3f R%2.3f \n", t.left_motor,t.right_motor);
    }
   // for(int i = 0; i< PARAMS; i++){printf("NN Parameter:%.3f \n",trainingData.parameters[i]);}

    printf("Done\n");           
    return 0;

}





  // trainingData.left_sensor_values[1] = 130;
    // trainingData.right_sensor_values[1] = 124;

    // trainingData.left_sensor_values[2] = 196;
    // trainingData.right_sensor_values[2] = 118;

    // trainingData.left_sensor_values[3] = 192;
    // trainingData.right_sensor_values[3] = 102;

    // trainingData.left_sensor_values[4] = 157;
    // trainingData.right_sensor_values[4] = 93;

    // trainingData.left_sensor_values[5] = 101;
    // trainingData.right_sensor_values[5] = 163;

    // trainingData.left_sensor_values[6] = 117;
    // trainingData.right_sensor_values[6] = 111;

    // trainingData.left_sensor_values[7] = 100;
    // trainingData.right_sensor_values[7] = 192;

    // trainingData.left_sensor_values[8] = 168;
    // trainingData.right_sensor_values[8] = 77;

    // trainingData.left_sensor_values[9] = 192;
    // trainingData.right_sensor_values[9] = 55;

    // trainingData.left_sensor_values[10] = 118;
    // trainingData.right_sensor_values[10] = 196;

    // trainingData.left_sensor_values[11] = 108;
    // trainingData.right_sensor_values[11] = 116;

    // trainingData.left_sensor_values[12] = 103;
    // trainingData.right_sensor_values[12] = 94;

    // trainingData.left_sensor_values[13] = 192;
    // trainingData.right_sensor_values[13] = 194;

    // trainingData.left_sensor_values[14] = 105;
    // trainingData.right_sensor_values[14] = 111;

    // trainingData.left_sensor_values[15] = 188;
    // trainingData.right_sensor_values[15] = 77;

    // trainingData.left_sensor_values[16] = 98;
    // trainingData.right_sensor_values[16] = 92;

    // trainingData.left_sensor_values[17] = 99;
    // trainingData.right_sensor_values[17] = 195;

    // trainingData.left_sensor_values[18] = 111;
    // trainingData.right_sensor_values[18] = 109;

    // trainingData.left_sensor_values[19] = 100;
    // trainingData.right_sensor_values[19] = 91;




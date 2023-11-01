/*
Name: Nathan Jaggers and Weston Keitz  

Assignment Number: Lab 4 Part 2

Description: Test for Monty Carlo Localization algorithm.


Notes:

*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    
    struct NeuralData trainingData;
    struct MotorValues m;    
    struct motor_command t;
    float parameters[PARAMS];
  //  float *ptr = &parameters[0];
    for(int i = 0 ; i < DATA_POINTS;i++){
        trainingData.left_sensor_values[i] = 255*(float)rand() / RAND_MAX;
        trainingData.right_sensor_values[i] = 255*(float)rand() / RAND_MAX;
    }
  
    for(int i = 0; i < PARAMS; i++){
        parameters[i] = (float)rand() / RAND_MAX;
        // printf("\n---Init Param---\n");

        // printf("%1.4f \n", trainingData.parameters[i]);
    }

 
    train_neural_network(500, 0.07, &trainingData, parameters);
    
    // printf("---Trained Param---\n");
    // for(int j = 0 ; j < PARAMS; j++){
    //     printf("Param %d: %2.6f\n",j,trainingData.parameters[j]);
    // }

 //   printf("NN Outputs L:%2.3f R%2.3f \n", m.left*100,m.right*100);
    for(int j = 0 ; j < DATA_POINTS; j++){
        m = compute_neural_network(trainingData.left_sensor_values[j],trainingData.right_sensor_values[j], parameters);
        t = compute_proportional(trainingData.left_sensor_values[j],trainingData.right_sensor_values[j]);

        
        printf("---- Data Point %d ----\n", j);
        // printf("NN Hidden Output 1:%.3f \n",m.h1);
        // printf("NN Hidden Output 2:%.3f \n",m.h2);
        // printf("NN Hidden Output 3:%.3f \n",m.h3);
        // printf("Hidden: %0.4f\n",trainingData.parameters[j]);
     //   printf("Inputs     L:%1.3f R%1.3f \n", trainingData.left_sensor_values[j],trainingData.right_sensor_values[j]);
        // printf("%f\n", t.right_motor);
        printf("NN Outputs L:%2.3f R:%2.3f \n", m.left*100,m.right*100);
        printf("NN Ints    L:%3d    R:%3d \n",(int16_t)(PERCENT * m.left),(int16_t)(PERCENT * m.right));
        printf("PP Outputs L:%2.3f R:%2.3f \n", t.left_motor,t.right_motor);
    }
//    // for(int i = 0; i< PARAMS; i++){printf("NN Parameter:%.3f \n",trainingData.parameters[i]);}

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




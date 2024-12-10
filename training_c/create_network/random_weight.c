#include "random_weight.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double initializeWeight(double min, 
                        double max, 
                        int outputNodeSize, 
                        int inputNodeSize,
                        int initializeType){
    if(initializeType == Random){
        return initializeRandom(min,max);
    }else if(initializeType == Xavier){
        return initializeXavier(outputNodeSize, inputNodeSize);
    }else if(initializeType == He){
        return initializeHe(outputNodeSize);
    }
    printf("initializing type error\n");
    exit(1);
}

double initializeRandom(double min, double max){
    return (((double)rand() / RAND_MAX) * (max - min) + min);
}

double initializeXavier(int outputNodeSize, int inputNodeSize){
    double limit = sqrt(6.0/(outputNodeSize+inputNodeSize));
    return (((double)rand() / RAND_MAX) * 2 * limit - limit);
}

double initializeHe(int outputNodeSize){
    double stddev = sqrt(2.0/outputNodeSize);
    return (((double)rand()/RAND_MAX) * 2 * stddev - stddev);
    // return (stddev * ((float)rand() / RAND_MAX));
}
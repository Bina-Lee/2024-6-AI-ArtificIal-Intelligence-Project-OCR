#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define Random 1
#define Xavier 2
#define He 3

double initializeWeight(double min, 
                        double max, 
                        int outputNodeSize, 
                        int inputNodeSize,
                        int initializeType);

double initializeRandom(double min, double max);

double initializeXavier(int outputNodeSize, int inputNodeSize);

double initializeHe(int outputNodeSize);
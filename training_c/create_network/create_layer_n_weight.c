#include "create_layer_n_weight.h"
#include "random_weight.h"
#include <stdlib.h>

double* layerCreation(int n){
    double* layer = (double*)malloc(sizeof(double) * n);
    return layer;
}

double** weightTableCreation(int numberOfOutputNode, 
                            int numberOfInputNode, 
                            int initializeType){
    double** weightTable = (double**)malloc(sizeof(double*) * numberOfOutputNode);
    for(int i=0;i<numberOfOutputNode;i++){
        weightTable[i] = (double*)malloc(sizeof(double) * numberOfInputNode);
        for(int j=0;j<numberOfInputNode;j++){
            weightTable[i][j] = initializeWeight(-0.5, 0.5, 
                                                numberOfOutputNode, 
                                                numberOfInputNode,
                                                He);    //Random,Xavier,He
        }
    }
    return weightTable;
}
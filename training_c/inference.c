#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "load_n_preprocess/load_data.h"
#include "load_n_preprocess/preprocessing.h"

#include "csv_operate/import_weight_csv.h"

#include "typedef_struct/korean_word.h"
#include "math_func/math_func.h"
#include "create_network/create_layer_n_weight.h"

#include "word_inference/word_inference.h"

#include <stdio.h>
#include <stdlib.h>

#define SIGMOID     1
#define TANH        2
#define RELU        3
#define LEAKY_RELU  4
#define SOFTMAX     5

#define HiddenLayerInit
#define OutputLayerInit

#define HiddenLayerActive RELU
    //SIGMOID, TANH, RELU, LEAKY_RELU
#define OutputLayerActive SIGMOID

double clip(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

void clipGradient(double* delta, int size, double min, double max) {
    for (int i = 0; i < size; i++) {
        delta[i] = clip(delta[i], min, max);
    }
}

void clipWeights(double*** weight, int L, int* N, double min, double max) {
    for (int i = 0; i < L - 1; i++) {
        for (int j = 0; j < N[i]; j++) {
            for (int k = 0; k < N[i+1]; k++) { 
                weight[i][j][k] = clip(weight[i][j][k], min, max);
            }
        }
    }
}

double activationFunction(double input, int activate){
    input = clip(input, -10.0, 10.0);
    if(activate == SIGMOID){
        return sigmoid(input);
    }else if(activate == TANH){
        return tanh_clip(input);
    }else if(activate == RELU){
        return relu(input);
    }else if(activate == LEAKY_RELU){
        return leaky_relu(input,0.1);
    // }else if(activate == SOFTMAX){

    }
    return -9999.9999;
}

double activationFunctionDerivative(double input, int activate){
    if(activate == SIGMOID){
        return sigmoid_derivative(input);
    }else if(activate == TANH){
        return tanh_derivative(input);
    }else if(activate == RELU){
        return relu_derivative(input);
    }else if(activate == LEAKY_RELU){
        return leaky_relu_derivative(input,0.1);
    // }else if(activate == SOFTMAX){

    }
    return -9999.9999;
}

void matrixMultiplication(double* input,
                            int inputSize, 
                            double** weight, 
                            double* output,
                            int outputSize){
    for(int i=0;i<outputSize;i++){
        double add=0.0;
        for(int j=0;j<inputSize;j++){
            add = add + input[j] * weight[j][i];
        }
        output[i] = add;
    }
}

int main(){
    double input_layer[16][16] = {0,};
    {//Load image and preprocessing
        unsigned char pixels[64][64] = {0,};
        FILE* openfile = load_image_n_data(pixels,0);
        cropRange imageCropRange = cropIndex(pixels);
        unsigned char stretchedImage[64][64]={0,};
        imageStretch(pixels,stretchedImage,imageCropRange);
        image64to16(stretchedImage,input_layer);
        fclose(openfile);
    }

    char filepath[200];
    printf("Enter Weight's File Path : ");
    scanf("%s",filepath);
    int L;
    int* N;
    double** Z;
    double** H;
    double*** weight;
    load_weights_from_csv(filepath, &L, &N, &weight);

    Z = (double**)malloc(sizeof(double*) * L);
    H = (double**)malloc(sizeof(double*) * L);
    for(int i=0;i<L;i++){
        Z[i] = layerCreation(N[i]);
        H[i] = layerCreation(N[i]);
    }
    for(int i=0;i<N[0];i++){
        Z[0][i] = input_layer[i/16][i%16];
        H[0][i] = Z[0][i];
    }




    for(int i=0;i<(L-1);i++){
        matrixMultiplication(H[i],N[i],weight[i],Z[i+1],N[i+1]);
        if(i<L-2){
            for(int j=0;j<N[i+1];j++){
                H[i+1][j] = activationFunction(Z[i+1][j],HiddenLayerActive);
            }
        }else{
            for(int j=0;j<N[i+1];j++){
                H[i+1][j] = activationFunction(Z[i+1][j],OutputLayerActive);
            }
        }
    }





    WORDVAR output = (WORDVAR){H[L-1][0],H[L-1][1],H[L-1][2]};
    WORDIDX conv = convert_VAR2IDX(output);
    char* character = convert_IDX2Char(conv);
    printf("Character: %s\n", character);
    free(character);

    {
        for(int i=0;i<(L-1);i++){
            for(int j=0;j<N[i];j++)free(weight[i][j]);
            free(weight[i]);
        }
        free(weight);

        free(N);

        for(int i=0;i<L;i++)free(Z[i]);
        for(int i=0;i<L;i++)free(H[i]);
        free(Z);
        free(H);
    }

    return 0;
}
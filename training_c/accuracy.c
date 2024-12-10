#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "load_n_preprocess/load_data.h"
#include "load_n_preprocess/preprocessing.h"
#include "load_n_preprocess/load_target.h"
#include "typedef_struct/korean_word.h"
#include "math_func/math_func.h"
#include "create_network/create_layer_n_weight.h"
#include "create_network/random_weight.h"
#include "csv_operate/export_weight_csv.h"
#include "word_inference/word_inference.h"

#include "csv_operate/import_weight_csv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


    int fileIdx = 1;

    char filepath[200];
    printf("Enter Weight's File Path : ");
    scanf("%s",filepath);
    char filename[200];

    FILE *accuracyFile = fopen("accuracy.txt", "w");
    FILE *accuracyFileNot = fopen("notCorrectLog.txt", "w");
    // if (accuracyFile == NULL) {
    //     perror("파일 열기 실패");
    //     return 1; // 프로그램 종료
    // }

    int all = 0;
    int correct = 0;

    int all3 = 0;
    int correct3 = 0;

    int L;
    int* N;
    double** Z;
    double** H;
    double*** weight;
    load_weights_from_csv(filepath, &L, &N, &weight);

    for(int fileIdx = 0;fileIdx<3388;fileIdx++){

        double input_layer[16][16] = {0,};
        {//Load image and preprocessing
            unsigned char pixels[64][64] = {0,};
            FILE* openfile = load_image_n_data(pixels,fileIdx+1);
            cropRange imageCropRange = cropIndex(pixels);
            unsigned char stretchedImage[64][64]={0,};
            imageStretch(pixels,stretchedImage,imageCropRange);
            image64to16(stretchedImage,input_layer);
            fclose(openfile);
        }
        WORDIDX targetIDX = find_target(fileIdx+1);

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
        char* targetChar = convert_IDX2Char(targetIDX);
        char ifcorrect = strcmp(character, targetChar)?'X':'O';
        printf("file idx : %d / Character: %s / target : %s / %c /", 
                fileIdx+1, character, targetChar, ifcorrect);

        fprintf(accuracyFile, "file idx : %d / Character: %s / target : %s / %c /", 
                                fileIdx+1, character, targetChar, ifcorrect);
        if(ifcorrect == 'X')fprintf(accuracyFileNot,"file idx : %d / Character: %s / target : %s / %c /", 
                                fileIdx+1, character, targetChar, ifcorrect);

        all++;
        if(ifcorrect == 'O')correct++;
        if(conv.first == targetIDX.first)correct3++;
        if(conv.middle == targetIDX.middle)correct3++;
        if(conv.last == targetIDX.last)correct3++;
        all3+=3;
        free(character);
        free(targetChar);

        double accuracy = (double)correct / (double)all;
        double accuracy3 = (double)correct3 / (double)(all3);
        printf("Accuracy : %lf / Accuracy3 : %lf\n",accuracy,accuracy3);
        fprintf(accuracyFile, "Accuracy : %lf / Accuracy3 : %lf\n",accuracy,accuracy3);

        if(ifcorrect == 'X')fprintf(accuracyFileNot, "Accuracy : %lf / Accuracy3 : %lf\n",accuracy,accuracy3);


        for(int i=0;i<L;i++)free(Z[i]);
        for(int i=0;i<L;i++)free(H[i]);
    }

    fclose(accuracyFile);
    fclose(accuracyFileNot);

    {
        for(int i=0;i<(L-1);i++){
            for(int j=0;j<N[i];j++)free(weight[i][j]);
            free(weight[i]);
        }
        free(weight);

        free(N);

        free(Z);
        free(H);
    }

    return 0;
}
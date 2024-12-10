#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// #include <windows.h>
#include <pthread.h>

#include "load_n_preprocess/load_data.h"
#include "load_n_preprocess/preprocessing.h"
#include "load_n_preprocess/load_target.h"
#include "typedef_struct/korean_word.h"
#include "math_func/math_func.h"
#include "create_network/create_layer_n_weight.h"
#include "create_network/random_weight.h"
#include "csv_operate/export_weight_csv.h"
#include "word_inference/word_inference.h"

//font : 28 / char : 111
#define SAMPLE      3388
#define BATCH_SIZE  121
#define ALL_S       28
#define TRAINING_S  21
#define TEST_S      7

double LEARNING_RATE=0.1;

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

typedef struct{
    int thread_id;

    int fileIdx; 
    int L;
    int* N;
    double** Z; 
    double** H;
    double*** weight;
    WORDVAR* target;

    int size;
}forwardThreadData;

typedef struct{
    int thread_id;

    int fileIdx; 
    int L;
    int* N;
    double** Z; 
    double** H;
    double*** weight;
    WORDVAR* target;

    double** delta;
    double learningRate;

    int size;
}backwardThreadData;

void* forwardPropagation(void* arg){
    forwardThreadData* data = (forwardThreadData*)arg;

    int fileIdx         = data->fileIdx;
    int L               = data->L;
    int* N              = data->N;
    double** Z          = data->Z;
    double** H          = data->H;
    double*** weight    = data->weight;
    WORDVAR* target     = data->target;

    // int fileIdx = 1;

    double input_layer[16][16] = {0,};
    {//Load image and preprocessing
        unsigned char pixels[64][64] = {0,};
        FILE* openfile = load_image_n_data(pixels,fileIdx);
        cropRange imageCropRange = cropIndex(pixels);
        unsigned char stretchedImage[64][64]={0,};
        imageStretch(pixels,stretchedImage,imageCropRange);
        image64to16(stretchedImage,input_layer);
        fclose(openfile);
    }
    WORDIDX targetIDX = find_target(fileIdx);
    *target = convert_IDX2VAR(targetIDX);

    //initialized input value from input_layer 16x16image
    for(int i=0;i<N[0];i++){
        Z[0][i] = input_layer[i/16][i%16];
        H[0][i] = Z[0][i];
    }

    //forward propagation
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

    return NULL;
}

void* backwardPropagation(void* arg){
    backwardThreadData* data = (backwardThreadData*)arg;

    int fileIdx         = data->fileIdx;
    int L               = data->L;
    int* N              = data->N;
    double** Z          = data->Z;
    double** H          = data->H;
    double*** weight    = data->weight;
    WORDVAR* target     = data->target;

    double** delta     = data->delta;
    double learningRate = data->learningRate;

    const double CLIP_MIN = -1.0;
    const double CLIP_MAX =  1.0;

    delta[L-1][0] = (H[L-1][0] - target->first) 
                    * activationFunctionDerivative(H[L-1][0], 
                    OutputLayerActive);
    delta[L-1][1] = (H[L-1][1] - target->middle) 
                    * activationFunctionDerivative(H[L-1][1], 
                    OutputLayerActive);
    delta[L-1][2] = (H[L-1][2] - target->last) 
                    * activationFunctionDerivative(H[L-1][2], 
                    OutputLayerActive);

    for (int i = 0; i < 3; i++) {
        if (delta[L-1][i] < CLIP_MIN) delta[L-1][i] = CLIP_MIN;
        if (delta[L-1][i] > CLIP_MAX) delta[L-1][i] = CLIP_MAX;
    }

    for(int i = L-2;i>=0;i--){
        for(int j=0;j<N[i];j++){
            delta[i][j] = 0.0;
            for(int k=0;k<N[i+1];k++){
                delta[i][j] += delta[i+1][k] * weight[i][j][k];
            }
            delta[i][j] *= activationFunctionDerivative(H[i][j],HiddenLayerActive);

            if (delta[i][j] < CLIP_MIN) delta[i][j] = CLIP_MIN;
            if (delta[i][j] > CLIP_MAX) delta[i][j] = CLIP_MAX;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]){

    FILE *log = fopen("training_log.csv", "w");

    double last_MSE_LOSS = 0.0;

    srand((unsigned int)time(NULL));

    int L;  //create layer and node, and weight

    printf("How many layers? Maximum 16\n");
    scanf("%d",&L);

    int* N = (int*)malloc(sizeof(int)*L);

    for(int i=1;i<L-1;i++){
        printf("How many Nodes in Hidden Layer %d? Maximum 256\n",i);
        scanf("%d",&N[i]);
    }

    //needed node size
    double** Z[BATCH_SIZE];
    double** H[BATCH_SIZE];
    double*** weight = (double***)malloc(sizeof(double**) * (L-1));
    {
        for(int i=0;i<BATCH_SIZE;i++){
            Z[i] = (double**)malloc(sizeof(double*) * L);
            H[i] = (double**)malloc(sizeof(double*) * L);

            N[0] = 256;
            Z[i][0] = layerCreation(N[0]);
            H[i][0] = layerCreation(N[0]);
            for(int j=1;j<L-1;j++){
                Z[i][j] = layerCreation(N[j]);
                H[i][j] = layerCreation(N[j]);
            }
            N[L-1] = 3;
            Z[i][L-1] = layerCreation(N[L-1]);
            H[i][L-1] = layerCreation(N[L-1]);
        }
        
        for(int i=0;i<(L-1);i++){
            weight[i] = weightTableCreation(N[i],N[i+1],He);//Random,Xavier,He
        }
    }

    WORDVAR target[BATCH_SIZE];

    pthread_t threads[BATCH_SIZE];
    forwardThreadData forwardThread_data[BATCH_SIZE];
    backwardThreadData backwardThread_data[BATCH_SIZE];

    int chunk_size = 1;

    int epoch_count = 0;
    double prev_m_last = 0.0;
    int if_minus_count = 0;
    for(int epoch_loop;;epoch_loop = (epoch_loop + TEST_S)%ALL_S){
        int start_index_epoch = epoch_loop * BATCH_SIZE;
        for(int training_loop = 0;training_loop<TRAINING_S;training_loop++){
            int training_start_index = start_index_epoch
                                        + training_loop * BATCH_SIZE;

            for(int i=0;i<BATCH_SIZE;i++){
                forwardThread_data[i].thread_id = i;
                forwardThread_data[i].fileIdx = (training_start_index + i)%SAMPLE + 1;
                forwardThread_data[i].L = L;
                forwardThread_data[i].N = N;
                forwardThread_data[i].Z = Z[i];
                forwardThread_data[i].H = H[i];
                forwardThread_data[i].weight = weight;
                forwardThread_data[i].target = &target[i];
                forwardThread_data[i].size = chunk_size; 

                if(pthread_create(&threads[i], NULL, forwardPropagation, &forwardThread_data[i]) != 0){
                    fprintf(stderr, "Error creating thread %d\n", i);
                    return 1;
                }
            }
            for(int i = 0; i < BATCH_SIZE; i++){
                pthread_join(threads[i], NULL);
            }


            // {   //one forward propagation in batch
            //     int fileIdx = 1;
            //    
            //     double input_layer[16][16] = {0,};
            //     {//Load image and preprocessing
            //         unsigned char pixels[64][64] = {0,};
            //         FILE* openfile = load_image_n_data(pixels,fileIdx);
            //         cropRange imageCropRange = cropIndex(pixels);
            //         unsigned char stretchedImage[64][64]={0,};
            //         imageStretch(pixels,stretchedImage,imageCropRange);
            //         image64to16(stretchedImage,input_layer);
            //         fclose(openfile);
            //     }
            //     WORDIDX targetIDX = find_target(fileIdx);
            //     WORDVAR targetVAR = convert_IDX2VAR(targetIDX);
            //    
            //     //initialized input value from input_layer 16x16image
            //     for(int i=0;i<N[0];i++){
            //         Z[0][i] = input_layer[i/16][i%16];
            //         H[0][i] = Z[0][i];
            //     }
            //    
            //     //forward propagation
            //     for(int i=0;i<(L-1);i++){
            //         matrixMultiplication(H[i],N[i],weight[i],Z[i+1],N[i+1]);
            //         for(int j=0;j<N[i+1];j++)H[i+1][j] = sigmoid(Z[i+1][j]);
            //     }
            // }
            //
            // double MSE = 0;
            // for(int i = 0;i<BATCH_SIZE;i++){
            //     MSE += (target[i].first  - H[i][L-1][0])*(target[i].first  - H[i][L-1][0]);
            //     MSE += (target[i].middle - H[i][L-1][1])*(target[i].middle - H[i][L-1][1]);
            //     MSE += (target[i].last   - H[i][L-1][2])*(target[i].last   - H[i][L-1][1]);
            // }
            // MSE /= BATCH_SIZE;


            double** delta[BATCH_SIZE];
            for(int i=0;i<BATCH_SIZE;i++){
                delta[i] = (double**)malloc(sizeof(double*) * L);
                for(int j=0;j<L;j++){
                    delta[i][j] = (double*)malloc(sizeof(double) * N[j]);
                }
            }

            for(int i=0;i<BATCH_SIZE;i++){
                backwardThread_data[i].thread_id = i;
                backwardThread_data[i].fileIdx = (training_start_index + i)%SAMPLE + 1;
                backwardThread_data[i].L = L;
                backwardThread_data[i].N = N;
                backwardThread_data[i].Z = Z[i]; 
                backwardThread_data[i].H = H[i];
                backwardThread_data[i].weight = weight;
                backwardThread_data[i].target = &target[i];

                backwardThread_data[i].delta = delta[i];
                backwardThread_data[i].learningRate = LEARNING_RATE;
                backwardThread_data[i].size = chunk_size;  

                if(pthread_create(&threads[i], NULL, backwardPropagation, &backwardThread_data[i]) != 0){
                    fprintf(stderr, "Error creating thread %d\n", i);
                    return 1;
                }
            }
            for(int i = 0; i < BATCH_SIZE; i++){
                pthread_join(threads[i], NULL);
            }

            for(int i=0;i<L-1;i++){
                for(int j=0;j<N[i];j++){
                    for(int k=0;k<N[i+1];k++){
                        double gradientSum = 0.0;
                        for(int n=0;n<BATCH_SIZE;n++){
                            gradientSum += delta[n][i+1][k] * H[n][i][j];
                        }
                        weight[i][j][k] -= LEARNING_RATE * (gradientSum / BATCH_SIZE);
                    }
                }
            }
            clipWeights(weight, L, N, -5.0, 5.0);

            for(int i=0;i<BATCH_SIZE;i++){
                for(int j=0;j<L;j++)free(delta[i][j]);
                free(delta[i]);
            }

        }   //training_loop end

        int start_index_test = start_index_epoch
                                + TRAINING_S * BATCH_SIZE;
        double LOSS_MSE = 0.0;

        double calc_accuracy;
        double calc_accuracy_pm1;
        double calc_accuracy_pm2;

        int all_accuracy = 0;
        int cor_accuracy = 0;
        int cor_pm1_accuracy = 0;
        int cor_pm2_accuracy = 0;

        for(int test_loop = 0;test_loop<TEST_S;test_loop++){
            int test_start_index = start_index_test
                                + test_loop * BATCH_SIZE;

            for(int i=0;i<BATCH_SIZE;i++){
                forwardThread_data[i].thread_id = i;
                forwardThread_data[i].fileIdx = (test_start_index + i)%SAMPLE + 1;
                forwardThread_data[i].L = L;
                forwardThread_data[i].N = N;
                forwardThread_data[i].Z = Z[i];
                forwardThread_data[i].H = H[i];
                forwardThread_data[i].weight = weight;
                forwardThread_data[i].target = &target[i];
                forwardThread_data[i].size = chunk_size; 

                if(pthread_create(&threads[i], NULL, forwardPropagation, &forwardThread_data[i]) != 0){
                    fprintf(stderr, "Error creating thread %d\n", i);
                    return 1;
                }
            }
            for(int i = 0; i < BATCH_SIZE; i++){
                pthread_join(threads[i], NULL);
            }

            for(int i=0;i<BATCH_SIZE;i++){
                WORDVAR error;
                error.first  = target[i].first  -H[i][L-1][0];
                error.middle = target[i].middle -H[i][L-1][1];
                error.last   = target[i].last   -H[i][L-1][2];
                error.first  = error.first  * error.first;
                error.middle = error.middle * error.middle;
                error.last   = error.last   * error.last;
                
                double error3 = error.first + error.middle + error.last;
                error3 /=3;
                LOSS_MSE += error3;

                WORDIDX output_IDX   = convert_VAR2IDX((WORDVAR)
                                                        {H[i][L-1][0],
                                                        H[i][L-1][1],
                                                        H[i][L-1][2]});
                WORDIDX for_accuracy = convert_VAR2IDX(*target);

                int ac_1 = (output_IDX.first  - for_accuracy.first);
                int ac_2 = (output_IDX.middle - for_accuracy.middle);
                int ac_3 = (output_IDX.last   - for_accuracy.last);
                unsigned int ac_1_s = (unsigned int)(ac_1 * ac_1);
                unsigned int ac_2_s = (unsigned int)(ac_2 * ac_2);
                unsigned int ac_3_s = (unsigned int)(ac_3 * ac_3);

                if(ac_1_s == 0)cor_accuracy++;
                if(ac_2_s == 0)cor_accuracy++;
                if(ac_3_s == 0)cor_accuracy++;
                if(ac_1_s < 2) cor_pm1_accuracy++;
                if(ac_2_s < 2) cor_pm1_accuracy++;
                if(ac_3_s < 2) cor_pm1_accuracy++;
                if(ac_1_s < 5) cor_pm2_accuracy++;
                if(ac_2_s < 5) cor_pm2_accuracy++;
                if(ac_3_s < 5) cor_pm2_accuracy++;

                all_accuracy +=3;
            }   

        }   //test_loop end

        calc_accuracy     = ((double)cor_accuracy)     / ((double)all_accuracy);
        calc_accuracy_pm1 = ((double)cor_pm1_accuracy) / ((double)all_accuracy);
        calc_accuracy_pm2 = ((double)cor_pm2_accuracy) / ((double)all_accuracy);

        LOSS_MSE /= BATCH_SIZE * TEST_S;

        epoch_count++;

        prev_m_last = last_MSE_LOSS - LOSS_MSE;

        printf("Epoch : %d / accuracy : %lf / %lf / %lf / MSE : %lf / prev-last : %lf\n",epoch_count,
                                                                                        calc_accuracy,
                                                                                        calc_accuracy_pm1,
                                                                                        calc_accuracy_pm2, 
                                                                                        LOSS_MSE, 
                                                                                        prev_m_last);
        fprintf(log, "%d,%lf,%lf,%lf,%lf,%lf\n",epoch_count,
                                                calc_accuracy,
                                                calc_accuracy_pm1,
                                                calc_accuracy_pm2, 
                                                LOSS_MSE, 
                                                prev_m_last);

        last_MSE_LOSS = LOSS_MSE;

        if(isnan(LOSS_MSE) || isinf(LOSS_MSE))break;
        if(prev_m_last<0)if_minus_count++;
        // if(if_minus_count>5000)break;
        if(epoch_count > 5000)break;
        // if(LOSS_MSE * 10 < LEARNING_RATE)LEARNING_RATE/=10;
        LEARNING_RATE = LOSS_MSE * 100;
        if(LEARNING_RATE>0.1)LEARNING_RATE = 0.1;
        if(LOSS_MSE < 0.00001)break;

    }   //epoch_loop end




    // printf("print output\n");
    // for(int i=0;i<N[L-1];i++)printf("%f ",network[L-1][i]);





    // WORDIDX conv = convert_VAR2IDX(targetVAR);
    // char* character = convert_IDX2Char(conv);
    // printf("Character: %s\n", character);
    // free(character);


    fclose(log);

    save_weights_to_csv("weight.csv",L,N,weight);
    {
        for(int i=0;i<(L-1);i++){
            for(int j=0;j<N[i];j++)free(weight[i][j]);
            free(weight[i]);
        }
        free(weight);

        free(N);

        for(int i=0;i<BATCH_SIZE;i++){
            for(int j=0;j<L;j++){
                free(Z[i][j]);
                free(H[i][j]);
            }
            free(Z[i]);
            free(H[i]);
        }
    }

    return 0;
}
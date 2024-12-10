#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

double* layerCreation(int n){
    double* layer = (double*)malloc(sizeof(double) * n);
    return layer;
}

double** weightTableCreation(int numberOfOutputNode, int numberOfInputNode){
    double** weightTable = (double**)malloc(sizeof(double*) * numberOfOutputNode);
    for(int i=0;i<numberOfOutputNode;i++){
        weightTable[i] = (double*)malloc(sizeof(double) * numberOfInputNode);
        // printf("Enter w_%d,1 to w_%d,%d\n",i+1,i+1,numberOfInputNode);
        for(int j=0;j<numberOfInputNode;j++){
            // scanf("%lf",&weightTable[i][j]);
            weightTable[i][j]=(double)rand()/RAND_MAX - 0.5;
        }
    }
    return weightTable;
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

double sigmoid(double x){
    return 1.0 / (1.0 + exp(-x));
}

int main(){
    srand(time(NULL));

    int L;
    // printf("How many layers? Maximum 16\n");
    // scanf("%d",&L);
    L=16;
    double** network = (double**)malloc(sizeof(double*) * L);

    int* N = (int*)malloc(sizeof(int)*L);
    for(int i=0;i<L;i++){
        // printf("How many nodes in layer %d? Maximum 256\n",i+1);
        // scanf("%d",&N[i]);
        N[i]=256;
        network[i] = layerCreation(N[i]);
    }
    
    double*** weight = (double***)malloc(sizeof(double**) * (L-1));
    for(int i=0;i<(L-1);i++){
        // printf("Create weight table for layer %d to %d\n",i+1,i+2);
        weight[i] = weightTableCreation(N[i],N[i+1]);
    }



    // printf("Enter %d input\n",N[0]);
    for(int i=0;i<N[0];i++){
        // scanf("%lf",&network[0][i]);
        network[0][i]=(double)rand()/RAND_MAX - 0.5;
    }

    clock_t start = clock();

    for(int i=0;i<(L-1);i++){
        matrixMultiplication(network[i],N[i],weight[i],network[i+1],N[i+1]);
        for(int j=0;j<N[i+1];j++)network[i+1][j] = sigmoid(network[i+1][j]);
    }

    clock_t end = clock();

    // printf("print output\n");
    // for(int i=0;i<N[L-1];i++)printf("%f ",network[L-1][i]);

    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Execution time: %lf seconds\n", duration);

    {
        for(int i=0;i<(L-1);i++){
            for(int j=0;j<N[i];j++)free(weight[i][j]);
            free(weight[i]);
        }
        free(weight);

        free(N);

        for(int i=0;i<L;i++)free(network[i]);
        free(network);
        return 0;
    }
}
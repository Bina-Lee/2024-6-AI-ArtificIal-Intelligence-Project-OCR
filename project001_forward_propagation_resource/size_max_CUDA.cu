#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cuda_runtime.h>

#define CUDA_CALL(x) {const cudaError_t a=(x); if(a!=cudaSuccess) {printf("\nCUDA Error: %s (err_num=%d)\n", cudaGetErrorString(a), a); exit(1);}}

__global__ void matrixMultiplyKernel(double *d_input, 
                                    double *d_weight, 
                                    double *d_output, 
                                    int inputSize, 
                                    int outputSize) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < outputSize) {
        double sum = 0.0;
        for (int j = 0; j < inputSize; j++) {
            sum += d_input[j] * d_weight[j * outputSize + row];
        }
        d_output[row] = sum;
    }
}

__global__ void sigmoidKernel(double *d_data, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < size) {
        d_data[idx] = 1.0 / (1.0 + exp(-d_data[idx]));
    }
}

double* layerCreation(int n) {
    double* layer = (double*)malloc(sizeof(double) * n);
    return layer;
}

double** weightTableCreation(int numberOfOutputNode, int numberOfInputNode) {
    double** weightTable = (double**)malloc(sizeof(double*) * numberOfOutputNode);
    for (int i = 0; i < numberOfOutputNode; i++) {
        weightTable[i] = (double*)malloc(sizeof(double) * numberOfInputNode);
        for (int j = 0; j < numberOfInputNode; j++) {
            weightTable[i][j] = (double)rand() / RAND_MAX - 0.5;
        }
    }
    return weightTable;
}

int main() {
    srand(time(NULL));
    int L = 16;
    double** network = (double**)malloc(sizeof(double*) * L);

    int* N = (int*)malloc(sizeof(int) * L);
    for (int i = 0; i < L; i++) {
        N[i] = 256;
        network[i] = layerCreation(N[i]);
    }

    double*** weight = (double***)malloc(sizeof(double**) * (L - 1));
    for (int i = 0; i < (L - 1); i++) {
        weight[i] = weightTableCreation(N[i], N[i + 1]);
    }

    for (int i = 0; i < N[0]; i++) {
        network[0][i] = (double)rand() / RAND_MAX - 0.5;
    }

    // CUDA 관련 변수 선언
    double *d_input, *d_output, **d_weightList;
    CUDA_CALL(cudaMalloc((void**)&d_input, sizeof(double) * N[0]));
    CUDA_CALL(cudaMalloc((void**)&d_output, sizeof(double) * N[1]));

    // 가중치 배열을 위한 포인터 배열 생성 및 데이터 복사
    d_weightList = (double**)malloc(sizeof(double*) * (L - 1));
    for (int layer = 0; layer < (L - 1); layer++) {
        int inputSize = N[layer];
        int outputSize = N[layer + 1];

        // 가중치 2차원 배열을 1차원 배열로 변환 후 디바이스로 복사
        double *weightFlat = (double*)malloc(sizeof(double) * inputSize * outputSize);
        for (int i = 0; i < inputSize; i++) {
            for (int j = 0; j < outputSize; j++) {
                weightFlat[i * outputSize + j] = weight[layer][j][i];
            }
        }
        CUDA_CALL(cudaMalloc((void**)&d_weightList[layer], sizeof(double) * inputSize * outputSize));
        CUDA_CALL(cudaMemcpy(d_weightList[layer], weightFlat, sizeof(double) * inputSize * outputSize, cudaMemcpyHostToDevice));
        free(weightFlat);
    }

    // 연산 시간 측정
    clock_t start = clock();

    for (int layer = 0; layer < (L - 1); layer++) {
        int inputSize = N[layer];
        int outputSize = N[layer + 1];

        // 입력 레이어를 디바이스로 복사
        CUDA_CALL(cudaMemcpy(d_input, network[layer], sizeof(double) * inputSize, cudaMemcpyHostToDevice));

        // 행렬 곱셈을 위한 CUDA 커널 호출
        int blockSize = 256;
        int gridSize = (outputSize + blockSize - 1) / blockSize;
        matrixMultiplyKernel<<<gridSize, blockSize>>>(d_input, d_weightList[layer], d_output, inputSize, outputSize);
        CUDA_CALL(cudaDeviceSynchronize());

        // 시그모이드 활성화 함수를 위한 CUDA 커널 호출
        sigmoidKernel<<<gridSize, blockSize>>>(d_output, outputSize);
        CUDA_CALL(cudaDeviceSynchronize());

        // 결과를 호스트로 복사
        CUDA_CALL(cudaMemcpy(network[layer + 1], d_output, sizeof(double) * outputSize, cudaMemcpyDeviceToHost));
    }

    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Execution time: %lf seconds\n", duration);

    // CUDA 메모리 해제
    for (int layer = 0; layer < (L - 1); layer++) {
        CUDA_CALL(cudaFree(d_weightList[layer]));
    }
    free(d_weightList);
    CUDA_CALL(cudaFree(d_input));
    CUDA_CALL(cudaFree(d_output));

    // 메모리 해제
    for (int i = 0; i < (L - 1); i++) {
        for (int j = 0; j < N[i]; j++) free(weight[i][j]);
        free(weight[i]);
    }
    free(weight);

    free(N);

    for (int i = 0; i < L; i++) free(network[i]);
    free(network);

    return 0;
}

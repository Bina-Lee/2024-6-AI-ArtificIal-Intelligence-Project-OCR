#include "import_weight_csv.h"
#include <stdio.h>
#include <stdlib.h>

void load_weights_from_csv(const char *filename, 
                            int *num_layers, 
                            int **nodes_per_layer, 
                            double ****weight) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open weight file");
        exit(1);
    }

    // 첫 번째 줄: 레이어 개수 읽기
    fscanf(file, "%d\n", num_layers);
    *nodes_per_layer = (int*)malloc(sizeof(int) * (*num_layers));
    
    // 두 번째 줄: 각 레이어의 노드 개수 읽기
    for (int i = 0; i < *num_layers; i++) {
        fscanf(file, "%d", &((*nodes_per_layer)[i]));
        if (i < (*num_layers - 1))fgetc(file); // ',' 읽기
    }
    fgetc(file); // '\n' 읽기

    // 가중치 배열 할당 (weights[layer][from_node][to_node])
    *weight = (double***)malloc(sizeof(double**) * (*num_layers-1));
    for (int i = 0; i < *num_layers - 1; i++) {
        int cols = (*nodes_per_layer)[i + 1];
        (*weight)[i] = (double**)malloc(sizeof(double *) * (*nodes_per_layer)[i]);
        for (int j = 0; j < (*nodes_per_layer)[i]; j++) {
            (*weight)[i][j] = (double*)malloc(sizeof(double) * (*nodes_per_layer)[i + 1]);
            for (int k = 0; k < (*nodes_per_layer)[i + 1]; k++) {
                fscanf(file, "%lf", &((*weight)[i][j][k]));
                if (k < (*nodes_per_layer)[i + 1] - 1) {
                    fgetc(file); // ',' 읽기
                }
            }
            fgetc(file); // '\n' 읽기
        }
    }
    fclose(file);
    printf("Weights loaded successfully from %s\n", filename);
    return;
}
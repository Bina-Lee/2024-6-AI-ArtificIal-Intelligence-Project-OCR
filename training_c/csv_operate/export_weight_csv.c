#include "export_weight_csv.h"
#include <stdio.h>
#include <stdlib.h>

void save_weights_to_csv(const char *filename, 
                        int num_layers, 
                        int *nodes_per_layer, 
                        double ***weights) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    // Layer 갯수 저장
    fprintf(file, "%d\n", num_layers);

    // 각 layer의 Node 수 저장
    for (int i = 0; i < num_layers; i++) {
        fprintf(file, "%d", nodes_per_layer[i]);
        if (i < num_layers - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    // 각 레이어 사이의 가중치 저장
    for (int i = 0; i < num_layers - 1; i++) {
        int rows = nodes_per_layer[i];
        int cols = nodes_per_layer[i + 1];
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                fprintf(file, "%lf", weights[i][r][c]);
                if (c < cols - 1) {
                    fprintf(file, ",");
                }
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
    printf("Weights saved successfully to %s\n", filename);
    return;
}
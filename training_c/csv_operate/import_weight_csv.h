#include <stdio.h>
#include <stdlib.h>

void load_weights_from_csv(const char *filename, 
                            int *num_layers, 
                            int **nodes_per_layer, 
                            double ****weight);
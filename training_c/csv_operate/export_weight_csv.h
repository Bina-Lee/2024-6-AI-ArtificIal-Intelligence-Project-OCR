#include <stdio.h>
#include <stdlib.h>

void save_weights_to_csv(const char *filename, 
                        int num_layers, 
                        int *nodes_per_layer, 
                        double ***weights);
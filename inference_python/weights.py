import numpy as np

def load_weights_from_csv(filepath):
    with open(filepath, "r") as f:
        num_layers = int(f.readline().strip())
        nodes_per_layer = list(map(int, f.readline().strip().split(",")))
        
        weights = []
        for l in range(num_layers - 1):
            rows = []
            for _ in range(nodes_per_layer[l]):
                row = list(map(float, f.readline().strip().split(",")))
                rows.append(row)
            weights.append(np.array(rows))
    return num_layers, nodes_per_layer, weights
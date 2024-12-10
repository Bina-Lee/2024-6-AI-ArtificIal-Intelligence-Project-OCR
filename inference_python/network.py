import numpy as np
from activation import sigmoid, relu

def matrix_multiplication(input_layer, weights):
    return np.dot(input_layer, weights)

def feedforward(input_layer, weights, nodes_per_layer, hidden_activation=relu, output_activation=sigmoid):
    num_layers = len(nodes_per_layer)
    activations = [input_layer.flatten()]
    for l in range(num_layers - 1):
        z = matrix_multiplication(activations[-1], weights[l])
        activation_func = hidden_activation if l < num_layers - 2 else output_activation
        activations.append(activation_func(z))
    return activations[-1]
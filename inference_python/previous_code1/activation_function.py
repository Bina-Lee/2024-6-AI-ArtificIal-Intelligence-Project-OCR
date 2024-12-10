import numpy as np

def sigmoid(x):
    return 1/(1+np.exp(-x))

def sigmoid_derivative(x):
    sig = sigmoid(x)
    return sig*(1-sig)

def tanh(x):
    return np.tanh(x)

def tanh_derivative(x):
    return 1-np.tanh(x)**2

def relu(x):
    return x if x>0 else 0

def relu_derivative(x):
    return 0 if x>0 else 1

def leaky_relu(x,alpha=0.1):
    return x if x>0 else x*alpha

def leaky_relu_derivative(x,alpha=0.1):
    return 0 if x>0 else alpha

def softmax(x):
    exp_x = np.exp(x - np.max(x))  # for numerical stability
    return exp_x / np.sum(exp_x, axis=0)
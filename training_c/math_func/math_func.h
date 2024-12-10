#include <math.h>

// 1. Sigmoid Activation Function
double sigmoid(double x);

// Derivative of Sigmoid
double sigmoid_derivative(double x);;

double tanh_clip(double x);

// Derivative of Tanh
double tanh_derivative(double x);

// 3. ReLU (Rectified Linear Unit) Activation Function
double relu(double x);

// Derivative of ReLU
double relu_derivative(double x);

// 4. Leaky ReLU Activation Function
double leaky_relu(double x, double alpha);

// Derivative of Leaky ReLU
double leaky_relu_derivative(double x, double alpha);
#include <math.h>

// 1. Sigmoid Activation Function
double sigmoid(double x) {
    if (x > 20) x = 20;
    if (x < -20) x = -20;
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of Sigmoid
double sigmoid_derivative(double x) {
    double sig = sigmoid(x);
    return sig * (1.0 - sig);
}

// // 2. Tanh Activation Function
// double tanh_activation(double x) {
//     return tanh(x);
// }

double tanh_clip(double x) {
    // Clipping input to prevent overflow
    if (x > 20) x = 20;   // Avoid overflow
    if (x < -20) x = -20; // Avoid underflow
    return tanh(x);
}

// Derivative of Tanh
double tanh_derivative(double x) {
    double tanh_x = tanh(x);
    return 1.0 - tanh_x * tanh_x;
}

// 3. ReLU (Rectified Linear Unit) Activation Function
double relu(double x) {
    return x > 0 ? x : 0;
}

// Derivative of ReLU
double relu_derivative(double x) {
    return x > 0 ? 1 : 0;
}

// 4. Leaky ReLU Activation Function
double leaky_relu(double x, double alpha) {
    return x > 0 ? x : alpha * x;
}

// Derivative of Leaky ReLU
double leaky_relu_derivative(double x, double alpha) {
    return x > 0 ? 1 : alpha;
}

// // 5. Softmax Activation Function
// // Softmax function for an array of values
// void softmax(double* input, double* output, int length) {
//     double max_input = input[0];
//     for (int i = 1; i < length; i++) {
//         if (input[i] > max_input) max_input = input[i];
//     }

//     double sum_exp = 0.0;
//     for (int i = 0; i < length; i++) {
//         output[i] = exp(input[i] - max_input);
//         sum_exp += output[i];
//     }

//     for (int i = 0; i < length; i++) {
//         output[i] /= sum_exp;
//     }
// }
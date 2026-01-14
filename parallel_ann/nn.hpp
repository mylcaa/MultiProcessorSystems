#ifndef NN_HPP
#define NN_HPP

#include <iostream>
#include <vector>
#include <math.h>
#include <random>
#include "matrix.hpp"

// Initialize a random number generator
extern random_device rd; // Seed

using namespace std;

class NeuralNetwork
{
    public:
        vector<int> _topology;
        vector<Matrix<float>> _neuronMatrix;
        vector<Matrix<float>> _unactivatedMatrix;
        vector<Matrix<float>> _biasMatrix;
        vector<Matrix<float>> _weightMatrix;
        float _learningRate;

    public:
        //initializing net with random values
        NeuralNetwork(vector<int> topology, float learningRate);
        //initializing net with trained values
        NeuralNetwork(vector<int> topology, float learningRate, const char *bias_file, const char *weight_file);
        // function to generate output from given input vector
        bool feedForward(Matrix<float> neuronMatrix);
        // function to train with given output vector
        bool backPropagate(vector<float> target);
        // function to retrive final output
        vector<float> getPredictions();
        // write down weights and biases of a trained neural network
        void print(const char *bias_file, const char *weight_file);

        void printToTerminal();
};

//Derivative ReLU 
inline float DReLU(float val){
    return ((val <= 0)? 0 : 1);
}

inline float HeInit(int inputNeuronNum) {
    return sqrt(6.0f/inputNeuronNum);
} 

inline float XavierInit(int outputLayer, int lastHiddenLayer) {
    return sqrt(6.0f/(lastHiddenLayer+outputLayer));
}

#endif

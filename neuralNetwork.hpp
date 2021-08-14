#ifndef NEURALNET_HPP
#define NEURALNET_HPP   

#include <vector>
#include <stdlib.h>
#include <math.h>

class layer
{
private:
    int num_inputs;
    int num_neurons;
    std::vector<std::vector<double>> weights;
    std::vector<double> biases;
    //double (*activationFunction)(double);
    //activation function? reLU 
public:
    layer(int num_inputs,int num_neurons);
    ~layer();
    std::vector<double> compute(std::vector<double> input);
    void mutate(double porcentageMutation,double mutateMax);
    void mix(layer otherlayer);
};

class neuralNetwork
{
private:
    std::vector<int> arch;
    std::vector<layer> layers;
public:
    neuralNetwork(std::vector<int> arch);
    ~neuralNetwork();
    std::vector<double> forward(std::vector<double> input);
    void mutate(double porcentageMutation,double mutateMax);
    void mix(neuralNetwork otherNetwork);
};

#endif
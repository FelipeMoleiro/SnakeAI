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

layer::layer(int num_inputs,int num_neurons)
{
    this->num_inputs = num_inputs;
    this->num_neurons = num_neurons;
    for(int i=0;i<num_neurons;i++){
        std::vector<double> neuron;
        for(int j=0;j<num_inputs;j++){
            neuron.push_back(2*(((double)rand())/RAND_MAX)-1);
        }
        weights.push_back(neuron);
        biases.push_back(2*(((double)rand())/RAND_MAX)-1);
    }
}

layer::~layer()
{
}

void layer::mutate(double porcentageMutation,double mutateMax)
{
    for(u_int i=0;i<weights.size();i++){
        for(u_int j=0;j<weights[i].size();j++){
            if( (double)rand()/RAND_MAX < porcentageMutation ){
                weights[i][j] += ( ((double)rand()/RAND_MAX)*2*mutateMax - mutateMax);
                if(weights[i][j] > 1){
                    weights[i][j] = 1;
                }else if(weights[i][j] < -1){
                    weights[i][j] = -1;
                }
            }
        }
    }
    for(uint i=0;i<biases.size();i++){
        if( (double)rand()/RAND_MAX < porcentageMutation ){
            biases[i] += ( ((double)rand()/RAND_MAX)*2*mutateMax - mutateMax);
        }
    }
}

void layer::mix(layer otherLayer){
    /*
    if(weights.size() != otherLayer.weights.size()) return;
    for(u_int i=0;i<weights.size();i++){
        if(weights[i].size() != otherLayer.weights[i].size()) return;
        for(u_int j=0;j<weights[i].size();j++){
            weights[i][j] = (weights[i][j] + otherLayer.weights[i][j]) / 2.0;      
        }
    }
    if(biases.size() != otherLayer.biases.size()) return;
    for(uint i=0;i<biases.size();i++){
        biases[i] = (biases[i] + otherLayer.biases[i]) / 2.0;
    }*/
    if(weights.size() != otherLayer.weights.size()) return;
    for(u_int i=0;i<weights.size();i++){
        if(weights[i].size() != otherLayer.weights[i].size()) return;
        for(u_int j=0;j<weights[i].size();j++){
            if(rand()%2){
                weights[i][j] = otherLayer.weights[i][j];
            } 
        }
    }
    if(biases.size() != otherLayer.biases.size()) return;
    for(uint i=0;i<biases.size();i++){
        if(rand()%2){
            biases[i] = otherLayer.biases[i];
        }
    }
       

}

double sigmoid(double x){
    double sigmoidValue = 1;
    sigmoidValue /= (1+exp(-x));
    return sigmoidValue;
}

std::vector<double> layer::compute(std::vector<double> input){
    std::vector<double> output;
    if((int)input.size() != this->num_inputs) return output;

    for(int i=0;i<num_neurons;i++){
        double neuron = 0;
        for(int j=0;j<num_inputs;j++){
            neuron += weights[i][j] * input[j];
        }
        neuron += biases[i];
        
        //activation layer ???
        neuron = sigmoid(neuron);

        output.push_back(neuron);
    }

    return output;
}

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

neuralNetwork::neuralNetwork(std::vector<int> arch)
{
    this->arch = arch;
    for(unsigned int i=1;i<arch.size();i++){
        this->layers.push_back(layer(arch[i-1],arch[i]));
    }
}

neuralNetwork::~neuralNetwork()
{
}

std::vector<double> neuralNetwork::forward(std::vector<double> input){
    std::vector<double> output;
    if((int)input.size() != arch[0]) return output;

    for(unsigned int i=0;i<layers.size();i++){
        input = layers[i].compute(input);
    }

    return input;
}

void neuralNetwork::mutate(double porcentageMutation,double mutateMax){
    
    for(unsigned int i=0;i<layers.size();i++){
        layers[i].mutate(porcentageMutation,mutateMax);
    }
}


void neuralNetwork::mix(neuralNetwork otherNetwork){
    if(arch != otherNetwork.arch) return;

    for(unsigned int i=0;i<layers.size();i++){
        layers[i].mix(otherNetwork.layers[i]);
    }
}
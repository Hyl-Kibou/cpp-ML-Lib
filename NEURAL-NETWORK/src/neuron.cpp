#include "neuron.hpp"
#include <random>

double gen_random_number(double min, double max){
    std::uniform_real_distribution<double> dist(min, max);  //(min, max)

    //Mersenne Twister: Good quality random number generator
    std::mt19937 rng; 
    //Initialize with non-deterministic seeds
    rng.seed(std::random_device{}()); 

    return dist(rng);
    
    //double random = (double)rand() / RAND_MAX;    
    //return min + random * (max - min);    
}

Neuron::Neuron(int prev_layer_size, int curr_layer_size){
    init_weights(prev_layer_size);
}

void Neuron::init_weights(int prev_layer_size){
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    for(int i = 0; i < prev_layer_size + 1; ++i){
        weights.push_back(gen_random_number(-1.0, 1.0));
    }
}   

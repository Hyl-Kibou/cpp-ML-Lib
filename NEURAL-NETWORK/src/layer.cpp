#include "layer.hpp"
#include <cstdio>

Layer::Layer(int prev_layer_size, int curr_layer_size) {
    
    for(int i = 0; i < curr_layer_size; ++i){
        neurons.push_back(new Neuron(prev_layer_size, curr_layer_size));
    }

    this->current_layer_size = curr_layer_size;

    //printf("prev: %d curr: %d\n", prev_layer_size, curr_layer_size);
}
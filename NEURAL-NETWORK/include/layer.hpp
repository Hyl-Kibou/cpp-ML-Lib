#ifndef __LAYER_H
#define __LAYER_H

#include "neuron.hpp"
#include <vector>

static int layer_id = 0;

class Layer
{
    public:
        int current_layer_size;
        std::vector<Neuron *> neurons;
        std::vector<double> layer_output;
        
        Layer(int, int);
        ~Layer();

        //std::vector<double> get_layer_output();
        //int get_size();
};

#endif
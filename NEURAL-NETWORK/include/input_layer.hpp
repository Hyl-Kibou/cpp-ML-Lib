#ifndef __INPUT_LAYER_H
#define __INPUT_LAYER_H

#include "layer.hpp"
#include "data.hpp"

class InputLayer : public Layer{
    public:
        InputLayer(int prev, int current) : Layer(prev, current){}
        void set_layer_outputs(Data *d);
};

#endif
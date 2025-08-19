#ifndef __OUTPUT_LAYER_H
#define __OUTPUT_LAYER_H

#include "layer.hpp"
#include "data.hpp"

class OutputLayer : public Layer{
    public:
        OutputLayer(int prev, int curr): Layer(prev, curr){}
    
        void feed_forward(Layer prev);
        void back_prop(Data * data);
        void update_weights(double, Layer *);

};

#endif
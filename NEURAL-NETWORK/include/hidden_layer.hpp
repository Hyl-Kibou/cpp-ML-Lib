#ifndef __HIDDEN_LAYER_H
#define __HIDDEN_LAYER_H

#include "layer.hpp"

class HiddenLayer : public Layer{
    public:
        HiddenLayer(int prev, int curr): Layer(prev, curr){}

        void feed_forward(Layer prev);
        void back_prop(Layer next);
        void update_weights(double, Layer *);
};

#endif
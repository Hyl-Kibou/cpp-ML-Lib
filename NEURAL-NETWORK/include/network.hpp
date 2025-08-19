#ifndef __NETWORK_H
#define __NETWORK_H

#include "data.hpp"
#include "neuron.hpp"
#include "layer.hpp"

//#include "input_layer.hpp"
//#include "hidden_layer.hpp"
//#include "output_layer.hpp"

#include "common.hpp"

class Network : public CommonData {

    public:
        std::vector<Layer *> layers;
        double learning_rate;
        double test_performance;        

        Network(std::vector<int> spec, int, int, double);
        ~Network();
        
        std::vector<double> frwd_prop(Data * data);
        double activate(std::vector<double>, std::vector<double>); //dot product
        double transfer(double); //sigmoid
        double transfer_derivative(double);
        
        void back_prop(Data * data);
        void update_weights(Data * data);
        int predict(Data * data); //return index of max value

        void train(int iterations);
        double test();
        void validate();

    /*
    private:
    InputLayer *InputLayer;
    OutputLayer *OutputLayer;
    std::vector<HiddenLayer *> hidden_layers;
    double eta;
    
    public:
    Network(std::vector<int> hidden_layer_spec, int, int);
    ~Network();
    
    void frwd_prop(Data * data);
    void back_prop(Data * data);
    
    void update_weights();
    void train();
    void test();
    void validate();
    */
    
};

#endif
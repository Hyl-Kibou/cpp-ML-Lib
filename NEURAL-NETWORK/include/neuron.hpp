#ifndef __NEURON_H
#define __NEURON_H

#include <cmath>
#include <vector>

class Neuron {
    public:
        double output;
        double delta;
        std::vector<double> weights;

        Neuron(int, int);
        ~Neuron();
        
        void init_weights(int prev_layer_size);
    /*
    std::vector<double> weights;
    double pre_activation;
    double activated_output;
    double output_derivative;
    double error;
    double alpha;
    
    public:
    Neuron(int, int);
    ~Neuron();
    
    void init_weights(int prev_layer_size, int curr_layer_size);
    void set_error(double);
    void set_weight(double, int);        
    void set_alpha(double);
    
    double calc_pre_activation(std::vector<double>);
    double activate();
    double calc_output_derivative();
    
    double sigmoid();
    double relu();
    double leaky_relu();
    double inverse_sqrt_relu();
    
    double get_output();
    double get_output_derivative();
    double get_error();
    std::vector<double> get_weights();
    */   
};

#endif
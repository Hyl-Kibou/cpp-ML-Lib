#include "network.hpp"
#include "layer.hpp"
#include "data_handler.hpp"

#include <numeric>

Network::Network(std::vector<int> spec, int input_size, int num_classes, double learning_rate) {
    
    for(int i = 0; i < spec.size(); ++i){
        if(i == 0){
            layers.push_back(new Layer(input_size, spec[i]));
        }
        else{
            layers.push_back(new Layer(spec[i-1], spec[i]));
        }
    }

    layers.push_back(new Layer(layers[layers.size()-1]->neurons.size(), num_classes));
    //layers.push_back(new Layer(spec[spec.size()-1], num_classes));

    this->learning_rate = learning_rate;
}

Network::~Network(){
    //
}

double Network::activate(std::vector<double> weights, std::vector<double> input) {
    double activation = weights.back(); //bias term
    for(int i = 0; i < weights.size() -1; ++i){
        activation += weights[i] * input[i];
    }
    return activation;
}

double Network::transfer(double activation) {
    return 1.0 / (1.0 + exp(-activation));
}

double Network::transfer_derivative(double output){
    return output * (1 - output);
}

std::vector<double> Network::frwd_prop(Data * data){
    std::vector<double> inputs = *data->get_normalized_feature_vector();
    for(int i = 0; i < layers.size(); ++i){
        Layer *layer = layers.at(i);
        std::vector<double> new_inputs;
        for(Neuron *n : layer->neurons){
            double activation = this->activate(n->weights, inputs);
            n->output = this->transfer(activation);
            new_inputs.push_back(n->output);
        }

        inputs = new_inputs;
    }
    return inputs;
}

void Network::back_prop(Data * data){
    for(int i = layers.size() -1; i>=0; --i){
        Layer *layer = layers[i];
        std::vector<double> errors;
        if(i != layers.size() -1){
            for(int j = 0; j< layer->neurons.size(); ++j){
                double error = 0.0;
                for(Neuron *n : layers[i+1]->neurons){
                    error += (n->weights[j] * n->delta);
                }
                errors.push_back(error);
            }
        }
        else{
            for(int j = 0; j< layer->neurons.size(); ++j){
                Neuron *n = layer->neurons[j];
                /*
                if(data->get_enumerated_label() == j){
                    errors.push_back((double)(data->get_enumerated_label() == j) - n->output);
                }
                else{
                    errors.push_back(0.0 - n->output);
                }
                */
                
                errors.push_back((double)(data->get_enumerated_label() == j) - n->output);
                //errors.push_back((double)data->getClassVector().at(j) - n->output);
            }
        }
        for(int j = 0; j<layer->neurons.size(); ++j){
            Neuron *n = layer->neurons[j];
            n->delta = errors[j] * this->transfer_derivative(n->output); // gradient / derivative
        }        
    }
}

void Network::update_weights(Data * data){
    std::vector<double> inputs = *data->get_normalized_feature_vector();
    for(int i = 0; i < layers.size(); ++i){
        if(i != 0){
            for(Neuron *n : layers[i-1]->neurons){
                inputs.push_back(n->output);
            }
        }
        for(Neuron * n : layers[i]->neurons){
            for(int j = 0; j<inputs.size(); ++j){
                n->weights[j] += this->learning_rate * n->delta * inputs[j];
            }
            n->weights.back() += this->learning_rate * n->delta;
        }
        inputs.clear();
    }
}

int Network::predict(Data * data){
    
    std::vector<double> outputs = frwd_prop(data);
    
    return std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));
}

void Network::train(int iterations){
    for(int i = 0; i<iterations; ++i){
        double sum_error = 0.0;
        for(Data *data : *this->train_data){
            std::vector<double> outputs = frwd_prop(data);
            double temp_error_sum = 0.0;
            for(int j = 0; j < outputs.size(); ++j){
                temp_error_sum += std::pow((double)(data->get_enumerated_label() == j) - outputs[j], 2);
            }
            sum_error += temp_error_sum;
            back_prop(data);
            update_weights(data);
        }
        printf("Iteration: %d \t Error=%.4f\n", i, sum_error);
    }
}

double Network::test(){
    double num_correct = 0.0;
    double count = 0.0;
    for(Data * data : *this->test_data){
        ++count;
        int index = predict(data);
        if(data->get_enumerated_label()==index){
            ++num_correct;
        }
    }

    double test_performance=(num_correct/count);
    return test_performance;
}

void Network::validate(){
    double num_correct = 0.0;
    double count = 0.0;
    for(Data * data : *this->validation_data){
        ++count;
        int index = predict(data);
        if(data->get_enumerated_label()==index){
            ++num_correct;
        }
    }

    double test_performance=(num_correct/count);
    printf("Validation performance: %.4f\n", test_performance);
}

int main(){
    DataHandler *dh = new DataHandler();

    dh->read_feature_labels("../data/train-labels-idx1-ubyte/train-labels.idx1-ubyte");
    dh->read_feature_vector("../data/train-images-idx3-ubyte/train-images.idx3-ubyte");
    
    //dh->read_csv("../data/iris_data_set/iris.csv", ",", -1); 

    
    
    dh->split_data();
    dh->count_classes();    
    dh->normalize_data();
        
    int num_neurons_in_layer = (dh->feature_vector_size*2/3+*dh->get_class_count());
    std::vector<int> hidden_layers = {num_neurons_in_layer};

    printf("%d-", dh->feature_vector_size);
    for(int x : hidden_layers){
        printf("%d-", x);
    }
    printf("%d\n", *dh->get_class_count());
    
    Network *net = new Network(
        hidden_layers, 
        dh->feature_vector_size, 
        *dh->get_class_count(), 
        0.25);        
        
    dh->set_dictionary(net->class_dictionary, net->label_dictionary);
    
    net->set_training_data(dh->get_training_data());
    net->set_test_data(dh->get_test_data());
    net->set_validation_data(dh->get_validation_data());    
    
    net->train(15);
    net->validate();
    printf("Test Performance: %.3f\n", net->test());
}
#include "common.hpp"

void common_data::set_training_data(std::vector<data *> *vect){
    train_data = vect;
}

void common_data::set_test_data(std::vector<data *> *vect){
    test_data = vect;
}

void common_data::set_validation_data(std::vector<data *> *vect){
    validation_data = vect;
}

void common_data::set_num_classes(int * val){
    num_classes = val;
}

double common_data::euclidean_distance_vtov(std::vector<double> * point_a, std::vector<double> * point_b){
    double distance = 0.0;

    if(point_a->size()!=point_b->size()){
        printf("Error!! Points size mismatch.");
        exit(1);
    }

    for(int i = 0; i<point_a->size(); ++i){
        distance+=std::pow(point_a->at(i) - point_b->at(i), 2);
    }
    distance = std::sqrt(distance);

    return distance;
}
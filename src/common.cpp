#include "common.hpp"

void CommonData::set_training_data(std::vector<Data *> *vect){
    train_data = vect;
}

void CommonData::set_test_data(std::vector<Data *> *vect){
    test_data = vect;
}

void CommonData::set_validation_data(std::vector<Data *> *vect){
    validation_data = vect;
}

void CommonData::set_num_classes(int * val){
    num_classes = val;
}

void CommonData::prep_data(DataHandler * dh){
    dh->split_data();
    dh->count_classes();    
    
    dh->set_dictionary(class_dictionary, label_dictionary);

    set_training_data(dh->get_training_data());
    set_test_data(dh->get_test_data());
    set_validation_data(dh->get_validation_data());
}

double CommonData::euclidean_distance_vtov(std::vector<double> * point_a, std::vector<double> * point_b){
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
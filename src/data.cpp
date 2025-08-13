#include "data.hpp"

data::data(){
    feature_vector = new std::vector<INPUT_VAR_TYPE>;
}

data::~data(){
    //
}

void data::set_feature_vector (std::vector<INPUT_VAR_TYPE> *vec){
    feature_vector = vec;
}
void data::append_to_feature_vector(INPUT_VAR_TYPE val){
    feature_vector->push_back(val);
}
void data::set_label(LABEL_VAR_TYPE val){
    label = val;
}
void data::set_enumerated_label(int val){
    enum_label = val;
}
void data::set_distance(double val){
    distance = val;
}

int data::get_feature_vector_size(){
    return feature_vector->size();
}
LABEL_VAR_TYPE data::get_label(){
    return label;
}
int data::get_enumerated_label(){
    return enum_label;
}

std::vector<INPUT_VAR_TYPE> * data::get_feature_vector(){
    return feature_vector;
}
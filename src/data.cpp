#include "data.hpp"

Data::Data(){
    feature_vector = new std::vector<INPUT_VAR_TYPE>;
}

Data::~Data(){
    //
}

void Data::set_feature_vector (std::vector<INPUT_VAR_TYPE> *vec){
    feature_vector = vec;
}
void Data::set_normalized_feature_vector (std::vector<double> *vec){
    normalized_feature_vector = vec;
}
void Data::append_to_feature_vector(INPUT_VAR_TYPE val){
    feature_vector->push_back(val);
}
void Data::set_label(LABEL_VAR_TYPE val){
    label = val;
}
void Data::set_class(CLASS_VAR_TYPE val){
    data_class = val;
}
void Data::set_enumerated_label(int val){
    enum_label = val;
}
void Data::set_distance(double val){
    distance = val;
}

int Data::get_feature_vector_size(){
    return feature_vector->size();
}
LABEL_VAR_TYPE Data::get_label(){    
    return label;
}
CLASS_VAR_TYPE Data::get_class(){
    return data_class;
}

int Data::get_enumerated_label(){
    return enum_label;
}

std::vector<INPUT_VAR_TYPE> * Data::get_feature_vector(){
    return feature_vector;
}

std::vector<double> * Data::get_normalized_feature_vector(){
    return normalized_feature_vector;
}
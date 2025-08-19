#ifndef __COMMON_H
#define __COMMON_H

#include "data.hpp"
#include "data_handler.hpp"
#include <vector>
#include <cstdlib>
#include <cmath>

class CommonData{
    protected:
        std::vector<Data *> * train_data;
        std::vector<Data *> * test_data;
        std::vector<Data *> * validation_data;
        
        int * num_classes;
        
    public:
        std::vector<CLASS_VAR_TYPE> * class_dictionary;
        std::vector<LABEL_VAR_TYPE> * label_dictionary;
        
        void set_training_data(std::vector<Data *> *vect);
        void set_test_data(std::vector<Data *> *vect);
        void set_validation_data(std::vector<Data *> *vect);
        
        void set_num_classes(int * val);

        void prep_data(DataHandler *dh);

        double euclidean_distance_vtov(std::vector<double> * point_a, std::vector<double> * point_b);
};

#endif
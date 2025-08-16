#ifndef __COMMON_H
#define __COMMON_H

#include "data.hpp"
#include <vector>
#include <cstdlib>
#include <cmath>

class common_data{
    protected:
        std::vector<data *> * train_data;
        std::vector<data *> * test_data;
        std::vector<data *> * validation_data;
        int * num_classes;

    public:
        void set_training_data(std::vector<data *> *vect);
        void set_test_data(std::vector<data *> *vect);
        void set_validation_data(std::vector<data *> *vect);
        
        void set_num_classes(int * val);

        double euclidean_distance_vtov(std::vector<double> * point_a, std::vector<double> * point_b);
};

#endif
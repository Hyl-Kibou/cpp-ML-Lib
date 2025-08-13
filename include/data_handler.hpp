#ifndef __DATA_HANLDER_H
#define __DATA_HANDLER_H

#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <random>
#include <vector>
#include <unordered_set>

#include "stdint.h"
#include "data.hpp"

class data_handler
{
    std::vector<data *> *data_array;
    std::vector<data *> *train_data;
    std::vector<data *> *test_data;
    std::vector<data *> *validation_data;

    int num_classes;
    int feature_vector_size;

    std::map<LABEL_VAR_TYPE, int> class_map;

    const double TRAIN_SET_PERCENT = 0.75;
    const double TEST_SET_PERCENT = 0.20;
    const double VALIDATION_PERCENT = 0.05;

    public:
        data_handler();
        ~data_handler();
        
        void scale_data_array(int size);

        void read_feature_vector(std::string path);
        void read_feature_labels(std::string path);

        void stuff_data_in_array(int &counter, int &size, std::vector<data *> *&array, std::vector<int32_t> &order);
        void split_data();
        void count_classes();

        uint32_t convert_to_little_endian(const unsigned char* bytes);

        std::vector<data *> * get_training_data();
        std::vector<data *> * get_test_data();
        std::vector<data *> * get_validation_data();

};

#endif
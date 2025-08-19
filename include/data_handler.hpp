#ifndef __DATA_HANDLER_H
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

class DataHandler
{
    std::vector<Data *> *data_array;
    std::vector<Data *> *train_data;
    std::vector<Data *> *test_data;
    std::vector<Data *> *validation_data;

    int * num_classes;        
    
    std::map<LABEL_VAR_TYPE, int> label_map; //has enumerated
    std::map<CLASS_VAR_TYPE, int> class_map; //has enumerated    
    
    const double TRAIN_SET_PERCENT = 0.75;
    const double TEST_SET_PERCENT = 0.20;
    const double VALIDATION_PERCENT = 0.05;
    
    public:
        DataHandler();
        ~DataHandler();
        int feature_vector_size;
        
        void scale_data_array(int size);

        void read_csv(std::string path, std::string delimeter, int class_position);

        void read_feature_vector(std::string path);
        void read_feature_labels(std::string path);

        void stuff_data_in_array(int &counter, int &size, std::vector<Data *> *&array, std::vector<int32_t> &order);
        void split_data();
        void count_classes();

        void normalize_data();

        uint32_t convert_to_little_endian(const unsigned char* bytes);

        std::vector<Data *> * get_training_data();
        std::vector<Data *> * get_test_data();
        std::vector<Data *> * get_validation_data();

        int * get_class_count();    
        
        void set_dictionary(std::vector<CLASS_VAR_TYPE> * class_dictionary, std::vector<LABEL_VAR_TYPE> * label_dictionary);

};

#endif
#ifndef __DATA_H
#define __DATA_H

#include "config.hpp"

#include <vector>
#include "stdint.h"
#include "stdio.h"

class data
{
    std::vector<INPUT_VAR_TYPE> * feature_vector;
    LABEL_VAR_TYPE label;
    int enum_label;
    double distance;

    public:
        //let others know which functions i have
        data();
        ~data();

        void set_feature_vector (std::vector<INPUT_VAR_TYPE> *);
        void append_to_feature_vector(INPUT_VAR_TYPE);
        void set_label(LABEL_VAR_TYPE);
        void set_enumerated_label(int);
        void set_distance(double);

        int get_feature_vector_size();
        LABEL_VAR_TYPE get_label();
        int get_enumerated_label();

        std::vector<INPUT_VAR_TYPE> * get_feature_vector();

};

#endif
#ifndef __KNN_H
#define __KNN_H

#include <vector>
#include "common.hpp"

class knn : public CommonData
{
    int k;
    std::vector<Data *> * neighbors;

    public:
        knn(int);
        knn();
        ~knn();

        void find_knearest(Data * query_point);        

        void set_k(int val);
        
        LABEL_VAR_TYPE predict();
        double calculate_distance(Data* query_point, Data* input);
        double validate_performance();
        double test_performance();
};

#endif
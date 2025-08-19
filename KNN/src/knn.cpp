#include "knn.hpp"
#include "data_handler.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include "stdint.h"
#include <vector>
#include <set>


knn::knn(int val){
    k=val;
}
knn::knn(){
    //
}
knn::~knn(){
    //
}


void knn::find_knearest(Data * query_point){
    neighbors = new std::vector<Data *>;    

    if(k >= train_data->size()){
        printf("There are less than k neighbors, only: %d.\n", train_data->size()-1);
        exit(1);
    }
       
    std::set<std::pair<double, int>> nearest_neighbors;
       
    for(int j = 0; j<train_data->size(); ++j){
        double distance = calculate_distance(query_point, train_data->at(j));
        
        if(nearest_neighbors.size()<k){
            nearest_neighbors.insert({distance, j});
        }
        else{
            auto last = nearest_neighbors.end();
            --last;
            if(distance<last->first){
                nearest_neighbors.erase(last);
                nearest_neighbors.insert({distance, j});                
            }

        }

        /*
        if(nearest_neighbors.size()>k){
            auto it =  nearest_neighbors.end();
            --it;
            
            nearest_neighbors.erase(it);
        }
        */
    }

    for(std::pair<double, int> x: nearest_neighbors){
       neighbors->push_back(train_data->at(x.second));
    }
}

void knn::set_k(int val){
    k = val;
}

LABEL_VAR_TYPE knn::predict(){
    std::map<LABEL_VAR_TYPE, int> class_freq;
    LABEL_VAR_TYPE most_common_class;    

    for(Data * x : *neighbors){
        if(class_freq.find(x->get_label())==class_freq.end()){
            class_freq[x->get_label()]=0;
            most_common_class = x->get_label();
        }
        ++class_freq[x->get_label()];
    }

    for(std::pair<LABEL_VAR_TYPE, int> x : class_freq){
        if(x.second>class_freq[most_common_class]){
            most_common_class = x.first;
        }
    }
    delete neighbors;
    return most_common_class;
}

double knn::calculate_distance(Data* query_point, Data* input){
    double distance = 0.0;

    if(query_point->get_feature_vector_size()!= input->get_feature_vector_size()){
        printf("Error Data Vector Size Mismatch\n");
        exit(1);
    }

    #ifdef EUCLID
        for(unsigned i = 0; i<query_point->get_feature_vector_size(); ++i){
            distance += std::pow(query_point->get_feature_vector()->at(i) - input->get_feature_vector()->at(i), 2);
        }
        distance = std::sqrt(distance);
    #elif defined MANHATTAN
        //calculate Manhattan distance
    #endif
    return distance;
}

double knn::validate_performance(){
    double current_performance = 0;
    int correct_hits  = 0;
    int data_index = 0;

    for(Data * query_point : *validation_data){
        find_knearest(query_point);
        LABEL_VAR_TYPE prediction = predict();
        printf("Current: %d | Guess: %d.", query_point->get_label(), prediction);
        if(prediction == query_point->get_label()){
            ++correct_hits;
        }
        else{
            printf(" Missed!");
        }
        ++data_index;        
        printf("\nCurrent Performance #%4d = %.3f %%\n", data_index, ((double)correct_hits*100.0)/((double)data_index));
    }
    current_performance = ((double)correct_hits*100.0)/((double)validation_data->size());
    printf("Validation Performance for K: %d = %.3f %%\n", k, current_performance);
    return current_performance;
}
double knn::test_performance(){
    double current_performance = 0;
    int correct_hits  = 0;
    
    for(Data * query_point : *test_data){
        find_knearest(query_point);
        LABEL_VAR_TYPE prediction = predict();
        if(prediction == query_point->get_label()){
            ++correct_hits;
        }
    }
    current_performance = ((double)correct_hits*100.0)/((double)test_data->size());
    printf("Testing Performance = %.3f %%\n", current_performance);
    return current_performance;
}

int main(){
    
    DataHandler *dh = new DataHandler();
    
    dh->read_feature_labels("../data/train-labels-idx1-ubyte/train-labels.idx1-ubyte");
    dh->read_feature_vector("../data/train-images-idx3-ubyte/train-images.idx3-ubyte");
    
    //dh->read_csv("../data/iris_data_set/iris.csv", ",", -1);

    //dh->split_data();
    //dh->count_classes();    

    knn *knearest = new knn();
    //knearest->set_training_data(dh->get_training_data());
    //knearest->set_test_data(dh->get_test_data());
    //knearest->set_validation_data(dh->get_validation_data());

    knearest->prep_data(dh);

    double performance = 0;
    double best_performance = -1;
    int best_k = 1;

    for(int i = 1; i<=4; ++i){
        knearest->set_k(i);
        performance = knearest->validate_performance();
        if(performance>best_performance){
            best_performance = performance;
            best_k = i;
        }
        else{
            //break;
        }
    }
    knearest->set_k(best_k);
    printf("Best performant k is: %d.\n", best_k);
    
    knearest->test_performance();

}
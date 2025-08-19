#ifndef __KMEANS_H
#define __KMEANS_H

#include "common.hpp"
#include "data_handler.hpp"

#include <unordered_set>
#include <limits>
#include <cstdlib>
#include <cmath>
#include <unordered_map>
#include <random>

typedef struct cluster{

    std::vector<double> *centroid;
    std::vector<double> *next_centroid;
    std::vector<Data *> *cluster_points;
    int num_cluster_points;
    std::unordered_map<LABEL_VAR_TYPE, int> class_counts;
    LABEL_VAR_TYPE most_frequent_class;    

    cluster(Data * initial_point){
        centroid = new std::vector<double>;
        next_centroid = new std::vector<double>;
        num_cluster_points = 1;
        //cluster_points = new std::vector<Data *>;
        for(auto value: *(initial_point->get_feature_vector())){
            centroid->push_back(value);
            next_centroid->push_back(value);
        }
        //cluster_points->push_back(initial_point);
        class_counts[initial_point->get_label()] = 1;
        most_frequent_class = initial_point->get_label();
    }

    void add_to_cluster_and_update(Data *point){
        ++num_cluster_points;
        for(int i = 0; i<centroid->size(); ++i){
            double value=centroid->at(i);
            value *= num_cluster_points-1;
            value += point->get_feature_vector()->at(i);
            value /= (double)num_cluster_points;

            centroid->at(i) = value;
            next_centroid->at(i) =value;
        }
        if(class_counts.find(point->get_label())==class_counts.end()){
            class_counts[point->get_label()]=0;
        }
        ++class_counts[point->get_label()];

        if(class_counts[point->get_label()]>class_counts[most_frequent_class]){
            most_frequent_class = point->get_label();
        }
    }

    void add_to_cluster(Data *point){
        ++num_cluster_points;
        for(int i = 0; i<next_centroid->size(); ++i){
            double value=next_centroid->at(i);
            value *= num_cluster_points-1;
            value += point->get_feature_vector()->at(i);
            value /= (double)num_cluster_points;

            next_centroid->at(i) = value;
        }
        if(class_counts.find(point->get_label())==class_counts.end()){
            class_counts[point->get_label()]=0;
        }
        ++class_counts[point->get_label()];

        if(class_counts[point->get_label()]>class_counts[most_frequent_class]){
            most_frequent_class = point->get_label();
        }
    }

    bool reposition_centroid(){
        bool ans;
        if(*centroid==*next_centroid){            
            ans = 0;
        }
        else{
            ans = 1;
        }

        delete centroid;
        centroid = next_centroid;
        next_centroid = new std::vector<double> (centroid->size(), 0);

        num_cluster_points = 0;
        class_counts.clear();            
        class_counts[most_frequent_class]=0;
        
        return ans;
    }


} cluster_t;

class kmeans : public CommonData{
    int num_clusters;
    std::vector<cluster_t *> *clusters;
    std::vector<int> *used_indexes;

    public:
        kmeans(int k);
        void init_clusters();
        void init_clusters_for_each_class();

        void clean_clusters();

        void train(int, double);
        void train_one_round();
        double euclidean_distance(std::vector<double> *, Data *);
        double validate();
        double test();

};

#endif
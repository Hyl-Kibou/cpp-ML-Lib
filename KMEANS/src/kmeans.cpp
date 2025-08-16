#include "kmeans.hpp"

kmeans::kmeans(int k){
    num_clusters = k;
    clusters = new std::vector<cluster_t *>;
    used_indexes = new std::vector<int>;
}

void kmeans::init_clusters(){
    /*
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, train_data->size()-1); // define the range    
    int index = distr(gen);     
    */
   
   if(num_clusters>train_data->size()){
       printf("Error!! num_clusters: %d; is bigger than the size of the train data: %d.\n", num_clusters, train_data->size());
       exit(1);
    }    
    
    clean_clusters();
    
    for(int i = 0; i<num_clusters; ++i){
        clusters->push_back(new cluster_t(train_data->at(i)));
        used_indexes->at(i)=1;
    }
}

void kmeans::init_clusters_for_each_class(){
    
    clean_clusters();
    
    std::vector<int> bucket (*num_classes, 0);
    
    for(int i = 0; i<train_data->size(); ++i){
        if(bucket[train_data->at(i)->get_enumerated_label()]==0){
            clusters->push_back(new cluster_t(train_data->at(i)));
            bucket[train_data->at(i)->get_enumerated_label()]=1;
            used_indexes->at(i)=1;
        }
    }
    
    printf("num_clusters changed from: %d to %d.\n", num_clusters, clusters->size());
    
    num_clusters=clusters->size();
    
    if(num_clusters!=*num_classes){
        printf("Warning!! The number of clusters: %d and the number of classes in the data: %d are different.\n", num_clusters, num_classes);
    }
    
}

void kmeans::clean_clusters(){
    
    used_indexes->clear();
    
    for(int i = 0; i<train_data->size(); ++i){
        used_indexes->push_back(0);
    }        
}

void kmeans::train(int iterations = -1, double precision_tolerance = 0.0){ 
    
    while(iterations){
        
        for(int i = 0; i<train_data->size(); ++i){
            if(used_indexes->at(i) == 0){
                double min_dist = std::numeric_limits<double>::max();
                int best_cluster = 0;
                
                for(int j = 0; j<clusters->size(); ++j){
                    double current_dist = euclidean_distance(clusters->at(j)->centroid, train_data->at(i));
                    if(current_dist<min_dist){
                        best_cluster = j;
                        min_dist=current_dist;
                    }
                }
                clusters->at(best_cluster)->add_to_cluster(train_data->at(i));
                used_indexes->at(i) = 1;
            }        
        }                
        
        clean_clusters();
        
        int ans = 0;
        int total_points_used = 0;
        double total_distance = 0.0;
        
        for(auto x : *clusters){
            double dist =euclidean_distance_vtov(x->centroid, x->next_centroid);
            total_distance+=dist;
            ans+=x->reposition_centroid();
        }
        
        printf("Clusters changed: %d, cluster points total: %d, total distance moved: %.2f\n\n", ans, total_points_used, total_distance);
        
        if(ans==0){
            break;
        }
        else{
            if(iterations<-1000){
                printf("Warning!! Exited after %d iterations, didn't reach convergence.\n", -iterations);
                break;
            }
            if(total_distance<=precision_tolerance){            
                break;
            }
        }

        --iterations;
    }
}

void kmeans::train_one_round(){     
        
    for(int i = 0; i<train_data->size(); ++i){
        if(used_indexes->at(i) == 0){
            double min_dist = std::numeric_limits<double>::max();
            int best_cluster = 0;
            
            for(int j = 0; j<clusters->size(); ++j){
                double current_dist = euclidean_distance(clusters->at(j)->centroid, train_data->at(i));
                if(current_dist<min_dist){
                    best_cluster = j;
                    min_dist=current_dist;
                }
            }
            clusters->at(best_cluster)->add_to_cluster_and_update(train_data->at(i));
            used_indexes->at(i) = 1;
        }        
    }                               
}

double kmeans::euclidean_distance(std::vector<double> * centroid, data * point){
    double distance = 0.0;

    if(centroid->size()!=point->get_feature_vector_size()){
        printf("Error!! Centroid size and feature vector size mismatch.");
        exit(1);
    }

    for(int i = 0; i<centroid->size(); ++i){
        distance+=std::pow(centroid->at(i) - point->get_feature_vector()->at(i), 2);
    }
    distance = std::sqrt(distance);

    return distance;
}

double kmeans::validate(){
    
    double num_correct = 0.0;
    
    for(data * query_point : *validation_data){
        double min_dist = std::numeric_limits<double>::max();
        int best_cluster = 0;
        
        for(int j = 0; j<clusters->size(); ++j){
            double current_dist = euclidean_distance(clusters->at(j)->centroid, query_point);
            if(current_dist<min_dist){
                best_cluster = j;
                min_dist=current_dist;
            }
        }    
        
        if(clusters->at(best_cluster)->most_frequent_class == query_point->get_label()){
            ++num_correct;
        }
        
    }

    return 100.0 *(num_correct/ (double)validation_data->size());
}

double kmeans::test(){
    double num_correct = 0.0;

    for(data * query_point : *test_data){
        double min_dist = std::numeric_limits<double>::max();
        int best_cluster = 0;

        for(int j = 0; j<clusters->size(); ++j){
            double current_dist = euclidean_distance(clusters->at(j)->centroid, query_point);
            if(current_dist<min_dist){
                best_cluster = j;
                min_dist=current_dist;
            }
        }    
        
        if(clusters->at(best_cluster)->most_frequent_class == query_point->get_label()){
            ++num_correct;
        }
        
    }

    return 100.0 *(num_correct/ (double)test_data->size());
}

int main(){
    data_handler *dh = new data_handler();

    dh->read_feature_labels("../data/train-labels-idx1-ubyte/train-labels.idx1-ubyte");
    dh->read_feature_vector("../data/train-images-idx3-ubyte/train-images.idx3-ubyte");

    dh->split_data();
    dh->count_classes();

    double performance = 0.0;
    double best_performance = 0.0;  
    int best_k = *(dh->get_class_count());   

    for(int k = *(dh->get_class_count())+10; k < dh->get_training_data()->size()/10; ++k ){
        kmeans *km = new kmeans(k);
        km->set_training_data(dh->get_training_data());
        km->set_test_data(dh->get_test_data());
        km->set_validation_data(dh->get_validation_data());
        km->set_num_classes(dh->get_class_count());

        km->init_clusters();
        //km->init_clusters_for_each_class();
        km->train(200, 5);
        //km->train_one_round();
        performance = km->validate();
        printf("Current Performance with (K=%d): %.2f%\n", k, performance);
        if(performance > best_performance){
            best_performance = performance;
            best_k = k;
        }
    }

    kmeans *km = new kmeans(best_k);
    km->set_training_data(dh->get_training_data());
    km->set_test_data(dh->get_test_data());
    km->set_validation_data(dh->get_validation_data());
    km->set_num_classes(dh->get_class_count());

    km->init_clusters();
    km->train();
    performance = km->test();
    printf("Test Performance with (K=%d): %.2f%.\n", best_k, performance);    
}
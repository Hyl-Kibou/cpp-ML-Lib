#include "data_handler.hpp"

DataHandler::DataHandler(){
    data_array = new std::vector<Data *>;
    test_data = new std::vector<Data *>;
    train_data = new std::vector<Data *>;
    validation_data = new std::vector<Data *>;
    num_classes = new int;
    *num_classes = 0;
}
DataHandler::~DataHandler(){
    //erase the things from DataHandler
}

void DataHandler::scale_data_array(int size){
    for(int i = 0; i<size; ++i){ //Iterate on all elements
        Data *d = new Data();
        data_array->push_back(d);
    }
}

void DataHandler::read_csv(std::string path, std::string delimeter, int class_position){

    #if CLASS_VAR_TYPE_IS_STRING == 0
        printf("Error!! Read csv can only be used with CLASS_VAR_TYPE == std::string.\n");         
        exit(1);
    #else
    
    *num_classes = 0;
    std::ifstream data_file(path.c_str());
    std::string line;
    std::getline(data_file, line);
    while(std::getline(data_file, line)){
        if(line.length()==0){
            continue;
        }
        
        Data *d = new Data();
        size_t position = 0;
        std::string token;
        int round = 0;
        
        while((position = line.find(delimeter)) != std::string::npos){
            if(round == class_position){
                //This is the class
                if(class_map.find(line) == class_map.end()){                                    
                    class_map[line]=*num_classes;
                    ++(*num_classes);
                }
                d->set_label(class_map[line]);
            }else{
                token = line.substr(0, position);
                //printf("Line %s\n", token.c_str());
                
                d->append_to_feature_vector(static_cast<INPUT_VAR_TYPE>(std::stod(token)));
                line.erase(0, position + delimeter.size());
            }
            ++round;
        }
        if(round == class_position || class_position==-1){            
            //This is the class
            if(class_map.find(line) == class_map.end()){            
                class_map[line]=*num_classes;                
                ++(*num_classes);
            }                        
            d->set_label(class_map[line]);
            d->set_class(line);
        }
        else{                
            d->append_to_feature_vector(static_cast<INPUT_VAR_TYPE>(std::stod(line)));
        }
        
        data_array->push_back(d);
        
    }
    feature_vector_size = data_array->at(0)->get_feature_vector()->size();    
    
    printf("Data set read, %d points loaded correctly.\n", data_array->size());
    
    #endif
}


void DataHandler::read_feature_vector(std::string path){
    uint32_t header[4]; //MAGIC, NUM IMAGES, ROWSIZE, COLSIZE
    
    unsigned char bytes[4];
    FILE *f = fopen(path.c_str(), "r");
    
    if(f){
        for(int i = 0; i<4; ++i){
            if(fread(bytes, sizeof(bytes), 1, f)){
                header[i] = convert_to_little_endian(bytes);
            }
        }
        
        if(data_array->empty()){
            scale_data_array(header[1]);            
        }
        
        printf("FV File header read.\n");
        
        int image_size = header[2]*header[3];
        for(int i = 0; i<header[1]; ++i){ //Iterate on all elements            
            INPUT_VAR_TYPE element[1];
            
            for(int j = 0; j<image_size; ++j){ //Read all the Data from element i
                if(fread(element, sizeof(element), 1, f)){
                    data_array->at(i)->append_to_feature_vector(element[0]);                    
                }
                else{
                    printf("Error reading from File.\n");
                    exit(1);
                }
            }
            
        }
        feature_vector_size = data_array->at(0)->get_feature_vector()->size();    
        
        printf("All %lld Feature vectors read and loaded.\n", data_array->size());
    }
    else{
        printf("Data File not found.\n");
        exit(1);
    }
    
}

void DataHandler::read_feature_labels(std::string path){
    uint32_t header[2]; //MAGIC, NUM IMAGES

    unsigned char bytes[4];

    FILE *f = fopen(path.c_str(), "r");

    if(f){
        for(int i = 0; i<2; ++i){
            if(fread(bytes, sizeof(bytes), 1, f)){
                header[i] = convert_to_little_endian(bytes);
            }
        }
        
        printf("Label File header read.\n");

        if(data_array->empty()){
            scale_data_array(header[1]);            
        }
        
        for(int i = 0; i<header[1]; ++i){ //Iterate on all elements
            CLASS_VAR_TYPE element[1];
            
            if(fread(element, sizeof(element), 1, f)){
                #if CLASS_VAR_TYPE_IS_STRING == 0
                    data_array->at(i)->set_label(static_cast<CLASS_VAR_TYPE>(element[0]));
                #else
                    if(class_map.find(element[0]) == class_map.end()){                                    
                        class_map[element[0]]=*num_classes;
                        ++(*num_classes);
                    }
                    data_array->at(i)->set_label(class_map[element[0]]);
                    data_array->at(i)->set_class(element[0]);
                #endif
            }
            else{
                printf("Error reading from File.\n");
                exit(1);
            }            
        }
        printf("All %lld labels read and loaded.\n", data_array->size());
    }
    else{
        printf("Data File not found.\n");
        exit(1);
    }
}

void DataHandler::stuff_data_in_array(int &counter, int &size, std::vector<Data *> * &array, std::vector<int32_t> &order){

    int local_counter = 0;
    while(local_counter<size){        
        array->push_back(data_array->at(order[counter]));
        ++counter;
        ++local_counter;
    }

}

void DataHandler::split_data(){
    //std::unordered_set<int32_t> used_indexes;
    
    int train_size = data_array->size() * TRAIN_SET_PERCENT;
    int test_size = data_array->size() * TEST_SET_PERCENT;
    int valid_size = data_array->size() -train_size-test_size;    
    
    std::vector<int32_t> shuffled_order (data_array->size());
    
    for(int i = 0; i<shuffled_order.size(); ++i){
        shuffled_order[i]=i;
    }
    
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle(shuffled_order.begin(), shuffled_order.end(), g);
    
    int counter = 0;  
    
    stuff_data_in_array(counter, train_size, train_data, shuffled_order);
    stuff_data_in_array(counter, test_size, test_data, shuffled_order);
    stuff_data_in_array(counter, valid_size, validation_data, shuffled_order);
    
    printf("Training Data Size: %lu.\n", train_data->size());
    printf("Testing Data Size: %lu.\n", test_data->size());
    printf("Validation Data Size: %lu.\n", validation_data->size());
    
    
}
void DataHandler::count_classes(){
    if(*num_classes == 0){
        int count = 0;
        for(Data * x : *data_array){
            if(label_map.find(x->get_label())==label_map.end()){
                label_map[x->get_label()]=count;
                ++count;
            }
            x->set_enumerated_label(label_map[x->get_label()]);
        }
        
        *num_classes = count;
    }
    else{
        for(Data * x : *data_array){
            if(label_map.find(x->get_label())==label_map.end()){
                label_map[x->get_label()]=x->get_label();
            }
            x->set_enumerated_label(label_map[x->get_label()]);
        }        
    }    
    
    printf("Counted number of classes: %d.\n", *num_classes);
}

void DataHandler::normalize_data(){
    std::vector<INPUT_VAR_TYPE> mins, maxs;


    for(int i = 0; i < data_array->size(); ++i){
        Data *d = data_array->at(i);
        int j = 0;
        for(INPUT_VAR_TYPE val : *d->get_feature_vector()){
            if(i==0){
                mins.push_back(val);
                maxs.push_back(val);
            }
            else{
                if(mins[j]>val){
                    mins[j] = val;
                }
                if(maxs[j]<val){
                    maxs[j] = val;
                }
            }
            ++j;
        }
    }
    
    
    for(int i = 0; i < data_array->size(); ++i){
        Data *d = data_array->at(i);
        d->set_normalized_feature_vector(new std::vector<double>);

        for(int j = 0; j < d->get_feature_vector_size(); ++j){
            if(maxs[j] == mins[j]){
                d->get_normalized_feature_vector()->push_back(0.0);
            }
            else{
                d->get_normalized_feature_vector()->push_back((double)(d->get_feature_vector()->at(j) - mins[j])/(double)(maxs[j]-mins[j]));
            }
        }
    }

}

uint32_t DataHandler::convert_to_little_endian(const unsigned char* bytes){
    return (uint32_t) ((bytes[0] << 24) | 
    (bytes[1] << 16) | 
    (bytes[2] << 8) |
    (bytes[3]));
}

std::vector<Data *> * DataHandler::get_training_data(){
    return train_data;
}
std::vector<Data *> * DataHandler::get_test_data(){
    return test_data;
}
std::vector<Data *> * DataHandler::get_validation_data(){
    return validation_data;
}

int * DataHandler::get_class_count(){
    return num_classes;
}

void DataHandler::set_dictionary(std::vector<CLASS_VAR_TYPE> * class_dictionary, std::vector<LABEL_VAR_TYPE> * label_dictionary){
    
    class_dictionary = new std::vector<CLASS_VAR_TYPE>;
    label_dictionary = new std::vector<LABEL_VAR_TYPE>;

    class_dictionary->resize(*num_classes);
    label_dictionary->resize(*num_classes);

    for(Data * x : *data_array){
        class_dictionary->at(x->get_enumerated_label()) = x->get_class();
        label_dictionary->at(x->get_enumerated_label()) = x->get_label();
    }
}

int main(){

    DataHandler *dh = new DataHandler();

    //dh->read_feature_vector("./data/train-images-idx3-ubyte/train-images.idx3-ubyte");
    //dh->read_feature_labels("./data/train-labels-idx1-ubyte/train-labels.idx1-ubyte");

    printf("cats\n");

    dh->read_csv("./data/iris_data_set/iris.csv", ",", -1);

    dh->split_data();
    dh->count_classes();

    return  0;
}
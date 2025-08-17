#include "data_handler.hpp"

data_handler::data_handler(){
    data_array = new std::vector<data *>;
    test_data = new std::vector<data *>;
    train_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
    num_classes = new int;
    *num_classes = 0;
}
data_handler::~data_handler(){
    //erase the things from data_handler
}

void data_handler::scale_data_array(int size){
    for(int i = 0; i<size; ++i){ //Iterate on all elements
        data *d = new data();
        data_array->push_back(d);
    }
}

void data_handler::read_csv(std::string path, std::string delimeter, int class_position){

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
        
        data *d = new data();
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


void data_handler::read_feature_vector(std::string path){
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
            
            for(int j = 0; j<image_size; ++j){ //Read all the data from element i
                if(fread(element, sizeof(element), 1, f)){
                    data_array->at(i)->append_to_feature_vector(element[0]);                    
                }
                else{
                    printf("Error reading from File.\n");
                    exit(1);
                }
            }

        }
        printf("All %lld Feature vectors read and loaded.\n", data_array->size());
    }
    else{
        printf("Data File not found.\n");
        exit(1);
    }
}

void data_handler::read_feature_labels(std::string path){
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

void data_handler::stuff_data_in_array(int &counter, int &size, std::vector<data *> * &array, std::vector<int32_t> &order){

    int local_counter = 0;
    while(local_counter<size){        
        array->push_back(data_array->at(order[counter]));
        ++counter;
        ++local_counter;
    }

}

void data_handler::split_data(){
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
void data_handler::count_classes(){
    if(*num_classes == 0){
        int count = 0;
        for(data * x : *data_array){
            if(label_map.find(x->get_label())==label_map.end()){
                label_map[x->get_label()]=count;
                ++count;
            }
            x->set_enumerated_label(label_map[x->get_label()]);
        }
        
        *num_classes = count;
    }
    else{
        for(data * x : *data_array){
            if(label_map.find(x->get_label())==label_map.end()){
                label_map[x->get_label()]=x->get_label();
            }
            x->set_enumerated_label(label_map[x->get_label()]);
        }        
    }
    printf("Counted number of classes: %d.\n", *num_classes);
}

uint32_t data_handler::convert_to_little_endian(const unsigned char* bytes){
    return (uint32_t) ((bytes[0] << 24) | 
                        (bytes[1] << 16) | 
                        (bytes[2] << 8) |
                        (bytes[3]));
}

std::vector<data *> * data_handler::get_training_data(){
    return train_data;
}
std::vector<data *> * data_handler::get_test_data(){
    return test_data;
}
std::vector<data *> * data_handler::get_validation_data(){
    return validation_data;
}

int * data_handler::get_class_count(){
    return num_classes;
}

int main(){

    data_handler *dh = new data_handler();

    //dh->read_feature_vector("./data/train-images-idx3-ubyte/train-images.idx3-ubyte");
    //dh->read_feature_labels("./data/train-labels-idx1-ubyte/train-labels.idx1-ubyte");

    printf("cats\n");

    dh->read_csv("./data/iris_data_set/iris.csv", ",", -1);

    dh->split_data();
    dh->count_classes();

    return  0;
}
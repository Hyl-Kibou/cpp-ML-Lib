#include "data_handler.hpp"

data_handler::data_handler(){
    data_array = new std::vector<data *>;
    test_data = new std::vector<data *>;
    train_data = new std::vector<data *>;
    validation_data = new std::vector<data *>;
}
data_handler::~data_handler(){
    //erase the things from data_handler
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
        
        printf("FV File header read.\n");
        
        int image_size = header[2]*header[3];
        for(int i = 0; i<header[1]; ++i){ //Iterate on all elements
            data *d = new data();
            uint8_t element[1];
            
            for(int j = 0; j<image_size; ++j){ //Read all the data from element i
                if(fread(element, sizeof(element), 1, f)){
                    d->append_to_feature_vector(element[0]);                    
                }
                else{
                    printf("Error reading from File.\n");
                    exit(1);
                }
            }

            data_array->push_back(d);

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
        
        for(int i = 0; i<header[1]; ++i){ //Iterate on all elements
            uint8_t element[1];
            
            if(fread(element, sizeof(element), 1, f)){
                data_array->at(i)->set_label(element[0]);
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
    int count = 0;
    for(data * x : *data_array){
        if(class_map.find(x->get_label())==class_map.end()){
            class_map[x->get_label()]=count;
            ++count;
        }
        x->set_enumerated_label(class_map[x->get_label()]);
    }

    num_classes = count;
    printf("Counted number of classes: %d.\n", num_classes);
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

int main1(){

    data_handler *dh = new data_handler();

    dh->read_feature_vector("./data/train-images-idx3-ubyte/train-images.idx3-ubyte");
    dh->read_feature_labels("./data/train-labels-idx1-ubyte/train-labels.idx1-ubyte");

    dh->split_data();
    dh->count_classes();

    return  0;
}
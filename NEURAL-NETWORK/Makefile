CC := g++ -g #-fsanitize=address
SRC := $(CPP_ML_LIB_ROOT)/NEURAL-NETWORK/src
INCLUDE_DIR := $(CPP_ML_LIB_ROOT)/NEURAL-NETWORK/include
LIB_DIR := $(CPP_ML_LIB_ROOT)/lib

CFLAGS := -std=c++17 -fPIC
LDFLAGS := -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR)

all : main

main : obj $(SRC)/network.cpp obj/layer.o obj/neuron.o $(LIB_DIR)/libdata.so
	$(CC) $(CFLAGS) $(SRC)/network.cpp -o main $(LDFLAGS) -I$(INCLUDE_DIR) \
	-I$(CPP_ML_LIB_ROOT)/include -ldata ./obj/*.o

obj:
	if [ ! -d "./obj" ]; then mkdir obj; fi

obj/layer.o : $(SRC)/layer.cpp
	$(CC) -c $(CFLAGS) $(SRC)/layer.cpp -o obj/layer.o -I$(INCLUDE_DIR) \
	-I$(CPP_ML_LIB_ROOT)/include 

obj/neuron.o : $(SRC)/neuron.cpp
	$(CC) -c $(CFLAGS) $(SRC)/neuron.cpp -o obj/neuron.o -I$(INCLUDE_DIR) \
	-I$(CPP_ML_LIB_ROOT)/include 

clean :
	rm -r main obj
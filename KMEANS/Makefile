CC := g++
SRC := $(CPP_ML_LIB_ROOT)/KMEANS/src
INCLUDE_DIR := $(CPP_ML_LIB_ROOT)/KMEANS/include
LIB_DIR := $(CPP_ML_LIB_ROOT)/lib

CFLAGS := -std=c++17 -fPIC
LDFLAGS := -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR)

all : main

main : $(SRC)/kmeans.cpp $(LIB_DIR)/libdata.so
	$(CC) $(CFLAGS) $(SRC)/kmeans.cpp -o main $(LDFLAGS) -I$(INCLUDE_DIR) \
	-I$(CPP_ML_LIB_ROOT)/include -ldata

clean :
	rm main
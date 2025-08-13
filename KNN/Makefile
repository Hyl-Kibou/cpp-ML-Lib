CC := g++
SRC := $(CPP_ML_LIB_ROOT)/KNN/src
INCLUDE_DIR := $(CPP_ML_LIB_ROOT)/KNN/include
LIB_DIR := $(CPP_ML_LIB_ROOT)/lib

CFLAGS := -std=c++17 -DEUCLID
LDFLAGS := -L$(LIB_DIR) -Wl,-rpath,$(LIB_DIR)

all : main

main : $(SRC)/knn.cpp $(LIB_DIR)/libdata.so
	$(CC) $(CFLAGS) $(SRC)/knn.cpp -o main $(LDFLAGS) -I$(INCLUDE_DIR) \
	-I$(CPP_ML_LIB_ROOT)/include -ldata

clean :
	rm main
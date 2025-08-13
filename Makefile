CC:=g++
LOCAL_ROOT := $(CPP_ML_LIB_ROOT)
INCLUDE_DIR := $(LOCAL_ROOT)/include
SRC := $(LOCAL_ROOT)/src
CFLAGS := -shared -std=c++17 -g
LIB_DATA := libdata.so

all : $(LIB_DATA)

$(LIB_DATA) : libdir objdir obj/data_handler.o obj/data.o obj/common.o
	$(CC) $(CFLAGS) -o $(LOCAL_ROOT)/lib/$(LIB_DATA) obj/*.o
	rm -r $(LOCAL_ROOT)/obj
	@echo "Compilation Successful !!"

libdir:
	mkdir -p $(LOCAL_ROOT)/lib

objdir:
	mkdir -p $(LOCAL_ROOT)/obj

obj/data_handler.o : $(SRC)/data_handler.cpp
	$(CC) -fPIC $(CFLAGS) -o obj/data_handler.o -I$(INCLUDE_DIR) -c $(SRC)/data_handler.cpp

obj/data.o : $(SRC)/data.cpp
	$(CC) -fPIC $(CFLAGS) -o obj/data.o -I$(INCLUDE_DIR) -c $(SRC)/data.cpp

obj/common.o : $(SRC)/common.cpp
	$(CC) -fPIC $(CFLAGS) -o obj/common.o -I$(INCLUDE_DIR) -c $(SRC)/common.cpp

clean:
	rm -r $(LOCAL_ROOT)/lib
	rm -r $(LOCAL_ROOT)/obj

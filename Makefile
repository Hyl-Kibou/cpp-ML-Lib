CC:=g++
LOCAL_ROOT := $(CPP_ML_LIB_ROOT)
INCLUDE_DIR := $(LOCAL_ROOT)/include
SRC := $(LOCAL_ROOT)/src
CFLAGS := -shared -std=c++17 -g -fPIC
LIB_DATA := libdata.so

CONFIG_FILE := $(INCLUDE_DIR)/config.hpp

INPUT_TYPE ?= uint8_t
LABEL_TYPE ?= int
CLASS_TYPE ?= uint8_t

all : $(LIB_DATA)

$(LIB_DATA) : set_type libdir objdir obj/data_handler.o obj/data.o obj/common.o 	
	$(CC) $(CFLAGS) -o $(LOCAL_ROOT)/lib/$(LIB_DATA) obj/*.o
	rm -r $(LOCAL_ROOT)/obj
	@echo "Compilation Successful !!"

set_type: update-config
	@echo "Setting LABEL_VAR_TYPE to $(LABEL_TYPE) in $(CONFIG_FILE)..."
	@sed -i 's/^#define LABEL_VAR_TYPE .*/#define LABEL_VAR_TYPE $(LABEL_TYPE)/' $(CONFIG_FILE)
	
	@echo "Setting INPUT_VAR_TYPE to $(INPUT_TYPE) in $(CONFIG_FILE)..."
	@sed -i 's/^#define INPUT_VAR_TYPE .*/#define INPUT_VAR_TYPE $(INPUT_TYPE)/' $(CONFIG_FILE)
	
	@echo "Setting CLASS_VAR_TYPE to $(CLASS_TYPE) in $(CONFIG_FILE)..."
	@sed -i 's/^#define CLASS_VAR_TYPE .*/#define CLASS_VAR_TYPE $(CLASS_TYPE)/' $(CONFIG_FILE)
	
	
update-config:
ifeq ("$(CLASS_TYPE)","std::string")
	@sed -i "s/^#define CLASS_VAR_TYPE_IS_STRING .*/#define CLASS_VAR_TYPE_IS_STRING 1/" $(CONFIG_FILE)
else
	@sed -i "s/^#define CLASS_VAR_TYPE_IS_STRING .*/#define CLASS_VAR_TYPE_IS_STRING 0/" $(CONFIG_FILE)
endif

libdir:
	mkdir -p $(LOCAL_ROOT)/lib

objdir:
	mkdir -p $(LOCAL_ROOT)/obj

obj/data_handler.o : $(SRC)/data_handler.cpp
	$(CC) $(CFLAGS) -o obj/data_handler.o -I$(INCLUDE_DIR) -c $(SRC)/data_handler.cpp

obj/data.o : $(SRC)/data.cpp 
	$(CC) $(CFLAGS) -o obj/data.o -I$(INCLUDE_DIR) -c $(SRC)/data.cpp

obj/common.o : $(SRC)/common.cpp
	$(CC) $(CFLAGS) -o obj/common.o -I$(INCLUDE_DIR) -c $(SRC)/common.cpp

clean:	
	rm -r $(LOCAL_ROOT)/main
	rm -r $(LOCAL_ROOT)/lib
	rm -r $(LOCAL_ROOT)/obj

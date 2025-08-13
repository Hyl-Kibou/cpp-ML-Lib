#! /bin/bash

if [[ -z $CPP_ML_LIB_ROOT ]]; then
    echo "Please define CPP_ML_LIB_ROOT"
    exit 1
fi

dir=$(echo "$@" | tr a-z A-Z) # makes all input uppercase
model_name_lower=$(echo "$@" | tr A-Z a-z)

mkdir -p $CPP_ML_LIB_ROOT/$dir/include $CPP_ML_LIB_ROOT/$dir/src
touch $CPP_ML_LIB_ROOT/$dir/Makefile
touch $CPP_ML_LIB_ROOT/$dir/include/"$model_name_lower.hpp"
touch $CPP_ML_LIB_ROOT/$dir/src/"$model_name_lower.cpp"
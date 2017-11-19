COMMON_LINK=-lboost_regex -luuid -pthread -lrt -ldl 
COMMON_INCLUDE=-I../
COMMON_MACRO=-D__LINUX
#COMMON_OPTION=-Wl,--no-as-needed
COMMON_CFLAGS=-g -std=c++11 -fPIC
COMMON_CXX=g++

BUILD_LIBRARY_PATH=../../build/lib
BUILD_INCLUDE_PATH=../../build/include
OUTPUT_LIB_PATH=/usr/lib64
OUTPUT_INCLUDE_PATH=/usr/include
OUTPUT_TEMPORARY_PATH=../../out

.PHONY: clean all install


COMMON_LIBRARY=-luuid -pthread -lrt -ldl 
COMMON_INCLUDE=-I../
COMMON_MACRO=-D__LINUX
#COMMON_OPTION=-Wl,--no-as-needed
COMMON_CFLAGS=-g -std=c++11 -fPIC
COMMON_CXX=g++

PUBLIC_COMMON_LIBRARY=-lboost_filesystem -lboost_regex

BUILD_LIBRARY_PATH=../../build/lib
BUILD_INCLUDE_PATH=../../build/include
OUTPUT_LIB_PATH=/usr/local/lib64
OUTPUT_INCLUDE_PATH=/usr/local/include
OUTPUT_TEMPORARY_PATH=../../out
OUTPUT_EXAMPLE_PATH=../../out

.PHONY: clean all install example


LIB_NAME=fr_public
#
PROJECT_INC=../..
THIRDPART_PATH=${PROJECT_INC}/library
THIRDPART_INC_PATH=${THIRDPART_PATH}/include
THIRDPART_LIB_PATH=${THIRDPART_PATH}/lib64
#
THIRDPART_INC=-I${THIRDPART_INC_PATH}
LIB_INC=-I. -I${PROJECT_INC}/src ${THIRDPART_INC}
#
THIRDPART_LIB=-ljsoncpp -lboost_regex
LIB_LIB=-L${THIRDPART_PATH}/lib64 ${THIRDPART_LIB} -luuid -pthread -lrt -ldl
#
CXX=g++
MACRO=-D__LINUX
CFLAGS=-g -std=c++11
LINK_NONEEDED=-Wl,--no-as-needed

PROJECT_PATH=../../out
PROJECT_OBJECTS= \
	./$(PROJECT_PATH)/./pub_cfg.o \
	./$(PROJECT_PATH)/./pub_collection.o \
	./$(PROJECT_PATH)/./pub_file.o \
	./$(PROJECT_PATH)/./pub_ini.o \
	./$(PROJECT_PATH)/./pub_json.o \
	./$(PROJECT_PATH)/./pub_log.o \
	./$(PROJECT_PATH)/./pub_md5.o \
	./$(PROJECT_PATH)/./pub_memory.o \
	./$(PROJECT_PATH)/./pub_rc4.o \
	./$(PROJECT_PATH)/./pub_string.o \
	./$(PROJECT_PATH)/./pub_test.o \
	./$(PROJECT_PATH)/./pub_thread.o \
	./$(PROJECT_PATH)/./pub_timer.o \
	./$(PROJECT_PATH)/./pub_tool.o

./${PROJECT_PATH}/%.o : %.cpp
	-mkdir -p ./${PROJECT_PATH}/$(dir $<)
	$(CXX) $(CFLAGS) $(MACRO) -c $(LIB_INC) $< -o $@

TARGET=lib${LIB_NAME}.a
all : ${PROJECT_OBJECTS}
	ar crv ${THIRDPART_PATH}/lib64/${TARGET} ${PROJECT_OBJECTS} 
	mkdir -p ${THIRDPART_INC_PATH}/${LIB_NAME}
	rm -f ${THIRDPART_INC_PATH}/${LIB_NAME}/*
	cp -rf *.h ${THIRDPART_INC_PATH}/${LIB_NAME}

.PHONY: clean
clean :
	rm -rf ${PROJECT_INC}/out/pub_*.o
	rm -f ${THIRDPART_PATH}/lib64/${TARGET}
	rm -f ${THIRDPART_INC_PATH}/${LIB_NAME}/*



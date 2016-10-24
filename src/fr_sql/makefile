LIB_NAME=fr_sql
#
PROJECT_INC=../..
THIRDPART_PATH=${PROJECT_INC}/library
THIRDPART_INC_PATH=${THIRDPART_PATH}/include
THIRDPART_LIB_PATH=${THIRDPART_PATH}/lib64

THIRDPART_INC=-I${THIRDPART_INC_PATH}
LIB_INC=-I. -I${PROJECT_INC}/src ${THIRDPART_INC}
#
THIRDPART_LIB=-lmysqlcppconn
LIB_LIB=-L${THIRDPART_PATH}/lib64 ${THIRDPART_LIB} -luuid -pthread -lrt -ldl
#
CXX=g++
MACRO=-DDEBUG -D__LINUX
LDFLAGS=-shared
CFLAGS=-g -std=c++11 -fPIC
LINK_NONEEDED=-Wl,--no-as-needed

PROJECT_PATH=../../out
PROJECT_OBJECTS= \
	./$(PROJECT_PATH)/./sql_manager.o \
	./$(PROJECT_PATH)/./sql_operator.o

./${PROJECT_PATH}/%.o : %.cpp
	-mkdir -p ./${PROJECT_PATH}/$(dir $<)
	$(CXX) $(CFLAGS) $(MACRO) -c $(LIB_INC) $< -o $@

TARGET=libfr_sql.so
all : ${PROJECT_OBJECTS}
	$(CXX) $(LDFLAGS) -o ${THIRDPART_PATH}/lib64/${TARGET} ${PROJECT_OBJECTS} ${MAIN_OBJECT} ${LIB_LIB} ${LINK_NONEEDED}
	mkdir -p ${THIRDPART_INC_PATH}/${LIB_NAME}
	rm -f ${THIRDPART_INC_PATH}/${LIB_NAME}/*
	cp -rf *.h ${THIRDPART_INC_PATH}/${LIB_NAME}

.PHONY: clean
clean :
	rm -rf ${PROJECT_INC}/out/sql_*.o
	rm -f ${THIRDPART_PATH}/lib64/${TARGET}
	rm -f ${THIRDPART_INC_PATH}/${LIB_NAME}/*


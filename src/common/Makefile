PROJECT_INC=../..
THIRDPART_PATH=${PROJECT_INC}/library
THIRDPART_INC=-I${THIRDPART_PATH}/include \
	-I${THIRDPART_PATH}/libevent \
	-I${THIRDPART_PATH}/mysql \
	-I${THIRDPART_PATH}/xlsreader \
	-I${THIRDPART_PATH}/xlswriter
LIB_INC=-I. -I${PROJECT_INC}/src ${THIRDPART_INC}
#
#THIRDPART_LIB=-ljsoncpp -lboost_regex
LIB_LIB=-L${THIRDPART_PATH}/lib64 ${THIRDPART_LIB} -luuid -pthread -lrt -ldl
#
CXX=g++
MACRO=-DDEBUG -D__LINUX
LDFLAGS=-shared
CFLAGS=-g -std=c++11 -fPIC
LINK_NONEEDED=-Wl,--no-as-needed

PROJECT_PATH=../../out
PROJECT_OBJECTS= \
	./$(PROJECT_PATH)/./c_collection.o
#	./$(PROJECT_PATH)/./c_collection.o \
#./$(PROJECT_PATH)/./c_file.o \
#	./$(PROJECT_PATH)/./c_ini.o \
#	./$(PROJECT_PATH)/./c_log.o \
#	./$(PROJECT_PATH)/./c_memory.o \
#	./$(PROJECT_PATH)/./c_thread.o \
#	./$(PROJECT_PATH)/./c_timer.o \
#	./$(PROJECT_PATH)/./c_tool.o \
#	./$(PROJECT_PATH)/./c_json.o \
#	./$(PROJECT_PATH)/./c_string.o

./${PROJECT_PATH}/%.o : %.cpp
	-mkdir -p ./${PROJECT_PATH}/$(dir $<)
	$(CXX) $(CFLAGS) $(MACRO) -c $(LIB_INC) $< -o $@

TARGET=libcommon.so
all : ${PROJECT_OBJECTS}
	$(CXX) $(LDFLAGS) -o ${THIRDPART_PATH}/lib64/${TARGET} ${PROJECT_OBJECTS} ${MAIN_OBJECT} ${LIB_LIB} ${LINK_NONEEDED}

.PHONY: clean tags static
static : ${PROJECT_OBJECTS}
	ar crv ${THIRDPART_PATH}/lib64/libcommon.a ${PROJECT_OBJECTS}
clean :
	rm -rf ${PROJECT_INC}/out/c_*.o
	rm -f ${THIRDPART_PATH}/lib64/${TARGET}
tags:
	rm -rf ${PROJECT_INC}/tags
	touch ${PROJECT_INC}/tags
	find ${PROJECT_INC} -iname '*.cpp' -or -iname '*.c' -or -iname '*.h'-or -iname '*.hpp' | xargs ctags --c++-kinds=+p --fields=+iaS --extra=+q --langmap=c++:+.inl -a ${PROJECT_INC}/tags



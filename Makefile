# software name.
MAIN_NAME=main
SOFTWARE_NAME=Main_64
#
THIRDPART_PATH=./library
THIRDPART_INC=-I${THIRDPART_PATH}/include \
	-I${THIRDPART_PATH}/libevent \
	-I${THIRDPART_PATH}/mysql \
	-I${THIRDPART_PATH}/xlsreader \
	-I${THIRDPART_PATH}/xlswriter
PROJECT_INC=-I./src
LIB_INC=${PROJECT_INC} ${THIRDPART_INC}
#
THIRDPART_LIB=-ljsoncpp -lxls -lxlsreader -levent -lmysqlcppconn-static -lboost_regex
LIB_LIB=-L${THIRDPART_PATH}/lib64 -luuid -pthread -lrt -ldl ${THIRDPART_LIB}
#
CXX=g++
MACRO=-DDEBUG -D__LINUX
CXXFLAG= -g -std=c++11 $(MACRO) -O2
LINK_NONEEDED=-Wl,--no-as-needed

PROJECT_PATH=out
PROJECT_OBJECTS= \
	./$(PROJECT_PATH)/./src/sql/sql_operator.o \
	./$(PROJECT_PATH)/./src/network/net_connection.o \
	./$(PROJECT_PATH)/./src/network/net_interface.o \
	./$(PROJECT_PATH)/./src/network/net_packer.o \
	./$(PROJECT_PATH)/./src/network/net_protocol.o \
	./$(PROJECT_PATH)/./src/network/net_struct.o \
	./$(PROJECT_PATH)/./src/network/socket_simple.o \
	./$(PROJECT_PATH)/./src/network/net_test.o \
	./$(PROJECT_PATH)/./src/common/collection.o \
	./$(PROJECT_PATH)/./src/common/common_file.o \
	./$(PROJECT_PATH)/./src/common/common_ini.o \
	./$(PROJECT_PATH)/./src/common/common_log.o \
	./$(PROJECT_PATH)/./src/common/common_memory.o \
	./$(PROJECT_PATH)/./src/common/common_mutex.o \
	./$(PROJECT_PATH)/./src/common/common_thread.o \
	./$(PROJECT_PATH)/./src/common/common_timer.o \
	./$(PROJECT_PATH)/./src/common/common_tool.o \
	./$(PROJECT_PATH)/./src/common/json_tool.o \
	./$(PROJECT_PATH)/./src/common/string_util.o \
	./$(PROJECT_PATH)/./src/xls/xls_reader.o \
	./$(PROJECT_PATH)/./src/xls/xls_struct.o \
	./$(PROJECT_PATH)/./src/xls/xls_writer.o

MAIN_OBJECT=./$(PROJECT_PATH)/./src/${MAIN_NAME}.o

./${PROJECT_PATH}/%.o : %.cpp
	-mkdir -p ./${PROJECT_PATH}/$(dir $<)
	$(CXX) $(CXXFLAG) -c $(LIB_INC) $< -o $@

all : ${PROJECT_OBJECTS} ${MAIN_OBJECT}
	$(CXX) $(CXXFLAG) -o ${SOFTWARE_NAME} ${PROJECT_OBJECTS} ${MAIN_OBJECT} ${LIB_LIB} ${LINK_NONEEDED}

.PHONY: clean tags install
# 安装是指生成静态链接文件。
install : ${PROJECT_OBJECTS}
	ar crv ./library/lib64/libcommon.a ${PROJECT_OBJECTS}
clean :
	rm -rf ./out/
	rm -f ${SOFTWARE_NAME}
tags:
	rm -rf tags
	touch tags
	find . -iname '*.cpp' -or -iname '*.c' -or -iname '*.h'-or -iname '*.hpp' | xargs ctags --c++-kinds=+p --fields=+iaS --extra=+q --langmap=c++:+.inl -a tags



# software name.
MAIN_NAME=main
SOFTWARE_NAME=Main_64

# third part path
THIRDPART_PATH=./thirdpart
# mysql ( mysql connection )
MYSQL_INCLUDE=
# xlslib ( xls writer )
XLSLIB_INCLUDE=-I${THIRDPART_PATH}/include/xlslib
# libxls ( xls reader )
LIBXLS_INCLUDE=-I${THIRDPART_PATH}/include/libxls
# json ( json cpp )   json 不能直接使用目录，会和库文件冲突
JSON_INCLUDE=-I${THIRDPART_PATH}/include/
# boost
BOOST_INC=-I${THIRDPART_PATH}/include/boost
# thirdpart include 
THIRDPART_INC=-I${THIRDPART_PATH}/include $(MYSQL_INCLUDE) ${XLSLIB_INCLUDE} ${LIBXLS_INCLUDE} ${JSON_INCLUDE} ${BOOST_INC}
# project child path
PROJECT_INC=-I./src \
		    -I./src/tool \
		    -I./src/exception
# all include path
LIB_INC=-I. ${PROJECT_INC} ${THIRDPART_INC} 


# ljsoncpp
JSON_LIB=-ljsoncpp
# xlslib
XLS_WRITER_LIB=-lxls
# libxls
XLS_READER_LIB=-lxlsreader
# event
EVENT_LIB=-levent
# mysql connector ++ lib
MYSQL_LIB=-lmysqlcppconn-static -lmysqlclient
# boost regex
BOOST_LIB=-lboost_regex
THIRDPART_LIB=${JSON_LIB} ${XLS_WRITER_LIB} ${XLS_READER_LIB} ${EVENT_LIB} ${MYSQL_LIB} ${BOOST_LIB}

LIB_LIB=-L/usr/lib -L/usr/local/lib -L/usr/lib64/mysql -L${THIRDPART_PATH}/lib64 -L${THIRDPART_PATH}/lib64/boost_lib ${THIRDPART_LIB} -luuid -pthread -lrt -ldl

# my local test/debug
# DEBUG宏等级定义：
# DETAIL_DEBUG:
# COMMON_DEBUG:
# [project]_DEBUG:
LOCALDEBUG=-D__COMMON -DCOMMON_DEBUG -D__LINUX

CXX=g++
CXXFLAG= -g -DDEBUG $(LOCALDEBUG) -O2
#CXXFLAG= -std=c++11 -g -DDEBUG $(LOCALDEBUG) -O2
LINK_NONEEDED=-Wl,--no-as-needed

	#./$(PROJECT_PATH)/./src/sql/CSSqlOperator.o
PROJECT_PATH=out
PROJECT_OBJECTS= \
	./$(PROJECT_PATH)/./src/exception/CSException.o \
	./$(PROJECT_PATH)/./src/tool/CSCommonTool.o \
	./$(PROJECT_PATH)/./src/tool/CSFile.o \
	./$(PROJECT_PATH)/./src/tool/CSFileTransmission.o \
	./$(PROJECT_PATH)/./src/tool/CSJsonTool.o \
	./$(PROJECT_PATH)/./src/tool/CSPMutex.o \
	./$(PROJECT_PATH)/./src/tool/CSStrUtil.o \
	./$(PROJECT_PATH)/./src/tool/CSTimer.o \
	./$(PROJECT_PATH)/./src/tool/CSXlsReader.o \
	./$(PROJECT_PATH)/./src/tool/CSXlsStruct.o \
	./$(PROJECT_PATH)/./src/tool/CSXlsWriter.o \
	./$(PROJECT_PATH)/./src/${MAIN_NAME}.o \


./${PROJECT_PATH}/%.o : %.cpp
	-mkdir -p ./${PROJECT_PATH}/$(dir $<)
	$(CXX) $(CXXFLAG) -c $(LIB_INC) -L./ $< -o $@

all : ${PROJECT_OBJECTS}
	$(CXX) $(CXXFLAG) -o ${SOFTWARE_NAME} ${PROJECT_OBJECTS} ${LIB_LIB} ${LINK_NONEEDED}

# install software
RELEASE_PATH=build
install :

restart :

# tar (gz)
tar :

# common 
	
cleanall :
	rm -rf ./out/
	rm -f ${SOFTWARE_NAME}
clean :
	rm -rf ./out/
	rm -f ${SOFTWARE_NAME}



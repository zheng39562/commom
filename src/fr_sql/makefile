include ../../make/config.mk

PROJECT_NAME=fr_sql
PROJECT_OBJECTS= \
	./$(OUTPUT_TEMPORARY_PATH)/./sql_operator.o \
	./$(OUTPUT_TEMPORARY_PATH)/./sql_operator_pool.o

# 此路径需要自定义
SQL_INCLUDE=-I/usr/include/mysqlcppconn/
# need library -lmysqlcppconn
SQL_LIBRARY=-lmysqlcppconn

./${OUTPUT_TEMPORARY_PATH}/%.o : %.cpp
	-mkdir -p ./${OUTPUT_TEMPORARY_PATH}/$(dir $<)
	$(COMMON_CXX) $(COMMON_CFLAGS) $(COMMON_MACRO) -c $(COMMON_INCLUDE) ${SQL_INCLUDE} $< -o $@

all : ${PROJECT_OBJECTS}
	mkdir -p ${BUILD_LIBRARY_PATH}
	mkdir -p ${BUILD_INCLUDE_PATH}/${PROJECT_NAME}
	mkdir -p ${OUTPUT_EXAMPLE_PATH}
	ar crv ${BUILD_LIBRARY_PATH}/lib${PROJECT_NAME}.a ${PROJECT_OBJECTS} 
	$(COMMON_CXX) ${COMMON_CFLAGS} ${PROJECT_OBJECTS} -fPIC -shared -o ${BUILD_LIBRARY_PATH}/lib${PROJECT_NAME}.so
	cp -rf *.h ${BUILD_INCLUDE_PATH}/${PROJECT_NAME}

install:
	cp ${BUILD_LIBRARY_PATH}/* ${OUTPUT_LIB_PATH}
	cp -rf ${BUILD_INCLUDE_PATH}/* ${OUTPUT_INCLUDE_PATH}/

clean :
	rm -rf ${OUTPUT_TEMPORARY_PATH}/sql_*.o
	rm -f ${BUILD_LIBRARY_PATH}/lib${PROJECT_NAME}.*
	rm -f ${BUILD_LIBRARY_PATH}/${PROJECT_NAME}/*
	rm -f ${OUTPUT_EXAMPLE_PATH}/sql_example

example : sql_example.cpp
	${COMMON_CXX} ${COMMON_CFLAGS} ${SQL_INCLUDE} ${COMMON_INCLUDE} ${SQL_LIBRARY} ${COMMON_LIBRARY} ${PUBLIC_COMMON_LIBRARY} -l${PROJECT_NAME} -lfr_public sql_example.cpp -o ${OUTPUT_EXAMPLE_PATH}/sql_example


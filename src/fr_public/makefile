include ../../make/config.mk

PROJECT_NAME=fr_public
PROJECT_OBJECTS= \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_cfg.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_file.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_ini.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_log.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_md5.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_memory.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_rc4.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_string.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_timer.o \
	./$(OUTPUT_TEMPORARY_PATH)/./pub_tool.o

./${OUTPUT_TEMPORARY_PATH}/%.o : %.cpp
	-mkdir -p ./${OUTPUT_TEMPORARY_PATH}/$(dir $<)
	$(COMMON_CXX) $(COMMON_CFLAGS) $(COMMON_MACRO) -c $(COMMON_INCLUDE) $< -o $@

all : ${PROJECT_OBJECTS}
	mkdir -p ${BUILD_LIBRARY_PATH}
	mkdir -p ${BUILD_INCLUDE_PATH}/${PROJECT_NAME}
	ar crv ${BUILD_LIBRARY_PATH}/lib${PROJECT_NAME}.a ${PROJECT_OBJECTS} 
	g++ ${COMMON_CFLAGS} ${PROJECT_OBJECTS} -fPIC -shared -o ${BUILD_LIBRARY_PATH}/lib${PROJECT_NAME}.so
	cp -rf *.h ${BUILD_INCLUDE_PATH}/${PROJECT_NAME}

install:
	cp ${BUILD_LIBRARY_PATH}/* ${OUTPUT_LIB_PATH}
	cp -rf ${BUILD_INCLUDE_PATH}/* ${OUTPUT_INCLUDE_PATH}/

clean :
	rm -rf ${OUTPUT_TEMPORARY_PATH}/pub_*.o
	rm -f ${BUILD_LIBRARY_PATH}/lib${PROJECT_NAME}.*
	rm -f ${BUILD_LIBRARY_PATH}/${PROJECT_NAME}/*


# 编译参数

SRC_PATH=./src
# public
PUB_MOD=fr_public
SQL_MOD=fr_sql
TEMPLATE_MOD=fr_template
XLS_MOD=fr_xls

# 编译函数定义
define make_pub
	@for sub in $(PUB_MOD); do \
		(cd ${SRC_PATH}/$$sub && make $1) \
	done;
endef

define make_sql
	@for sub in $(SQL_MOD); do \
		(cd ${SRC_PATH}/$$sub && make $1) \
	done;
endef

define make_template
	@for sub in $(TEMPLATE_MOD); do \
		(cd ${SRC_PATH}/$$sub && make $1) \
	done;
endef

define make_xls
	@for sub in $(XLS_MOD); do \
		(cd ${SRC_PATH}/$$sub && make $1) \
	done;
endef

all: fr_public fr_sql fr_template fr_xls

# 测试使用main 链接库根据测试调整。
test:
	g++ -DDEBUG -D__LINUX -g -std=c++11 ./src/main.cpp -I./library/include/ -L./library/lib64/ -lfr_public -lfr_xls -lfr_sql -lboost_regex -luuid -pthread -lrt -ldl -o Main_64 
		
.PHONY: fr_public fr_sql fr_template fr_xls help

fr_public:
#$(shell cp ./lib/* .)
	$(call make_pub, all)

fr_sql:
	$(call make_sql, all)

fr_template:
	$(call make_template, all)

fr_xls:
	$(call make_xls, all)

fr_pub_clean:
	$(call make_pub, clean)

fr_sql_clean:
	$(call make_sql, clean)

fr_template_clean:
	$(call make_template, clean)

fr_xls_clean:
	$(call make_xls, clean)

clean: fr_pub_clean fr_sql_clean fr_template_clean fr_xls_clean 

help:
	echo "makefile parameter : fr_public fr_sql fr_template fr_xls"
	echo "clear parameter : fr_pub_clean fr_sql_clean fr_template_clean fr_xls_clean"
	echo "if you want clean all. you can input make clean"

tags:
	rm -rf tags
	touch tags
	find . -iname '*.cpp' -or -iname '*.c' -or -iname '*.h'-or -iname '*.hpp' | xargs ctags --c++-kinds=+p --fields=+iaS --extra=+q --langmap=c++:+.inl -a tags

# 一些历史遗留。部分可能还会用到
#CFLAGS=-g -std=c++11 -fPIC
#LDFLAGS=-shared
#$(CXX) $(LDFLAGS) -o ${THIRDPART_LIB_PATH}/${TARGET} ${PROJECT_OBJECTS} ${MAIN_OBJECT} ${LIB_LIB} ${LINK_NONEEDED}


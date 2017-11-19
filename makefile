# 编译参数

# phony
.PHONY: fr_public fr_sql help install

# 编译函数定义
# $(1) : src path
# $(2) : make command
define make_func
	(cd $(1) && make $(2))
endef

ALL_PROJECT_NAME=fr_public fr_sql fr_template

all: 
	$(call make_func, ./src/fr_public, all)
	$(call make_func, ./src/fr_sql, all)

install: 
	$(call make_func, ./src/fr_public, install)
	$(call make_func, ./src/fr_sql, install)
	$(call make_func, ./src/fr_template, install)

clean: 
	$(call make_func, ./src/fr_public, clean)
	$(call make_func, ./src/fr_sql, clean)
		
fr_public:
	$(call make_func, ./src/fr_public, all)

fr_sql:
	$(call make_func, ./src/fr_sql, all)

fr_pub_clean:
	$(call make_func, ./src/fr_public, clean)

fr_sql_clean:
	$(call make_func, ./src/fr_sql, clean)

fr_template_clean:
	$(call make_func, ./src/fr_template, clean)

help:
	echo "command help"
	echo " --| all : "
	echo " --| install : "
	echo " --| clean : "
	echo " --| fr_public : "
	echo " --| fr_sql : "
	echo " --| fr_pub_clean : "
	echo " --| fr_sql_clean : "
	echo " --| fr_template_clean : "


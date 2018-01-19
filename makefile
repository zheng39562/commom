# 编译参数

# phony
.PHONY: frpublic frsql help install

# 编译函数定义
# $(1) : src path
# $(2) : make command
define make_func
	(cd $(1) && make $(2))
endef

ALL_PROJECT_NAME=frpublic frsql frtemplate

all: 
	$(call make_func, ./src/frpublic, all)
	$(call make_func, ./src/frsql, all)

install: 
	$(call make_func, ./src/frpublic, install)
	$(call make_func, ./src/frsql, install)
	$(call make_func, ./src/frtemplate, install)

clean: 
	$(call make_func, ./src/frpublic, clean)
	$(call make_func, ./src/frsql, clean)
		
frpublic:
	$(call make_func, ./src/frpublic, all)

frsql:
	$(call make_func, ./src/frsql, all)

frpub_clean:
	$(call make_func, ./src/frpublic, clean)

frsql_clean:
	$(call make_func, ./src/frsql, clean)

frtemplate_clean:
	$(call make_func, ./src/frtemplate, clean)

help:
	echo "command help"
	echo " --| all : "
	echo " --| install : "
	echo " --| clean : "
	echo " --| frpublic : "
	echo " --| frsql : "
	echo " --| frpub_clean : "
	echo " --| frsql_clean : "
	echo " --| frtemplate_clean : "


/**********************************************************
 *  \!file common_ini.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "common_ini.h"

#include "tool/common_tool.h"

namespace Universal{
	IniCfg::IniCfg()
		:m_IniInfo()
	{
		;
	}

	IniCfg::~IniCfg(){ ; }

	void IniCfg::InitFile(const string &filePath){
		;
	}

	string IniCfg::getString(const string &section, const string &key, string defaultValue){
		map<string, map<string, string> >::const_iterator citerSection = m_IniInfo.find(section);
		if(citerSection != m_IniInfo.end()){
			if(citerSection->second.find(key) != citerSection->second.end()){
				return citerSection->second.find(key)->second;
			}
		}
		return defaultValue;
	}

	int IniCfg::getInt(const string &section, const string &key, int defaultValue){
		string value = getString(section, key);
		if(checkNumber(value)){
			return strToInt(value);
		}
		return defaultValue;
	}

	double IniCfg::getDouble(const string &section, const string &key, double defaultValue){
		string value = getString(section, key);
		if(checkNumber(value)){
			return strToDouble(value);
		}
		return defaultValue;
	}

}

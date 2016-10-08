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

#include "common/common_tool.h"
#include "common/common_file.h"
#include "common/string_util.h"

namespace Universal{
	using namespace std;

	IniCfg::IniCfg()
		:m_IniInfo()
	{
		;
	}

	IniCfg::~IniCfg(){ ; }

	bool IniCfg::InitFile(const string &filePath){
		vector<string> lines;
		if(splitString(readFile(filePath), "\n", lines)){
			map<string, string>* pKeyValueMap(NULL);
			for(vector<string>::const_iterator citerLine = lines.begin(); citerLine != lines.end(); ++citerLine){
				if(isNewSection(*citerLine)){
					pKeyValueMap = NULL;
					string section = citerLine->substr(1, citerLine->size()-2);
					m_IniInfo.insert(pair<string, map<string, string> >(section, map<string, string>()));
					pKeyValueMap = &(m_IniInfo.find(section)->second);
				}
				else{
					 size_t pos = citerLine->find('=');
					 if(pKeyValueMap == NULL || pos == string::npos || pos == 0){
						 DEBUG_E("ini文件格式错误。");
						 return false;
					 }
					 pKeyValueMap->insert(pair<string, string>(
								 citerLine->substr(0, pos), 
								 citerLine->substr(pos+1, citerLine->size() - pos - 1)
								 ));
				}
			}
		}
		else{
			DEBUG_E("文件不存在，或格式错误。（注：使用\\n分隔行，而非\\r\\n）");
			return false;
		}

		return true;
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

	bool IniCfg::isNewSection(const string &section){
		if(section.size() <= 2 || section[0] != '[' || section[section.size()-1] != ']'){
			return false;
		}
		return true;
	}

}

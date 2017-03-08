/**********************************************************
 *  \!file c_ini.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "fr_public/pub_ini.h"

#include "fr_public/pub_tool.h"
#include "fr_public/pub_file.h"
#include "fr_public/pub_string.h"

using namespace std;

namespace Universal{
	IniCfg::IniCfg()
		:m_IniInfo(),
		 m_IniContent(),
		 m_FilePath()
	{
		;
	}

	IniCfg::~IniCfg(){ ; }

	bool IniCfg::initFile(const string &filePath){
		m_FilePath = filePath;
		vector<string> lines;
		m_IniContent = readFile(m_FilePath);
		strReplaceAll(m_IniContent, "\r\n", "\n");
		if(splitString(m_IniContent, "\n", lines)){
			map<string, string>* pKeyValueMap(NULL);
			for(vector<string>::const_iterator citerLine = lines.begin(); citerLine != lines.end(); ++citerLine){
				if(isNote(*citerLine)){ continue; }

				if(isNewSection(*citerLine)){
					pKeyValueMap = NULL;
					string section = citerLine->substr(1, citerLine->size()-2);
					if(m_IniInfo.find(section) == m_IniInfo.end()){
						m_IniInfo.insert(pair<string, map<string, string> >(section, map<string, string>()));
						pKeyValueMap = &(m_IniInfo.find(section)->second);
					}
				}
				else{
					string str = citerLine->substr(0, citerLine->find('#'));
					size_t pos = str.find('=');
					if(pKeyValueMap == NULL || pos == string::npos || pos == 0){
						PUB_DEBUG_E("ini文件格式错误。");
#ifdef _DEBUG
						assert(false);
#endif
						return false;
					}
					pKeyValueMap->insert(pair<string, string>(
							trim(str.substr(0, pos)), 
							trim(str.substr(pos+1, str.size() - pos - 1))
							));
				}
			}
		}
		else{
			PUB_DEBUG_E("文件不存在，或格式错误。（注：使用\\n分隔行，而非\\r\\n）");
			return false;
		}

		return true;
	}

	void IniCfg::saveFile(const std::string &filePath){
		writeFile(filePath, m_IniContent);
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

	bool IniCfg::setString(const std::string &section, const std::string &key, const std::string &value){
		auto iterSection = m_IniInfo.find(section);
		if(iterSection != m_IniInfo.end()){
			auto iterKeyValue = iterSection->second.find(key);
			if(iterKeyValue != iterSection->second.end()){
				iterKeyValue->second = value;
				return updateIniContent(section, key, value);
			}
			else{
				PUB_DEBUG_W("设置ini配置信息失败 : key[" << key << "] 不存在");
			}
		}
		else{
			PUB_DEBUG_W("设置ini配置信息失败 : section[" << section << "] 不存在");
		}
		return false;
	}

	bool IniCfg::setInt(const std::string &section, const std::string &key, int32 value){
		return setString(section, key, intToStr(value));
	}

	bool IniCfg::setDouble(const std::string &section, const std::string &key, double value){
		return setString(section, key, doubleToStr(value));
	}

	bool IniCfg::isNewSection(const string &section){
		if(section.size() <= 2 || section[0] != '[' || section[section.size()-1] != ']'){
			return false;
		}
		return true;
	}

	bool IniCfg::isNote(const std::string &str){
		return str.empty() || trim(str)[0] == '#';
	}

	bool IniCfg::updateIniContent(const std::string &section, const std::string &key, const std::string &value){
		string::size_type sectionPos(m_IniContent.find("\n[" + section + "]") + 1);
		if(sectionPos != std::string::npos){
			string::size_type nextSectionPos = m_IniContent.find("\n[", sectionPos);
			nextSectionPos = nextSectionPos != std::string::npos ? nextSectionPos : m_IniContent.size();

			string::size_type keyPos = m_IniContent.find(string("\n" + key).c_str(), sectionPos) + 1;
			if(keyPos < nextSectionPos){
				string::size_type valueBeginPos = m_IniContent.find('=', keyPos) + 1; 
				string::size_type valueEndPos = m_IniContent.find('\n', keyPos); 
				if(valueBeginPos != std::string::npos && valueEndPos != std::string::npos && valueBeginPos < valueEndPos){
					m_IniContent.replace(valueBeginPos, valueEndPos - valueBeginPos, value);
					return true;
				}
			}
		}
		return false;
	}

}

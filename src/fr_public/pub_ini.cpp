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
#include "pub_ini.h"

#include "pub_tool.h"
#include "pub_file.h"
#include "pub_string.h"

using namespace std;

namespace universal{
	IniCfg::IniCfg()
		:ini_info_(),
		 ini_content_(),
		 file_path_()
	{
		;
	}

	IniCfg::~IniCfg(){ ; }

	bool IniCfg::InitFile(const string &filePath){
		file_path_ = filePath;
		vector<string> lines;
		ini_content_ = ReadFile(file_path_);
		StrReplaceAll(ini_content_, "\r\n", "\n");
		if(SplitString(ini_content_, "\n", lines)){
			map<string, string>* pKeyValueMap(NULL);
			for(vector<string>::const_iterator citerLine = lines.begin(); citerLine != lines.end(); ++citerLine){
				if(IsNote(*citerLine)){ continue; }

				if(IsNewSection(*citerLine)){
					pKeyValueMap = NULL;
					string section = citerLine->substr(1, citerLine->size()-2);
					if(ini_info_.find(section) == ini_info_.end()){
						ini_info_.insert(pair<string, map<string, string> >(section, map<string, string>()));
						pKeyValueMap = &(ini_info_.find(section)->second);
					}
				}
				else{
					string str = citerLine->substr(0, citerLine->find('#'));
					size_t pos = str.find('=');
					if(pKeyValueMap == NULL || pos == string::npos || pos == 0){
						std::runtime_error("ini文件格式错误。");
#ifdef _DEBUG
						assert(false);
#endif
						return false;
					}
					pKeyValueMap->insert(pair<string, string>(
							Trim(str.substr(0, pos)), 
							Trim(str.substr(pos+1, str.size() - pos - 1))
							));
				}
			}
		}
		else{
			std::runtime_error("文件不存在，或格式错误。（注：使用\\n分隔行，而非\\r\\n）");
			return false;
		}

		return true;
	}

	void IniCfg::SaveFile(const std::string &filePath){
		WriteFile(filePath, ini_content_);
	}

	string IniCfg::GetString(const string &section, const string &key, string defaultValue){
		map<string, map<string, string> >::const_iterator citerSection = ini_info_.find(section);
		if(citerSection != ini_info_.end()){
			if(citerSection->second.find(key) != citerSection->second.end()){
				return citerSection->second.find(key)->second;
			}
		}
		return defaultValue;
	}

	int IniCfg::GetInt(const string &section, const string &key, int defaultValue){
		string value = GetString(section, key);
		if(CheckNumber(value)){
			return stoi(value);
		}
		return defaultValue;
	}

	double IniCfg::GetDouble(const string &section, const string &key, double defaultValue){
		string value = GetString(section, key);
		if(CheckNumber(value)){
			return stod(value);
		}
		return defaultValue;
	}

	bool IniCfg::SetString(const std::string &section, const std::string &key, const std::string &value){
		auto iterSection = ini_info_.find(section);
		if(iterSection != ini_info_.end()){
			auto iterKeyValue = iterSection->second.find(key);
			if(iterKeyValue != iterSection->second.end()){
				iterKeyValue->second = value;
				return UpdateIniContent(section, key, value);
			}
			else{
				std::runtime_error("设置ini配置信息失败 : key[" + key + "] 不存在");
			}
		}
		else{
			std::runtime_error("设置ini配置信息失败 : section[" + section + "] 不存在");
		}
		return false;
	}

	bool IniCfg::SetInt(const std::string &section, const std::string &key, int value){
		return SetString(section, key, to_string(value));
	}

	bool IniCfg::SetDouble(const std::string &section, const std::string &key, double value){
		return SetString(section, key, to_string(value));
	}

	bool IniCfg::IsNewSection(const string &section){
		if(section.size() <= 2 || section[0] != '[' || section[section.size()-1] != ']'){
			return false;
		}
		return true;
	}

	bool IniCfg::IsNote(const std::string &str){
		return str.empty() || Trim(str)[0] == '#';
	}

	bool IniCfg::UpdateIniContent(const std::string &section, const std::string &key, const std::string &value){
		string::size_type sectionPos(ini_content_.find("\n[" + section + "]") + 1);
		if(sectionPos != std::string::npos){
			string::size_type nextSectionPos = ini_content_.find("\n[", sectionPos);
			nextSectionPos = nextSectionPos != std::string::npos ? nextSectionPos : ini_content_.size();

			string::size_type keyPos = ini_content_.find(string("\n" + key).c_str(), sectionPos) + 1;
			if(keyPos < nextSectionPos){
				string::size_type valueBeginPos = ini_content_.find('=', keyPos) + 1; 
				string::size_type valueEndPos = ini_content_.find('\n', keyPos); 
				if(valueBeginPos != std::string::npos && valueEndPos != std::string::npos && valueBeginPos < valueEndPos){
					ini_content_.replace(valueBeginPos, valueEndPos - valueBeginPos, value);
					return true;
				}
			}
		}
		return false;
	}

}

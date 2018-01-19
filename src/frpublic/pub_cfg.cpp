/**********************************************************
 *  \file pub_cfg.cpp
 *  \brief
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "pub_cfg.h"

#include "pub_ini.h"

using namespace std;

namespace frpublic{
	Configurator::Configurator()
		:cfg_map_(),
		 default_cfg_key_(""),
		 cfg_type_(eCfgDataType_Unknow),
		 mutex_()
	{
		;
	}

	Configurator::~Configurator(){ 
		std::lock_guard<std::mutex> localLock(mutex_);
		for(auto iterCfg = cfg_map_.begin(); iterCfg != cfg_map_.begin(); ++iterCfg){
			if(iterCfg->second != NULL){
				switch(cfg_type_){
					case eCfgDataType_Ini:
						delete (IniCfg*)iterCfg->second; iterCfg->second = NULL;
						break;
					case eCfgDataType_DB:
					default:
						std::runtime_error("未知的配置格式。无法进行delete");
						break;
				}
			}
		}
	}

	bool Configurator::AddCfg(const CfgKey &cfgKey, const std::string &param, const eCfgDataType &type){
		std::lock_guard<std::mutex> localLock(mutex_);
		if(cfg_map_.find(cfgKey) != cfg_map_.end()){
			std::runtime_error("该配置key已经存在，不能重复添加.");
		}

		cfg_type_ = type;

		void* m_pCfg(NULL);
		switch (cfg_type_){
		case eCfgDataType_Ini:
			if (m_pCfg != NULL){
				delete (IniCfg*)m_pCfg; m_pCfg = NULL;
			}

			m_pCfg = new IniCfg();
			if (((IniCfg*)m_pCfg)->InitFile(param)){
				cfg_map_.insert(make_pair(cfgKey, m_pCfg));
			}
			else{
				std::runtime_error("初始化配置类失败（ini模式），参数为[" + param + "]");
				return false;
			}
			break;
		case eCfgDataType_DB:
			std::runtime_error("DB暂时不支持。");
			return false;
		default:
			std::runtime_error("初始化配置失败。未知的配置类型。");
			return false;
		}
		return true;
	}

	void Configurator::SaveCfg(const CfgKey &cfgKey){
		std::lock_guard<std::mutex> localLock(mutex_);
		void* m_pCfg = GetCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (cfg_type_){
				case eCfgDataType_Ini:
					((IniCfg*)m_pCfg)->SaveFile();
				default:
					break;
			}
		}
	}

	std::string Configurator::GetString(const CfgKey &cfgKey, const std::string &section, const std::string &key, const string &defaultValue){
		std::lock_guard<std::mutex> localLock(mutex_);
		void* m_pCfg = GetCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (cfg_type_){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->GetString(section, key, defaultValue);
				default:
					break;
			}
		}
		return defaultValue;
	}

	int Configurator::GetInt(const CfgKey &cfgKey, const std::string &section, const std::string &key, const int &defaultValue){
		std::lock_guard<std::mutex> localLock(mutex_);
		void* m_pCfg = GetCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (cfg_type_){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->GetInt(section, key, defaultValue);
				default:
					break;
			}
		}
		return defaultValue;
	}

	double Configurator::GetDouble(const CfgKey &cfgKey, const std::string &section, const std::string &key, const double &defaultValue){
		std::lock_guard<std::mutex> localLock(mutex_);
		void* m_pCfg = GetCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (cfg_type_){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->GetDouble(section, key, defaultValue);
				default:
					break;
			}
		}
		return defaultValue;
	}

	bool Configurator::SetString(const CfgKey &cfgKey, const std::string &section, const std::string &key, const std::string &value){
		std::lock_guard<std::mutex> localLock(mutex_);
		void* m_pCfg = GetCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (cfg_type_){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->SetString(section, key, value);
				default:
					break;
			}
		}
		return false;
	}

	bool Configurator::SetInt(const CfgKey &cfgKey, const std::string &section, const std::string &key, int value){
		std::lock_guard<std::mutex> localLock(mutex_);
		void* m_pCfg = GetCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (cfg_type_){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->SetInt(section, key, value);
				default:
					break;
			}
		}
		return false;
	}

	bool Configurator::SetDouble(const CfgKey &cfgKey, const std::string &section, const std::string &key, double value){
		std::lock_guard<std::mutex> localLock(mutex_);
		void* m_pCfg = GetCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (cfg_type_){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->SetDouble(section, key, value);
				default:
					break;
			}
		}
		return false;
	}

	void* Configurator::GetCfgPtr(const CfgKey &cfgKey){
		if(cfg_map_.find(cfgKey) != cfg_map_.end()){
			return cfg_map_.find(cfgKey)->second;
		}
		return NULL;
	}
}


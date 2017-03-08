/**********************************************************
 *  \file pub_cfg.cpp
 *  \brief
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "fr_public/pub_cfg.h"

#include "fr_public/pub_ini.h"

using namespace std;
using namespace boost;

namespace Universal{
	Configurator::Configurator()
		:m_CfgMap(),
		 m_DefaultCfgKey(""),
		 m_CfgType(eCfgDataType_Unknow),
		 m_Mutex()
	{
		;
	}

	Configurator::~Configurator(){ 
		mutex::scoped_lock localLock(m_Mutex);
		for(auto iterCfg = m_CfgMap.begin(); iterCfg != m_CfgMap.begin(); ++iterCfg){
			if(iterCfg->second != NULL){
				switch(m_CfgType){
					case eCfgDataType_Ini:
						delete (IniCfg*)iterCfg->second; iterCfg->second = NULL;
						break;
					case eCfgDataType_DB:
					default:
						PUB_DEBUG_C("未知的配置格式。无法进行delete");
						break;
				}
			}
		}
	}

	bool Configurator::addCfg(const CfgKey &cfgKey, const std::string &param, const eCfgDataType &type){
		mutex::scoped_lock localLock(m_Mutex);
		if(m_CfgMap.find(cfgKey) != m_CfgMap.end()){
			PUB_DEBUG_I("该配置key已经存在，不能重复添加.");
		}

		m_CfgType = type;

		void* m_pCfg(NULL);
		switch (m_CfgType){
		case eCfgDataType_Ini:
			if (m_pCfg != NULL){
				delete (IniCfg*)m_pCfg; m_pCfg = NULL;
			}

			m_pCfg = new IniCfg();
			if (((IniCfg*)m_pCfg)->initFile(param)){
				m_CfgMap.insert(make_pair(cfgKey, m_pCfg));
			}
			else{
				PUB_DEBUG_E("初始化配置类失败（ini模式），参数为[" << param << "]");
				return false;
			}
			break;
		case eCfgDataType_DB:
			PUB_DEBUG_E("DB暂时不支持。");
			return false;
		default:
			PUB_DEBUG_E("初始化配置失败。未知的配置类型。");
			return false;
		}
		return true;
	}

	void Configurator::saveCfg(const CfgKey &cfgKey){
		mutex::scoped_lock localLock(m_Mutex);
		void* m_pCfg = getCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (m_CfgType){
				case eCfgDataType_Ini:
					((IniCfg*)m_pCfg)->saveFile();
				default:
					break;
			}
		}
	}

	std::string Configurator::getString(const CfgKey &cfgKey, const std::string &section, const std::string &key, const string &defaultValue){
		mutex::scoped_lock localLock(m_Mutex);
		void* m_pCfg = getCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (m_CfgType){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->getString(section, key, defaultValue);
				default:
					break;
			}
		}
		return defaultValue;
	}

	int Configurator::getInt(const CfgKey &cfgKey, const std::string &section, const std::string &key, const int &defaultValue){
		mutex::scoped_lock localLock(m_Mutex);
		void* m_pCfg = getCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (m_CfgType){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->getInt(section, key, defaultValue);
				default:
					break;
			}
		}
		return defaultValue;
	}

	double Configurator::getDouble(const CfgKey &cfgKey, const std::string &section, const std::string &key, const double &defaultValue){
		mutex::scoped_lock localLock(m_Mutex);
		void* m_pCfg = getCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (m_CfgType){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->getDouble(section, key, defaultValue);
				default:
					break;
			}
		}
		return defaultValue;
	}

	bool Configurator::setString(const CfgKey &cfgKey, const std::string &section, const std::string &key, const std::string &value){
		mutex::scoped_lock localLock(m_Mutex);
		void* m_pCfg = getCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (m_CfgType){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->setString(section, key, value);
				default:
					break;
			}
		}
		return false;
	}

	bool Configurator::setInt(const CfgKey &cfgKey, const std::string &section, const std::string &key, int32 value){
		mutex::scoped_lock localLock(m_Mutex);
		void* m_pCfg = getCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (m_CfgType){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->setInt(section, key, value);
				default:
					break;
			}
		}
		return false;
	}

	bool Configurator::setDouble(const CfgKey &cfgKey, const std::string &section, const std::string &key, double value){
		mutex::scoped_lock localLock(m_Mutex);
		void* m_pCfg = getCfgPtr(cfgKey);
		if(m_pCfg != NULL){
			switch (m_CfgType){
				case eCfgDataType_Ini:
					return ((IniCfg*)m_pCfg)->setDouble(section, key, value);
				default:
					break;
			}
		}
		return false;
	}

	void* Configurator::getCfgPtr(const CfgKey &cfgKey){
		if(m_CfgMap.find(cfgKey) != m_CfgMap.end()){
			return m_CfgMap.find(cfgKey)->second;
		}
		return NULL;
	}
}


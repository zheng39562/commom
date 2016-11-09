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

namespace Universal{
	Configurator::Configurator()
		:m_pCfg(NULL),
		m_CfgType(eCfgDataType_Unknow)
	{
		;
	}

	Configurator::~Configurator(){ ; }

	bool Configurator::initCfg(const std::string &param, const eCfgDataType &type){
		m_CfgType = type;

		switch (m_CfgType){
		case eCfgDataType_Ini:
			if (m_pCfg != NULL){
				delete (IniCfg*)m_pCfg; m_pCfg = NULL;
			}

			m_pCfg = new IniCfg();
			if (!((IniCfg*)m_pCfg)->initFile(param)){
				DEBUG_E("初始化配置类失败（ini模式），参数为[" << param << "]");
				return false;
			}
			break;
		case eCfgDataType_DB:
			DEBUG_E("DB暂时不支持。");
			return false;
		default:
			DEBUG_E("初始化配置失败。未知的配置类型。");
			return false;
		}
		return true;
	}

	std::string Configurator::getString(const std::string &section, const std::string &key, string defaultValue){
		switch (m_CfgType){
			case eCfgDataType_Ini:
				return ((IniCfg*)m_pCfg)->getString(section, key, defaultValue);
			default:
				break;
		}
		return defaultValue;
	}

	int Configurator::getInt(const std::string &section, const std::string &key, int defaultValue){
		switch (m_CfgType){
		case eCfgDataType_Ini:
			return ((IniCfg*)m_pCfg)->getInt(section, key, defaultValue);
		default:
			break;
		}
		return defaultValue;
	}

	double Configurator::getDouble(const std::string &section, const std::string &key, double defaultValue){
		switch (m_CfgType){
		case eCfgDataType_Ini:
			return ((IniCfg*)m_pCfg)->getDouble(section, key, defaultValue);
		default:
			break;
		}
		return defaultValue;
	}
}

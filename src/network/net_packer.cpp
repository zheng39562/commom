/**********************************************************
 *  \!file net_packer.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_packer.h"
namespace Network{
	//! \todo	暂时使用该标识来表明节点为集合
	const string COLLECTION_ROOT("root");
	const string PACKER_PATH_SPLIT("/");
	const string PACKER_PATH_CURRENT_INDEX(".");
	const string PACKER_PATH_LAST_INDEX("..");
	Packer::Packer(const ConnectKey &_key)
		:m_ConnectKey(_key)
		 eProtocolDataFormat m_DataFormat(eProtocolDataFormat_String),
		 m_pCollection(new Collection(COLLECTION_ROOT))
	{
		;
	}
	Packer::Packer(const Packer &ref){
		;
	}
	Packer& Packer::operator=(const Packer &ref){
		;
	}
	~Packer(){ 
		if(m_pCollection){
			delete m_pCollection; 
			m_pCollection = NULL;
		}
	}

	const ConnectKey& Packer::getConnectKey()const{
		return m_ConnectKey;
	}
	const eProtocolDataFormat& Packer::getDataFormat()const{
		return m_DataFormat;
	}

	bool Packer::parseMsg(const std::string &msg, const eProtocolDataFormat &dataFormat){
		bool bRet(false);
		switch(dataFormat){
			case eProtocolDataFormat_Json:
				bRet = m_pCollection->parseJs(msg);
				break;
			default:
				DEBUG_E("数据包暂不支持该格式，清期待后续版本。");
		}
		return bRet;
	}
	string Packer::getPackerStr(){
		switch(dataFormat){
			case eProtocolDataFormat_Json:
				return m_pCollection->toJson();
			default:
				DEBUG_E("数据包暂不支持该格式，清期待后续版本。");
		}
		return string("");
	}

	string Packer::getString(const Name &name)const{
		return m_pCollection->getString(name);
	}
	int Packer::getInt(const Name &name)const{
		return strToInt(getString(name));
	}
	double Packer::getDouble(const Name &name)const{
		return strToDouble(getString(name));
	}
	bool Packer::getBoolean(const Name &name)const{
		return (bool)strToInt(getString(name));
	}
	char Packer::getChar(const Name &name)const{
		string c(getString(name));
		return c.empty() ? '\0' : c[0];
	}
	bool Packer::addItem(const Name &name, const int &value){
		return m_pCollection->addItem(name, intToStr(value));
	}
	bool Packer::addItem(const Name &name, const double &value){
		return m_pCollection->addItem(name, doubleToStr(value));
	}
	bool Packer::addItem(const Name &name, const char &value){
		return m_pCollection->addItem(name, value);
	}
	bool Packer::addItem(const Name &name, const string &value){
		return m_pCollection->addItem(name, value);
	}
	bool Packer::addCollection(const CollectionName &name){
		return m_pCollection->addCollection(name);
	}

	bool Packer::toPath(const Path &path){
		vector<Name> names;
		if(parsePath(path, names)){
			Collection* pCollectionBak = m_pCollection;
			if(isAbsolutePath(path)){
				while(m_pCollection->parent() != NULL){
					m_pCollection = m_pCollection->parent();
				}
			}

			for(vector<Name>::const_iterator citerName = names.begin(); citerName != names.end(); ++citerName){
				if(*citerName == PACKER_PATH_CURRENT_INDEX){
					continue;
				}
				if(m_pCollection->collectionExist(*citerName)){
					if(*citerName == PACKER_PATH_LAST_INDEX){
						m_pCollection = m_pCollection->parent();
					}
					else{
						m_pCollection = m_pCollection->child(*citerName);
					}
				}
				else{
					m_pCollection = pCollectionBak;
					return false;
				}
			}
		}
		else{
			return false;
		}
		return true;
	}
	string Packer::curPath(){
		if(m_pCollection->parent() == NULL && m_pCollection->getName() == COLLECTION_ROOT){
			return PACKER_PATH_SPLIT;
		}

		string path(PACKER_PATH_SPLIT + m_pCollection->getName());
		const Collection* parent = m_pCollection->parent();
		while(parent != NULL){
			path = PACKER_PATH_SPLIT + parent->getName() + parent;
			parent = parent->parent();
		}
		return path;
	}

	bool Packer::isAbsolutePath(const Path &path){
		return path.empty() ? false : path[0] == PACKER_PATH_SPLIT;
	}

	bool Packer::parsePath(const Path &path, vector<Name> names){
		if(path.empty()){
			return false;
		}
		if(path[0] != '.' && path[0] != '/'){
			return false;
		}
		names.clear():
		return splitString(path, PACKER_PATH_SPLIT, names);
	}

	void Packer::getItemNames(vector<Name> &names){
		m_pCollection->getItemNames(names);
	}

	void Packer::getCollectionNames(vector<Name> &names){
		m_pCollection->getCollectionNames(names);
	}
} // namespace Network{


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
	const string PACKER_COLLECTION_DENTIFICATION("!@#$%^&*()collection!@#$)(*&^%$!@#");
	Packer::Packer(const ConnectKey &_key)
		:m_ConnectKey(_key)
		 eProtocolDataFormat m_DataFormat(eProtocolDataFormat_String),
		 m_Coolection()
	{
		;
	}
	Packer::Packer(const Packer &ref){
		;
	}
	Packer& Packer::operator=(const Packer &ref){
		;
	}
	~Packer(){ ; }

	const ConnectKey& Packer::getConnectKey()const{
		return m_ConnectKey;
	}
	const eProtocolDataFormat& Packer::getDataFormat()const{
		return m_DataFormat;
	}

	bool Packer::parseMsg(const std::string &msg, const eProtocolDataFormat &dataFormat){
		;
	}
	string Packer::getPackerStr(){
		;
	}

	string Packer::getString(const PackerKey &packerKey)const{
		;
	}
	int Packer::getInt(const PackerKey &packerKey)const{
		;
	}
	double Packer::getDouble(const PackerKey &packerKey)const{
		;
	}
	bool Packer::getBoolean(const PackerKey &packerKey)const{
		;
	}
	char Packer::getChar(const PackerKey &packerKey)const{
		;
	}
	bool Packer::addItem(const ItemName &name, const int &value){
		;
	}
	bool Packer::addItem(const ItemName &name, const char &value){
		;
	}
	bool Packer::addItem(const ItemName &name, const double &value){
		;
	}
	bool Packer::addItem(const ItemName &name, const char &value){
		;
	}
	bool Packer::addItem(const ItemName &name, const string &value){
		;
	}
	bool Packer::addCollection(const CollectionName &name){
		;
	}

	bool Packer::cd(const std::string &path){
		;
	}
	string Packer::curPath(){
		;
	}
	void Packer::getItemNames(vector<ItemName> &itemNames){
		;
	}
	bool Packer::isCollection(){
		;
	}
} // namespace Network{


/**********************************************************
 *  \!file net_collection.cpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#include "net_collection.h"

#include "tool/string_util.h"

namespace Network{
	#define CONVERT_JSON_QUOTED(content) doubleQuotedStr(strToJsonStr(content));

	using namespace std;
	using namespace Universal;

	Collection::Collection(const Name &_name)
		:m_Name(_name),
		 m_pParent(NULL),
		 m_Items(),
		 m_Collection()
	{
		;
	}
	Collection::Collection(const Name &_name, Collection* _pParent)
		:m_Name(_name),
		 m_pParent(_pParent),
		 m_Items(),
		 m_Collection()
	{
		;
	}
	Collection::Collection(const Collection &ref){
		copyRef(ref);
	}
	Collection& Collection::operator=(const Collection &ref){
		copyRef(ref);
		return *this;
	}
	Collection::~Collection(){ 
		for(map<Name, Collection*>::iterator iter = m_Collection.begin(); iter != m_Collection.end(); ++iter){
			delete iter->second;
			iter->second = NULL;
		}
	}


	std::string Collection::getString(const Name &name)const{
		if(m_Items.find(name) != m_Items.end()){
			return m_Items.find(name)->second;
		}
		DEBUG_D("没有找到对应的name。");
		return "";
	}

	bool Collection::itemExist(const Name &name)const{
		return m_Items.find(name) != m_Items.end();
	}

	bool Collection::collectionExist(const Name &name)const{
		return m_Collection.find(name) != m_Collection.end();
	}

	bool Collection::addCollection(const Name &name){
		if(collectionExist(name)){
			DEBUG_D("该集合名已存在。添加集合失败。");
			return false;
		}

		m_Collection.insert(pair<Name, Collection*>(name, new Collection(name, this)));
		return true;
	}
	bool Collection::addCollection(Collection *pCollection){
		if(pCollection == NULL){
			DEBUG_E("不允许添加未创建的集合指针(指针为NULL)。");
			return false;
		}
		if(collectionExist(pCollection->getName())){
			DEBUG_D("该集合名已存在。添加集合失败。");
			return false;
		}
		if(pCollection->parent() == NULL){
			pCollection->parent() = this;
			m_Collection.insert(pair<Name, Collection*>(pCollection->getName(), pCollection));
		}
		else{
			pCollection->parent()->moveChild(pCollection->getName(), this);
		}
		
		return true;
	}

	bool Collection::addItem(const Name &name, const CollectionItem &item){
		if(itemExist(name)){
			DEBUG_D("该名称已存在。添加节点失败。");
			return false;
		}

		m_Items.insert(pair<Name, CollectionItem>(name, item));
		return true;
	}


	bool Collection::updateItem(const Name &name, const CollectionItem &item){
		if(itemExist(name)){
			m_Items.find(name)->second = item;
		}
		else{
			m_Items.insert(pair<Name, CollectionItem>(name, item));
		}

		return true;
	}

	bool Collection::updateItemName(const Name &oldName, const Name &newName){
		if(!itemExist(oldName)){
			DEBUG_D("节点" << oldName << "不存在。修改节点名失败。");
			return false;
		}
		if(itemExist(newName)){
			DEBUG_D("节点" << newName << "已存在。不能将节点" << oldName << " 修改为节点" << newName);
			return false;
		}

		m_Items.insert(pair<Name, CollectionItem>(newName, m_Items.find(oldName)->second));
		m_Items.erase(m_Items.find(oldName));
		return true;
	}

	bool Collection::updateCollectionName(const Name &oldName, const Name &newName){
		if(!collectionExist(oldName)){ 
			DEBUG_D("集合" << oldName << "不存在。修改集合名失败。");
			return false;
		}
		if(collectionExist(newName)){ 
			DEBUG_D("集合" << newName << "已存在。不能将集合" << oldName << " 修改为集合" << newName);
			return false;
		}

		m_Collection.insert(pair<Name, Collection*>(newName, m_Collection.find(oldName)->second));
		m_Collection.erase(m_Collection.find(oldName));
		return true;
	}

	bool Collection::delItem(const Name &name){
		if(itemExist(name)){
			m_Items.erase(m_Items.find(name));
		}
		return true;
	}

	bool Collection::delCollection(const Name &name){
		if(collectionExist(name)){
			map<Name, Collection*>::iterator iter = m_Collection.find(name);
			delete iter->second;
			iter->second = NULL;
			m_Collection.erase(iter);
		}
		return true;
	}

	bool Collection::moveChild(const Name &name, Collection *pNewParent){
		Collection* pChild = this->child(name);
		if(pChild == NULL){
			DEBUG_D("找不到对应的集合[" << name << "]。");
			return false;
		}

		m_Collection.erase(m_Collection.find(name));

		pChild->parent() = NULL;
		pNewParent->addCollection(pChild);
		return true;
	}

	Collection* Collection::child(const Name &name){
		Collection* pChild(NULL);
		if(collectionExist(name)){
			pChild = m_Collection.find(name)->second;
		}
		return pChild;
	}
	const Collection* Collection::child(const Name &name)const{
		const Collection* pChild(NULL);
		if(collectionExist(name)){
			pChild = m_Collection.find(name)->second;
		}
		return pChild;
	}

	void Collection::getItemNames(vector<Name> &names){
		names.clear();
		names.reserve(m_Items.size());
		for(map<Name, CollectionItem>::const_iterator citer = m_Items.begin(); citer != m_Items.end(); ++citer){
			names.push_back(citer->first);
		}
	}

	void Collection::getCollectionNames(vector<Name> &names)const{
		names.clear();
		names.reserve(m_Collection.size());
		for(map<Name, Collection*>::const_iterator citer = m_Collection.begin(); citer != m_Collection.end(); ++citer){
			names.push_back(citer->first);
		}
	}

	bool Collection::isArray()const{
		bool bIsArray(false);
		if(itemSize()==0 && collectionSize() != 0){
			bIsArray = true;
			vector<Name> names;
			getCollectionNames(names);
			for(int i=0; i<collectionSize(); ++i){
				if(i != strToInt(names.at(i))){
					bIsArray = false;
				}
			}
		}
		return bIsArray;
	}

	string Collection::toJson()const{
		return toJson(*this);
	}

	string Collection::toXml(){
		string xmlString;
		return xmlString;
	}

	string Collection::toJson(const Collection &collection)const{
		string jsString;
		if(collection.isEmpty()){
			jsString += CONVERT_JSON_QUOTED(m_Name) + ":" + CONVERT_JSON_QUOTED("");
		}
		else if(collection.isArray()){
			jsString += "[";

			string jsCollection("");
			for(int i=0; i<collection.collectionSize(); ++i){
				if(!jsCollection.empty()){
					jsCollection += ",";
				}
				jsCollection += toJson(*(collection.child(intToStr(i))));
			}
			jsString += jsCollection;

			jsString += "]";
		}
		else{
			jsString += "{";

			string jsItem("");
			for(map<Name, CollectionItem>::const_iterator citerItem = m_Items.begin(); citerItem != m_Items.end(); ++citerItem){
				if(!jsItem.empty()){
					jsItem += ",";
				}
				jsItem += CONVERT_JSON_QUOTED(citerItem->first) + ":" + CONVERT_JSON_QUOTED(citerItem->second);
			}

			string jsCollection("");
			for(map<Name, Collection*>::const_iterator citerCollection = m_Collection.begin(); citerCollection != m_Collection.end(); ++citerCollection){
				if(!jsCollection.empty()){
					jsCollection += ",";
				}
				if(citerCollection->second == NULL){
					DEBUG_E("collection结构异常，存在为NULL的子节点。请检查相关操作是否正确。");
					return "";
				}
				jsCollection += toJson(*(citerCollection->second));
			}

			jsString += jsItem;
			jsString += jsItem.empty() ? jsCollection : "," + jsCollection;
			jsString += "}";
		}
		return jsString;
	}

	bool Collection::parseJs(const string &jsString){
		Json::Value jsValue;
		if(JsonTool::parseJs(jsString, jsValue)){
			return parseJs(*this, jsValue);
		}
		else{
			return false;
		}
		return true;
	}

	bool Collection::parseJs(const Collection &collection, const Json::Value jsValue){
		if(jsValue.isArray()){
			for(int i=0; i<jsValue.size(); ++i){
				Name name(intToStr(i));
				Collection* pCollection(new Collection(name));
				parseJs(*pCollection, jsValue[i]);
				addCollection(pCollection);
			}
		}
		else if(jsValue.isObject()){
			Json::Value::Members vKeys = jsValue.getMemberNames();
			for(Json::Value::Members::const_iterator iter = vKeys.begin(); iter != vKeys.end(); ++iter){
				if(jsValue[*iter].isArray() || jsValue[*iter].isObject()){
					Name name(*iter);
					Collection* pCollection(new Collection(name));
					parseJs(*pCollection, jsValue[*iter]);
					addCollection(pCollection);
				}
				else{
					addItem(*iter, JsonTool::getJsString(jsValue, *iter));
				}
			}
		}
		else{
			DEBUG_E("结构错误。传入非对象/非数组类型的JSON。");
			return false;
		}

		return true;
	}

	bool Collection::parseXml(const string &xmlString){
		;
	}

	void Collection::copyRef(const Collection &ref){
		m_Name = ref.getName();
		m_pParent = NULL;

		vector<Name> names;
		getItemNames(names);
		for(vector<Name>::const_iterator citer = names.begin(); citer != names.end(); ++citer){
			addItem(*citer, ref.getString(*citer));
		}

		getCollectionNames(names);
		for(vector<Name>::const_iterator citer = names.begin(); citer != names.end(); ++citer){
			const Collection* pChild = ref.child(*citer);
			if(pChild == NULL){
				addCollection(*citer);
			}
			else{
				addCollection(new Collection(*pChild));
			}
		}
	}
} // namespace Universal{


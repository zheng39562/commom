/**********************************************************
 *  \!file net_collection.h
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _net_collection_H
#define _net_collection_H

#include <string>
#include "network/net_define.h"
#include "tool/json_tool.h"

namespace Network{
	typedef std::string CollectionItem;
	//! \brief	集合类。
	//! \note	类分为集合(collection)和项目(item).集合中能包含collection and item 
	//! \note	该类保护行为较少，数据操作请谨慎。
	class Collection {
		public:
			Collection(const Name &_name);
			Collection(const Name &_name, Collection* _pParent);
			//! \note	复制子节点内容，父节点置空值。
			Collection(const Collection &ref);
			//! \note	复制子节点内容，父节点置空值。
			Collection& operator=(const Collection &ref);
			virtual ~Collection();
		public:
			inline const Name& getName()const{ return m_Name; }
			std::string getString(const Name &name)const;
			//! \brief
			bool itemExist(const Name &name)const;
			bool collectionExist(const Name &name)const;
			//! \brief	
			bool addCollection(const Name &name);
			bool addCollection(Collection *pCollection);
			bool addItem(const Name &name, const CollectionItem &item);
			//! \brief	
			inline void updateName(const Name &name){ m_Name = name; }
			bool updateItem(const Name &name, const CollectionItem &item);
			bool updateItemName(const Name &oldName, const Name &newName);
			bool updateCollectionName(const Name &oldName, const Name &newName);
			//! \brief	
			bool delItem(const Name &name);
			bool delCollection(const Name &name);
			//! \brief
			bool moveChild(const Name &name, Collection *pNewParent);

			inline bool isEmpty()const{ return m_Items.empty() && m_Collection.empty(); }

			//! \brief	父节点获取。
			//! \note	不能在外界直接进行删除操作。
			//! \note	任意修改都会直接影响到相关节点。修改时请慎重。
			//! \note	如果想要修改节点的挂载，建议使用moveChild函数。该函数直接修改父节点会导致多重引用。
			inline Collection*& parent(){ return m_pParent; }
			//! \brief	子节点获取。
			//! \note	不能在外界直接进行删除操作。
			//! \note	任意修改都会直接影响到相关节点。
			Collection* child(const Name &name);
			const Collection* child(const Name &name)const;
			//! \brief	
			void getItemNames(vector<Name> &names);
			void getCollectionNames(vector<Name> &names)const;

			inline size_t itemSize()const{ return m_Items.size(); }
			inline size_t collectionSize()const{ return m_Collection.size(); }

			//! \brief	是否是个数组i
			//! \note	初衷用于转换格式使用，不推荐使用该函数来做循环获取。
			bool isArray()const;
			//! \brief	将集合数据转化成json字符串
			string toJson()const;
			//! \brief	将集合数据转化成xml字符串
			string toXml();
			//! \brief	解析json字符并保存成collection结构。需要注意，该函数会清空collection中的数据。
			bool parseJs(const string &jsString);
			//! \brief	解析xml字符并保存成collection结构。需要注意，该函数会清空collection中的数据。
			bool parseXml(const string &xmlString);
		private:
			string toJson(const Collection &collection)const;
			bool parseJs(const Collection &collection, const Json::Value jsValue);

			void copyRef(const Collection &ref);
		private:
			Name m_Name;
			Collection* m_pParent;
			map<Name, CollectionItem> m_Items;
			map<Name, Collection*> m_Collection;
	};
}

#endif 


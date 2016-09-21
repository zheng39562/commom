/**********************************************************
 *  \!file template_tree.hpp
 *  \!brief
 *  \!note	注意事项： 
 * 			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。 
 * 			2,第1条的规则同样适用于返回值的含义。 
 * 
 * \!version 
 * * \!author zheng39562@163.com
**********************************************************/
#ifndef _template_tree_H
#define _template_tree_H

#include <cassert>

namespace Universal{
	//! \todo	真正的迭代器。
	//! \todo	const function
	//! \todo	throw 替换 assert
	template<typename TKey, typename TValue>
		class KeyTree{
			public:
				typedef KeyTree* iterator;
				typedef KeyTree const * const_iterator;
				const iterator ITERATOR_END = NULL:
			public:
				KeyTree();
				KeyTree(const KeyTree &ref);
				KeyTree& operator=(const KeyTree &ref);
				~KeyTree();
			public:
				void setKey(const TKey &key);
				inline void setValue(const TValue &value);
				inline void setParent(iterator &iter);
				inline TKey getKey();
				inline TValue getValue();
				inline iterator getParent();

				void addChild(const TKey &key);
				void addChild(const TKey &key, const TValue &value);
				void addChild(const KeyTree &keyTree);
				void delChild(const TKey);

				iterator find(const TKey &key);
				iterator begin();
				//const_iterator begin()const;
				iterator end();
				//const_iterator end()const;

			private:
				TKey m_Key;
				TValue m_Value;
				iterator m_Parent;
				map<TKey, iterator> m_Child;
		};
	template<TKey, TValue> KeyTree<TKey, TValue>::KeyTree(){
		;
	}
	template<TKey, TValue> KeyTree<TKey, TValue>::KeyTree(const KeyTree &ref){
		;
	}
	template<TKey, TValue> KeyTree<TKey, TValue>::operator=(const KeyTree &ref){
		;
	}
	template<TKey, TValue> KeyTree<TKey, TValue>::~KeyTree(){
		;
	}
	template<TKey, TValue> void KeyTree<TKey, TValue>::setKey(const TKey &key){ 
		if(m_Parent != ITERATOR_END){
			assert(m_Parent->find(key) != m_Parent->end());

			m_Parent->delChild(key);
		}
		m_Key = key; 
		m_Parent->addChild(*this);
	}
	template<TKey, TValue> void KeyTree<TKey, TValue>::setValue(const TValue &value){ m_Value = value ; }
	template<TKey, TValue> void KeyTree<TKey, TValue>::setParent(const KeyTree &parent){ m_Parent = &parent; }

	template<TKey, TValue> Tkey KeyTree<TKey, TValue>::getKey(){ return m_Key; }
	template<TKey, TValue> TValue KeyTree<TKey, TValue>::getValue(){ return m_Value; }
	template<TKey, TValue> iterator KeyTree<TKey, TValue>::getParent(){ return m_Parent; }

	template<TKey, TValue> void KeyTree<TKey, TValue>::addChild(const TKey &key){ 
		iterator iter(new KeyTree()); 
		iter->setKey(key);
		m_Child.insert(pair<TKey, iterator>(key, iter));
	}
	template<TKey, TValue> void KeyTree<TKey, TValue>::addChild(const TKey &key, const TValue &value){ 
		; 
	}
	template<TKey, TValue> void KeyTree<TKey, TValue>::addChild(const KeyTree &keyTree){ 
		; 
	}

	template<TKey, TValue> iterator KeyTree<TKey, TValue>::parent(){ return m_Parent; }

	template<TKey, TValue> iterator KeyTree<TKey, TValue>::find(const TKey &key){
		iterator iter(NULL);
		if(m_Child.find(key) != m_Child.end()){
			iter = m_Child.find(key)->second;
		}
		return iter;
	}
	template<TKey, TValue> iterator KeyTree<TKey, TValue>::begin(){
		iterator iter(NULL);
		if(!m_Child.empty()){
			iter = m_Child.begin(key)->second;
		}
		return iter;
	}
	template<TKey, TValue> iterator KeyTree<TKey, TValue>::end(){ return NULL; }
}
#endif 


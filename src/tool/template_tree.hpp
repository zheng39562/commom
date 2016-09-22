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
				typedef const KeyTree* const_iterator;
				const iterator ITERATOR_END = NULL:
			public:
				KeyTree();
				KeyTree(const TKey &_key);
				KeyTree(const TKey &_key, const TValue &_value);
				//! \brief	拷贝和赋值都不复制除节点以外的信息。
				//! \note	节点关联性只能通过节点关联函数(setParent or oprtChild)来完成
				KeyTree(const KeyTree &ref);
				KeyTree& operator=(const KeyTree &ref);
				~KeyTree();
			public:
				void addChild(const TKey &key);
				void addChild(const TKey &key, const TValue &value);
				void addChild(const KeyTree &keyTree);

				void delChild(const TKey &key);

				void changeParent(KeyTree &parent);
				void changeChildKey(const TKey &origin, const TKey &dest);

				void unlinkParent();
				void unlinkChild(const KeyTree &child);

				iterator find(const TKey &key);
				iterator begin();
				//const_iterator begin()const;
				iterator end();
				//const_iterator end()const;

				//! \brief	不建议外间直接使用此函数获取子节点集。对此引用的操作会直接影响到树结构。
				//! \todo	此函数是由于addChild 和 setParent 会相互引用造成循环的折中办法。希望可以得到更好的解决。
				inline map<TKey, iterator>& child(){ return m_Child; }
				inline TValue& value(){ return m_Value; }
				inline const TValue& value()const{ return m_Value; }
				inline const TKey& key(){ return m_Key; }
				inline const TKey& key()const{ return m_Key; }
				inline const_iterator parent(){ return m_IterParent; }
				inline const_iterator parent()const{ return m_IterParent; }
			private:
				TKey m_Key;
				TValue m_Value;
				iterator m_IterParent;
				map<TKey, iterator> m_Child;
		};
	template<TKey, TValue> KeyTree<TKey, TValue>::KeyTree()
		:m_Key(),
		 m_Value(),
		 m_IterParent(ITERATOR_END),
		 m_Child()
	{ ; }
	template<TKey, TValue> KeyTree<TKey, TValue>::KeyTree(const TKey &_key)
		:m_Key(_key),
		 m_Value(),
		 m_IterParent(ITERATOR_END),
		 m_Child()
	{ ; }
	template<TKey, TValue> KeyTree<TKey, TValue>::KeyTree(const TKey &_key, const TValue &_value)
		:m_Key(_key),
		 m_Value(_value),
		 m_IterParent(ITERATOR_END),
		 m_Child()
	{ ; }
	template<TKey, TValue> KeyTree<TKey, TValue>::KeyTree(const KeyTree &ref){
		this->m_Key = ref.getKey();
		this->m_Value = ref.getValue();
		this->m_IterParent = ITERATOR_END;
		this->m_Child = map<TKey, iterator>;
	}
	template<TKey, TValue> KeyTree<TKey, TValue>::operator=(const KeyTree &ref){
		this->m_Key = ref.getKey();
		this->m_Value = ref.getValue();
		this->m_IterParent = ITERATOR_END;
		this->m_Child = map<TKey, iterator>;
	}
	template<TKey, TValue> KeyTree<TKey, TValue>::~KeyTree(){
		unlinkParent();

		for(map<string, iterator>::iterator iterChild = m_Child.begin(); iterChild = m_Child.begin(); ++iterChild){
			delete iterChild->second; iterChild->second = ITERATOR_END;
		}
	}

	template<TKey, TValue> void KeyTree<TKey, TValue>::addChild(const TKey &key){ 
		iterator iter(new KeyTree(key)); 
		addChlid(*iter);
	}
	template<TKey, TValue> void KeyTree<TKey, TValue>::addChild(const TKey &key, const TValue &value){ 
		iterator iter(new KeyTree(key, value)); 
		addChlid(*iter);
	}
	template<TKey, TValue> void KeyTree<TKey, TValue>::addChild(KeyTree &keyTree){ 
		keyTree.changeParent(*this);
	}

	template<TKey, TValue> void KeyTree<TKey, TValue>::delChild(const TKey &key){ 
		map<string, iterator>::iterator iterChild = m_Child.find(origin);
		if(iterChild != m_Child.end()){
			delete iterChild->second; iterChild->second = ITERATOR_END;
			m_Child.erase(iterChild);
		}
	}

	template<TKey, TValue> void iterator KeyTree<TKey, TValue>::changeParent(KeyTree &parent){ 
		unlinkParent();
		m_IterParent = &parent;
		m_IterParent->child().insert(pair<TKey, iterator>(m_Key, this));
	}

	template<TKey, TValue> void iterator KeyTree<TKey, TValue>::changeChildKey(const TKey &origin, const TKey &dest){ 
		map<string, iterator>::iterator iterChild = m_Child.find(origin);
		if(iterChild != m_Child.end()){
			m_Child.insert(pair<string, iterator>(dest, iterChild->second));
			m_Child.erase(iterChild);
		}
		else{
			DEBUG_E("没有找到对应的key");
		}
	}

	template<TKey, TValue> void iterator KeyTree<TKey, TValue>::unlinkParent(){ 
		if(m_IterParent != ITERATOR_END ){
			m_IterParent->unlinkChild(*this);
			m_IterParent = ITERATOR_END;
		}
	}
	template<TKey, TValue> void iterator KeyTree<TKey, TValue>::unlinkChild(const KeyTree &child){ 
		map<string, iterator>::iterator iterChild = m_Child.find(origin);
		if(iterChild != m_Child.end()){
			m_Child.erase(iterChild);
		}
		else{
			DEBUG_E("没有找到对应的key");
		}
	}

	template<TKey, TValue> iterator KeyTree<TKey, TValue>::find(const TKey &key){
		iterator iter(ITERATOR_END);
		if(m_Child.find(key) != m_Child.end()){
			iter = m_Child.find(key)->second;
		}
		return iter;
	}
	template<TKey, TValue> iterator KeyTree<TKey, TValue>::begin(){
		iterator iter(ITERATOR_END);
		if(!m_Child.empty()){
			iter = m_Child.begin(key)->second;
		}
		return iter;
	}
	template<TKey, TValue> iterator KeyTree<TKey, TValue>::end(){ return ITERATOR_END; }

}
#endif 


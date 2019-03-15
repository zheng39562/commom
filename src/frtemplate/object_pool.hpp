/**********************************************************
 *  \file object_pool.hpp
 *  \brief
 *  \note	注意事项： 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#ifndef _object_pool_H
#define _object_pool_H

#include <map>
#include <list>
#include <exception>

namespace frtemplate{


	enum ObjectPoolError{
		Err_Default = 0,
		Err_Full = 1 
	};
	/**
	 * 对象池.
	 *	仅用于对象操作(Fetch/Recover)非常频繁的场景.
	 *	操作频率不高的场合使用此方案，可能存在大量内存无效占用的情况(一直在池内没有使用，也不释放)
	 *	频率频繁定义:大于1次操作/秒的场景(每小时最少3600次操作)
	 *
	 * 功能列表：
	 *	1. 可伸缩: 每次回收进行一次检查.
	 *		回收算法思路: 每次Fetch/Recover之后，都会重置到原点，然后进行操作计算.
	 *			1. Fetch: 只有在没有free时才会new
	 *			2. Recover: 设定操作比例.当比例超过设定系数(百分比)会进行一次回收.
	 *			3. 进行new/delete后，会重置比例为50%，以当前[可用对象数]为基数.
	 *			4. 回收比例自行设定.
	 *		X 基数, Y 增量, a回收系数.
	 *		a = y / (2 * x + y) <==> y = 2 * a * x / (1 - a)
	 *		
	 *
	 * 注意点.  
	 *	1. 无锁，多线程使用需自行加锁.  
	 *	2. 对象传出为指针，需要被回收。否则会内存泄漏.
	 *	3. 回收数量不可超过最大数量，否则会自动设置为最大数量.
	 */
	typedef unsigned long TObjectAddr;
	template < typename TObject >
	class ObjectPool {
		public:
			/**
			 * 默认模式:最大值的1/4会开始回收，回收系数为0.2
			 * TODO: 随便写的默认值,后续根据经验再进行微调.
			 */
			ObjectPool(int _max_object_size);
			ObjectPool(int _min_free_object_size_, int _max_object_size, int _recover_percent, int _active_recover_fator);
			~ObjectPool();

		public:
			TObject* Fetch();
			void Recovery(TObject* obj);

			std::string GetPoolInfo();

			inline ObjectPoolError Error(){ return error_; }
			inline int GetUsedObjectSize(){ return addr_2used_objects_.size(); }
		private:
			inline int GetFetchFactor(){
				return fetch_times_ > recover_times_ ? ((fetch_times_ - recover_times_) * 100 / (fetch_times_ + recover_times_)) : 0;
			}
			inline int GetRecoverFactor(){
				return recover_times_ > fetch_times_ ? ((recover_times_ - fetch_times_) * 100 / (fetch_times_ + recover_times_)) : 0;
			}

			inline bool isUsedObject(TObjectAddr addr){ return addr_2used_objects_.find(addr) != addr_2used_objects_.end(); }

			inline void resetOprtCount(){
				recover_times_ = free_objects_.size();
				fetch_times_ = free_objects_.size();
			}

			void CheckAndPerformRecover();

			int CalCreateObjectSize();
			int CalRecoverObjectSize();

		private:
			int active_recover_fator_;
			int recover_percent_;
			int recover_times_;
			int fetch_times_;

			int min_free_object_size_;
			int max_object_size_;

			std::list<TObject*> free_objects_;
			std::map<TObjectAddr, TObject*> addr_2used_objects_;

			ObjectPoolError error_;
	};

	template < typename TObject >
	ObjectPool<TObject>::ObjectPool(int _max_object_size)
		:active_recover_fator_(20), 
		 recover_percent_(20),
		 recover_times_(0), 
		 fetch_times_(0), 
		 min_free_object_size_(_max_object_size / 4),
		 max_object_size_(_max_object_size), 
		 free_objects_(),
		 addr_2used_objects_(),
		 error_(Err_Default)
	{ ; }

	template < typename TObject >
	ObjectPool<TObject>::ObjectPool(int _min_free_object_size_, int _max_object_size, int _recover_percent, int _active_recover_fator)
		:active_recover_fator_((0 <= _active_recover_fator && _active_recover_fator <= 100) ? _active_recover_fator : 20),
		 recover_percent_((0 <= _recover_percent && _recover_percent <= 100) ? _recover_percent : 20),
		 recover_times_(0), 
		 fetch_times_(0), 
		 min_free_object_size_(_min_free_object_size_),
		 max_object_size_(_max_object_size), 
		 free_objects_(),
		 addr_2used_objects_(),
		 error_(Err_Default)
	{ 
	}

	template < typename TObject >
	ObjectPool<TObject>::~ObjectPool(){
		if(!addr_2used_objects_.empty()){
			throw std::logic_error("some object point are used.Can not disconstruction.");
		}
		for(TObject* obj : free_objects_){
			delete obj;
		}
		free_objects_.clear();
	}

	template < typename TObject >
	TObject* ObjectPool<TObject>::Fetch(){
		if(free_objects_.empty()){
			int create_num = CalCreateObjectSize(); 
			if(create_num <= 0){
				return NULL;
			}

			for(int index = 0; index < create_num; ++index){
				free_objects_.push_back(new TObject());
			}
			resetOprtCount();
		}

		TObject* obj = free_objects_.front();

		TObjectAddr addr = (TObjectAddr)obj;
		if(isUsedObject(addr)){
			throw std::logic_error("addr [" + std::to_string(addr) + "]is exist.");
			return NULL;
		}

		addr_2used_objects_.insert(std::make_pair(addr, obj));
		free_objects_.pop_front();

		++fetch_times_;
		return obj;
	}

	template < typename TObject >
	void ObjectPool<TObject>::Recovery(TObject* obj){
		if(obj == NULL){
			return;
		}

		TObjectAddr addr = (TObjectAddr)obj;
		if(!isUsedObject(addr)){
			throw std::logic_error("Point [" + std::to_string(addr) + "] is not a pool point.");
			return;
		}

		free_objects_.push_back(obj);
		addr_2used_objects_.erase(addr);
		++recover_times_;

		CheckAndPerformRecover();
	}

	template < typename TObject >
	std::string ObjectPool<TObject>::GetPoolInfo(){
		return std::string("")
			+ " active_recover_fator_ : " + std::to_string(active_recover_fator_)
			+ " recover_percent_ : " + std::to_string(recover_percent_)
			+ " recover_times_ : " + std::to_string(recover_times_)
			+ " fetch_times_ : " + std::to_string(fetch_times_)
			+ " min_free_object_size_ : " + std::to_string(min_free_object_size_)
			+ " max_object_size_ : " + std::to_string(max_object_size_)
			+ " free_objects_size : " + std::to_string(free_objects_.size())
			+ " addr_2used_objects_size : " + std::to_string(addr_2used_objects_.size())
			;
	}

	template < typename TObject >
	void ObjectPool<TObject>::CheckAndPerformRecover(){
		int recover_num = CalRecoverObjectSize();
		if(recover_num > 0){
			auto iter = free_objects_.begin();
			while(--recover_num >= 0 && iter != free_objects_.end()){
				delete *iter;
				iter = free_objects_.erase(iter);
			}

			resetOprtCount();
		}
	}

	template < typename TObject >
	int ObjectPool<TObject>::CalCreateObjectSize(){
		if(!free_objects_.empty()){ return 0; }
		if(addr_2used_objects_.empty() && free_objects_.empty()){ return min_free_object_size_; }

		int max_create_num = max_object_size_ - addr_2used_objects_.size();
		if(max_create_num <= 0){
			error_ = Err_Full;
			return 0;
		}

		int default_create_num = 20;
		int create_num = max_create_num;
		int fetchFactor = GetFetchFactor();
		if(fetchFactor > 0){
			create_num = max_create_num * fetchFactor / 100;
		}
		else{
			create_num = default_create_num;
		}

		create_num = create_num >= default_create_num ? create_num : default_create_num;
		return create_num >= max_create_num ? max_create_num : create_num;
	}

	template < typename TObject >
	int ObjectPool<TObject>::CalRecoverObjectSize(){
		int recover_num = 0;
		int max_recover_num = free_objects_.size() - min_free_object_size_;
		int recover_factor = GetRecoverFactor();
		if(max_recover_num > 0 && recover_factor >= active_recover_fator_){
			recover_num = free_objects_.size() * recover_percent_ / 100;
			recover_num = recover_num > max_recover_num ? max_recover_num : recover_num;
		}

		return recover_num;
	}

}

#endif 


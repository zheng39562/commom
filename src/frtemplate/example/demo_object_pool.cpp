/**********************************************************
 *  \file demo_object_pool.cpp
 *  \brief
 *  \note	注意事项： 
 * 
 * \version 
 * * \author zheng39562@163.com
**********************************************************/
#include "../object_pool.hpp"

#include <iostream>
#include <string>
#include <list>
#include <vector>

using namespace std;
using namespace frtemplate;

class ObjectA{
	public:
		ObjectA():num_(0){ ; }
		~ObjectA(){ ; }
	public:
		void set_num(int num){ num_ = num; }
		int num(){ return num_; }
	private:
		int num_;
};

void demoFetchMaxObject(ObjectPool<ObjectA> &pool, int max_object_size){
	vector<ObjectA*> tmp_array;
	tmp_array.reserve(max_object_size);

	for(int index = 0; index < max_object_size + 1; ++index){
		ObjectA* ptr = pool.Fetch();
		if(ptr == NULL){
			if(index == max_object_size){
				cout << __FUNCTION__ << ": Right" << endl;
			}
			else{
				cout << __FUNCTION__ << ": Error. (can not fetch max size) index : " << max_object_size << endl;
			}
		}
		else{
			tmp_array.push_back(ptr);
		}
	}

	for(ObjectA* ptr : tmp_array){
		pool.Recovery(ptr);
	}
}

int main(){
	int min_free_object_size = 100;
	int max_object_size = 1000;
	int recover_percent = 20;
	int active_recover_num = 20;
	ObjectPool<ObjectA> pool(min_free_object_size, max_object_size, recover_percent, active_recover_num);

	demoFetchMaxObject(pool, max_object_size);

	cout << pool.GetPoolInfo() << endl;

	return 0;
}


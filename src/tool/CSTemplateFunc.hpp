/**********************************************************
 * \file CSTemplateFunc.hpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSTemplateFunc_H
#define _CSTemplateFunc_H

#include "CSDefine.h"

/*
 * template function
 */
namespace Universal{
	//! \brief	封装map的find函数，避免溢出。
	//! \note	直接返回数据的重载函数 : 需要 T2 有默认构造函数。
	//! \todo	未考虑右值引用。(大部分的函数都未考虑右值引用。)
	template < typename T1, typename T2 >
		bool mapFind( const map<T1, T2> &mapVar, const T1 &key, T2 &value ){
			if( mapVar.find(key) != mapVar.end() ){
				value = mapVar.find(key)->second;
				return true;
			}
			return false;
		}
	template < typename T1, typename T2 >
		T2 mapFind( const map<T1, T2> &mapVar, const T1 &key ){
			T2 value;
			mapFind( mapVar, key, value );
			return value;
		}

	//! \brief	封装vector的at/[]函数。避免索引超出数据上限。
	//! \note	直接返回数据的重载函数 : 需要 T 有默认构造函数。
	template < typename T >
		bool vectorAt( const vector<T> &vecVar, const long &index, T &data ){
			if( vecVar.size() > index ){
				data = vecVar.at(index);
				return true;
			}
			return false;
		}
	template < typename T >
		T vectorAt( const vector<T> &vecVar, const long &index ){
			T data;
			vectorAt( vecVar, index, data );
			return data;
		}

} // namespace Universal{


#endif 


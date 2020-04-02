/**********************************************************
 * \file TemplateFunc.hpp
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _TemplateFunc_H
#define _TemplateFunc_H

#include <map>
#include <vector>

/*
 * template function
 */
namespace frtemplate{

	template < typename T >
	class FuncResult{
		public:
			FuncResult(T _info);
			FuncResult(std::string _err, T _info);
		public:
			inline bool success()const{ return err_.empty(); }
			inline const std::string& err()const{ return err_; }
			inline const T& info()const{ return info_; }
		private:
			std::string err_;
			T info_;
	};

	template < typename T >
	FuncResult<T>::FuncResult(T _info):
		err_(),
		info_()
	{ }
	template < typename T >
	FuncResult<T>::FuncResult(std::string _err, T _info):
		err_(_err),
		info_(_info)
	{ }

} // namespace Universal{


#endif 


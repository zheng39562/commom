/**********************************************************
 * \file logic/tool/XlsReader.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _XlsReader_H
#define _XlsReader_H

#include "libxls/xlsstruct.h"

#include "fr_public/pub_define.h"
#include "fr_xls/xls_struct.h"

using namespace std;

namespace Universal{
#define _XLSREADER_FILE_NOTEXIST				0x0001
#define _XLSREADER_PARSEFAILED				0x0002
	//! \brief	xls表的页数据。
	//! \note	此为中间层。( 相对于libxls)
	//! \note	包含数据结构解析和转换后数据的保存。
	//! \note	相对libxls而言，多了转储操作。所以时间上必然有增加。
	//! \todo	如果效率不理想，可考虑直接保存xlsWorkBook。但需要确认，长期持有此变量是否存在影响（如close函数时机等）
	//!			更改为xlsWorkBook的额外问题：当期望更改三方库时，不能仅修改parse函数，而是需要完全的重写类。			
	//! \todo	增加迭代器来访问sheet。
	class XlsReader{
		public:
			XlsReader();
			XlsReader( const string &_filePath );
			~XlsReader();
		private:
			XlsBookStruct						m_SheetArray;
		private:
			//! \brief	解析xlsWorkBook。
			void parseXls( const string &filePath );
		public:
			//! \brief	获取具体页。
			XlsSheet& getSheet( const long &indexOfSheet );
			//! \brief	获取整个XLS。
			XlsBookStruct& getXlsBook( );
			//! \brief	获得页数。
			long getCountOfSheet( ){ return m_SheetArray.size(); }
			bool read( const string &filePath );

	};
} // namespace Universal

#endif


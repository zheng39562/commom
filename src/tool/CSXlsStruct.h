/**********************************************************
 * \file logic/tool/CSXlsStruct.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSXlsStruct_H
#define _CSXlsStruct_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>

#include "libxls/xlsstruct.h"

#include "CSCommonTool.h"
#include "CSDefine.h"
#include "CSException.h"

using namespace std;
using namespace CSException;

namespace Universal{
	enum CSXlsColor{
		xlsColor_FirstInvalid = 0,
		xlsColor_Red = 1,
		xlsColor_Black,
		xlsColor_White,
		xlsColor_LastInvalid
	};
	struct CSXlsColorFormat{
		CSXlsColor backColor;
		CSXlsColor fontColor;
	};
	class SheetFormat{
		public:
			SheetFormat();
			~SheetFormat();
		private:
			map<unsigned long, CSXlsColorFormat>			m_ColumnFormat;		//! 0 based
			map<unsigned long, CSXlsColorFormat>			m_RowFormat;			//! 0 based
			CSXlsColor										m_DefaultBackColor;
			CSXlsColor										m_DefaultFontColor;
		private:
			void addColor( map<unsigned long, CSXlsColorFormat> &format,  const unsigned long &row, const CSXlsColorFormat &colorFormat );
		public:
			CSXlsColor getBackColor( const unsigned long &row, const unsigned long &column );
			CSXlsColor getFontColor( const unsigned long &row, const unsigned long &column );
			CSXlsColorFormat getColor( const unsigned long &row, const unsigned long &column );
			void setRowColor( const unsigned long &row, const CSXlsColorFormat &colorFormat );
			void setColumnColor( const unsigned long &column, const CSXlsColorFormat &colorFormat );
	};


	typedef vector<string>						CSXlsRowStruct;
	typedef list<CSXlsRowStruct>				CSXlsSheetStruct;
	//! \brief	xls表中的一个页数据类。
	//! \note	此为中间层。( 相对于libxls)
	//! \note	包含数据结构解析和转换后数据的保存。
	//! \note	相对libxls而言，多了转储操作。所以时间上必然有增加。
	//! \todo	作为xlsy页的完整封装（现在情况：拥有数据，但缺少操作）
	class CSXlsSheet{
		public:
			CSXlsSheet();
			CSXlsSheet( const xlsWorkSheet &_xlsSheet );
			~CSXlsSheet();
		private:
			long								m_MaxRows;				//! 最大行。
			long								m_MaxColumns;			//! 最大列。
			CSXlsSheetStruct					m_XlsContent;			//!	表格的内容。二维数组保存。
		private:
			//! \brief	解析xlsWorkSheet。
			//! \note	对unsinged char* -> char* 无法确实是否对所有的字符类型都是正确的。
			//!	\todo	需要确认强制转换的原理，保证转换的正确性。（目前经过测试的有：UTF-8格式的英文和中文）
			//! \todo	缺乏对doueble等浮点类型的正确转换：将强制转换为long。
			void parseXlsSheet( const xlsWorkSheet &xlsSheet );
		public:
			//! \brief	模拟迭代器，为当前类不完善的补全函数。
			CSXlsSheetStruct::iterator begin() { return m_XlsContent.begin(); }
			CSXlsSheetStruct::iterator end() { return m_XlsContent.end(); }
			CSXlsSheetStruct::const_iterator begin()const { return m_XlsContent.begin(); }
			CSXlsSheetStruct::const_iterator end()const { return m_XlsContent.end(); }

			//! \brief	获取具体数据。
			// const string& getContent( const long &row, const long &column )throw(CSLogException );
			//! \brief	获取一行的数据。
			// const CSXlsRowStruct& getRow( const long &row )throw(CSLogException );
			CSXlsSheetStruct& getXlsSheet() { return m_XlsContent; }
			long getRowCount(){  return m_MaxRows;  }
	};

	typedef vector<CSXlsSheet>					CSXlsBookStruct;
} // namespace Universal

#endif


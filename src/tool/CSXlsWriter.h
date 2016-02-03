/**********************************************************
 * \file logic/tool/CSXlsWriter.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _CSXlsWriter_H
#define _CSXlsWriter_H

#include "xlslib/xlslib.h"

#include "CSDefine.h"
#include "CSFile.h"
#include "CSCommonTool.h"
#include "CSException.h"
#include "CSXlsStruct.h"

using namespace std;
using namespace CSException;
using namespace xlslib_core;

namespace Universal{
	class CSXlsWriter{
		public:
			CSXlsWriter();
			~CSXlsWriter();
		private:
			color_name_t getXlsColor( const CSXlsColor &colorIndex );
		public:
			bool writeToFile( const string &filePath, const CSXlsBookStruct &xlsBook );
			bool writeToFile( const string &filePath, const CSXlsSheetStruct &xlsSheet, SheetFormat sheetFormat = SheetFormat() );
	};
} // namespace Universal

#endif


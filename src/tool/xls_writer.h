/**********************************************************
 * \file logic/tool/XlsWriter.h
 * \brief 
 * \note	注意事项：
 *			1,类中的成员函数中的同名参数的含义完全相同。仅会注释其中一个函数，其他函数则不再重复注释。重名的参数意义不同时，会独立注解。
 *			2,第1条的规则同样适用于返回值的含义。
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#ifndef _XlsWriter_H
#define _XlsWriter_H

#include "xlslib/xlslib.h"

#include "common_define.h"
#include "common_file.h"
#include "common_tool.h"
#include "common_exception.h"
#include "xls_struct.h"

using namespace std;
using namespace xlslib_core;

namespace Universal{
	class XlsWriter{
		public:
			XlsWriter();
			~XlsWriter();
		private:
			color_name_t getXlsColor( const XlsColor &colorIndex );
		public:
			bool writeToFile( const string &filePath, const XlsBookStruct &xlsBook );
			bool writeToFile( const string &filePath, const XlsSheetStruct &xlsSheet, SheetFormat sheetFormat = SheetFormat() );
	};
} // namespace Universal

#endif


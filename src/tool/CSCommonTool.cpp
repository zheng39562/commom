/**********************************************************
 * \file logic/tool/CSCommonTool.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSCommonTool.h"

#include "CSStrUtil.h"
#include "boost/regex.hpp"

using namespace boost;

/*
 * file path 
 */
namespace Universal{
	string getAbsPath( string filepath ){
		FILE* fp = NULL;
		string shellCmd = "cd " + filepath + ";pwd";
		if( (fp=popen(shellCmd.c_str(), "r" ) ) != NULL ){
			int arraynum = 300;
			char cpath[arraynum];
			// 去掉获取到的\n
			filepath = trimRight( fgets(cpath, arraynum, fp) );
			pclose( fp);  fp = NULL;
		}
		if( filepath.find_last_of("/") != filepath.size()-1 )
			filepath += "/";

		return filepath;
	}

	string getFileNameByPath( const string &filepath ){
		long folderIndex = filepath.find_last_of( '/', filepath.size() )+1;
		return filepath.substr( folderIndex, filepath.size()-folderIndex );
	}

	string completePath( const string &path ){
		if( !path.empty() && path[path.size()-1 ] != '/' ){
			return path + "/";
		}
		return path;
	}

}  // namepsace : Universal


/*
 * other function
 */
namespace Universal{
	//! \brief 计算当前对象占用的内存。用于计算保存string的类型。
	long calmemoryOfObject( const vector<string> &object ){
		string str( "1");
		long size = 0;
		for( vector<string>::const_iterator citer = object.begin(); citer != object.end(); ++citer ){
			size += sizeof( str[0]) * citer->size();
		}
		return size;
	}
	long calmemoryOfObject( const map<string, string> &object ){
		string str( "1");
		long size = 0;
		for( map<string, string>::const_iterator citer = object.begin(); citer != object.end(); ++citer ){
			size += sizeof( str[0]) * ( citer->first.size() + citer->second.size() );
		}
		return size;
	}
	long calmemoryOfObject( const vector<vector<string> > &object ){
		long size = 0;
		for( vector<vector<string> >::const_iterator citer = object.begin(); citer != object.end(); ++citer ){
			size += calmemoryOfObject( *citer );
		}
		return size;
	}
	long calmemoryOfObject( const vector<map<string, string> > &object ){
		long size = 0;
		for( vector<map<string, string> >::const_iterator citer = object.begin(); citer != object.end(); ++citer ){
			size += calmemoryOfObject( *citer );
		}
		return size;
	}
	void filterDuplicateValue( vector<string> &duplicateArray ){
		vector<string> newArray;
		bool isNewValue;
		for( vector<string>::const_iterator citerDuplicate = duplicateArray.begin(); citerDuplicate != duplicateArray.end(); ++citerDuplicate ){
			isNewValue = true;
			for( vector<string>::const_iterator citerSingleValue = newArray.begin(); citerSingleValue != newArray.end(); ++citerSingleValue ){
				if( *citerSingleValue == *citerDuplicate )
					isNewValue = false;
			}
			if( isNewValue )
				newArray.push_back( *citerDuplicate );
		}
		duplicateArray = newArray;
	}


	bool execShellCmd( const string &cmd ){
#ifdef COMMON_DEBUG
		DEBUG_D( "Exec shell command : [%s]", cmd.c_str() );
#endif
		FILE* fp = NULL;
		if( (fp=popen(cmd.c_str(), "r" ) ) != NULL ){
			pclose( fp);  fp = NULL;
			return true;
		}
		return false;
	}


	string getMobileType( const string &mobile ){
		string head1 = mobile.substr( 0, 1 );
		string head2 = mobile.substr( 0, 2 );
		string head3 = mobile.substr( 0, 3 );
		string head4 = mobile.substr( 0, 4 );

		if( head3 == "134" || head3 == "135" || head3 == "136" || head3 == "137" || head3 == "138" || head3 == "139" || head3 == "147" || head3 == "150" || 
			head3 == "151" || head3 == "152" || head3 == "157" || head3 == "158" || head3 == "159" || head3 == "182" || head3 == "183" || head3 == "184" || 
			head3 == "187" || head3 == "188" || head3 == "178" || 
			head4 == "1705"
				){
			return CS_PTHONETYPE_MOBILE;
		}
		else if( head3 == "130" || head3 == "131" || head3 == "132" || head3 == "155" || head3 == "156" || head3 == "185" || head3 == "186" ||
				 head4 == "1709" ){
			return CS_PTHONETYPE_UNICOM;
		}
		else if( head2 == "05" || head4 == "1700" ||
				 head3 == "133" || head3 == "153" || head3 == "180" || head3 == "181" || head3 == "189" || head3 == "169" || head3 == "177" ){
			return CS_PTHONETYPE_CDMA;
		}
		else{
			return CS_PTHONETYPE_QT;
		}
		return CS_STRINGFALSE;
	}


	string getSmsRoute( const string &mobile ){
		string head3 = mobile.substr( 0, 3 );
		string head4 = mobile.substr( 0, 4 );
		if( mobile.size() == 12 ){
			if( head3 == "057" || head4 == "1700" || head4 == "0580" )
				return CS_SMSROUTE_TEL;
		}
		if( head3 == "134" || head3 == "135" || head3 == "136" || head3 == "137" || head3 == "138" || head3 == "139" || head3 == "147" || 
			head3 == "150" || head3 == "151" || head3 == "152" || head3 == "157" || head3 == "158" || head3 == "159" || 
			head3 == "182" || head3 == "183" || head3 == "184" || head3 == "187" || head3 == "188" || head3 == "178" || 
			head4 == "1705" ){
			return CS_SMSROUTE_MO;
		}
		else if( head3 == "130" || head3 == "131" || head3 == "132" || head3 == "133" || head3 == "153" || head3 == "155" || head3 == "156" || 
				 head3 == "169" || head3 == "177" || head3 == "180" || head3 == "181" || head3 == "185" || head3 == "186" || head3 == "189" || 
				 head3 == "183" || head3 == "184" || head3 == "187" || head3 == "188" || head3 == "178" || 
				 head3 == "1709" ){ 
			return CS_SMSROUTE_UNTMP;
		}
		return CS_STRINGFALSE;
	}


	string findDataFromMap( const map<string, string> &mapData, const string &key ){
		if( mapData.find(key) != mapData.end() ){
			return mapData.find(key)->second;
		}
		return CS_STRINGFALSE;
	}
}  // namespace : Universal


//
// 字符验证。。。
//
namespace Universal{
	bool checkNumber( const string &number ){
		if( number.empty() ){ return false; }

		stringstream sin(number);
		double doubleTmp;
		char charTmp;
		if( !(sin >> doubleTmp) )
			return false;
		if( sin >> charTmp )
			return false;

		return true;
	}


	bool checkDate( const string &date ){
		regex reg("^(?<year>(1[8-9]\\d{2})|([2-9]\\d{3}))(-|/|.|)(((?<month>10|12|0?[13578])(-|/|.|)(?<day> 3[01]|[12][0-9]|0?[1-9]))|((?<month>11|0?[469])(-|/|.|)(?<day>30|[12][0-9]|0?[1-9]))|((?<month>0?[2])(-|/|.|)(?<day>0[1-9]|1[0-9]|2[0-8])))(\\s((?<hour>(0[1-9])|([1-9])|(1[0-2]))\\:(?<min>[0-5][0-9])((\\:)(?<sec>[0-5][0-9]))?(\\s[AM|PM|am|pm]{2,2})?))?$");
		return regex_match( date, reg );
		//	"^(?<year>(1[8-9]\d{2})|([2-9]\d{3}))(-|/|.|)(((?<month>10|12|0?[13578])(-|/|.|)(?<day> 3[01]|[12][0-9]|0?[1-9]))|((?<month>11|0?[469])(-|/|.|)(?<day>30|[12][0-9]|0?[1-9]))|((?<month>0?[2])(-|/|.|)(?<day>0[1-9]|1[0-9]|2[0-8])))(\s((?<hour>(0[1-9])|([1-9])|(1[0-2]))\:(?<min>[0-5][0-9])((\:)(?<sec>[0-5][0-9]))?(\s[AM|PM|am|pm]{2,2})?))?$"
	}


	bool checkMobile( const string &mobile ){
		if( mobile.size() != 11 ){  return false;  }

		string head3 = mobile.substr( 0, 3 );
		string head4 = mobile.substr( 0, 4 );
		if( 
			head3 == "130" || head3 == "131" || head3 == "132" || head3 == "133" || head3 == "134" || head3 == "135" || head3 == "136" || 
			head3 == "137" || head3 == "138" || head3 == "139" || head3 == "147" || head3 == "150" || head3 == "151" || head3 == "152" || 
			head3 == "153" || head3 == "155" || head3 == "156" || head3 == "157" || head3 == "158" || head3 == "159" || head3 == "169" || 
			head3 == "177" || head3 == "178" || head3 == "180" || head3 == "181" || head3 == "182" || head3 == "183" || head3 == "184" || 
			head3 == "185" || head3 == "186" || head3 == "187" || head3 == "188" || head3 == "189" ||
			head4 == "1700" || head4 == "1705" || head4 == "1709" 
			){ 
			return true;
		}
		return false;
	}

}  // namespace : Universal



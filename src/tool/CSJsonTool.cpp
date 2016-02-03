/**********************************************************
 * \file logic/CSJsonTool.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "CSJsonTool.h"

#include "CSCommonTool.h"

namespace Universal{
#define _JSONTOOL_FUNC_								CS_COMMON_FUNC_( "CSJsonTool")
	string CSJsonTool::indexOfSplittingPath = CS_SPLITKEY_POINT;
	string CSJsonTool::indexOfSplittingKey = CS_SPLITKEY_COMMA;

	CSJsonTool::CSJsonTool( ){;}
	CSJsonTool::~CSJsonTool( ){;}
	bool CSJsonTool::parseJs( const string &jsStr, Json::Value &jsValue ){
		Json::Reader jsReader;
		return jsReader.parse(  jsStr, jsValue );
	}


	Json::Value CSJsonTool::getJsValue( const Json::Value &jsValue, const string &path ){
		Json::Value jsValueTmp = jsValue;
		Json::Value* jsValueTmp_p = findJsValue( jsValueTmp, path );
		if(  jsValueTmp_p != NULL )
			return *jsValueTmp_p;
		else
			return Json::Value( );
	}

	
	Json::Value CSJsonTool::getJsValue( const Json::Value &jsArray, const string &key, const string &value ){
		Json::Value jsArrayTmp = jsArray;
		Json::Value* jsValueTmp_p = findJsValue( jsArrayTmp, key, value );
		if(  jsValueTmp_p != NULL )
			return *jsValueTmp_p;
		else
			return Json::Value( );
	}


	Json::Value* CSJsonTool::findJsValue( Json::Value &jsValue, const string &path ){
		if(  path.size() == 0 ) return &jsValue;

		vector<string> keys;
		if(  !parseKeyPath( path, keys ) )  return NULL;

		string error( "");
		Json::Value* jsFoundValue_p = &jsValue;
		for(  vector<string>::iterator iterKey = keys.begin(); iterKey != keys.end(); ++iterKey ){
			error += " key --> " + *iterKey + " ||";
			jsFoundValue_p = findJsValueByKey( *jsFoundValue_p, *iterKey );
			if(  jsFoundValue_p == NULL ){
				//DEBUG_D( "%s: found a empty json value.key path is %s.already go [%s]", _JSONTOOL_FUNC_, path.c_str(), error.c_str() );
				return NULL;
			}
		}  //for
		return jsFoundValue_p;
	}


	Json::Value* CSJsonTool::findJsValue( Json::Value &jsArray, const string &key, const string &value ){
		if(  jsArray.isArray() ){
			long jsCount = jsArray.size( );
			for(  int i=0; i<jsCount; ++i ){
				Json::Value::Members vKeys = jsArray[i].getMemberNames( );
				for(  Json::Value::Members::iterator iterJson = vKeys.begin(); iterJson != vKeys.end(); ++iterJson ){
					if(  key == (*iterJson) ){
						if(  jsArray[i][*iterJson].isString() && jsArray[i][*iterJson].asString() == value )
							return &jsArray[i];
					}
				} //for
			} //for
		}
		return NULL;
	}


	string CSJsonTool::getJsString( const Json::Value &jsValue, string path ){
		Json::Value jsValueTmp = getJsValue( jsValue, path );
		string value( CS_STRINGFALSE );
		if( jsValueTmp.isString() )
			value = jsValueTmp.asString( );
		else if( jsValueTmp.isUInt() || jsValueTmp.isInt() )
			value = CSInside::intToStr( jsValueTmp.asUInt() );
		else if( jsValueTmp.isDouble() )
			value = CSInside::intToStr( (long)jsValueTmp.asDouble() );
		else
			return value;
		return value;
	}
	bool CSJsonTool::getJsStrList( const Json::Value &jsArray, const string &key, vector<string> &strList ){
		strList.clear( );
		int jsCount = jsArray.size( );
		for(  int i=0; i<jsCount; ++i )
			strList.push_back( getJsString(jsArray[i], key ) );
		return true;
	}
	bool CSJsonTool::convertJsToMap( const Json::Value &jsValue, map<string, string> &mapResult, string keyList ){
		if(  jsValue.isArray() ){
			mapResult.clear( );
			mapResult.insert( pair<string,string>("array", getJsString(jsValue)) );
			DEBUG_D( "%s: Json is a array.", _JSONTOOL_FUNC_ );
			return false;
		}
		if(  jsValue.isNull() ){
			DEBUG_D( "%s: Json data has empty.", _JSONTOOL_FUNC_ );
			return false;
		}
		
		bool doFilterData = false;
		vector<string> needKeys;
		if(  keyList.size() != 0 ){
			doFilterData = true;
			if(  !parseKeyList(keyList, needKeys ) )  return false;
		}

		mapResult.clear( );
		Json::Value::Members vKeys = jsValue.getMemberNames( );
		for(  Json::Value::Members::iterator iterKey = vKeys.begin(); iterKey != vKeys.end(); ++iterKey ){
			if(  !doFilterData )
				mapResult.insert( pair<string,string>(*iterKey, getJsString(jsValue[*iterKey])) );
			else if(  std::find(needKeys.begin(), needKeys.end(), *iterKey ) != needKeys.end() )
				mapResult.insert( pair<string,string>(*iterKey, getJsString(jsValue[*iterKey])) );
		}  // for 
		return true;
	}
	bool CSJsonTool::convertJsToMapList( const Json::Value &jsValue, vector<map<string, string> > &mapListResult, string keyList ){
		mapListResult.clear( );
		map<string, string> content;
		if(  jsValue.isArray() ){
			for(  int i=0; i<jsValue.size(); ++i ){
				convertJsToMap( jsValue[i], content, keyList );
				mapListResult.push_back( content);
			}
		}
		else{
			DEBUG_D( "%s: Json value is not a array.", _JSONTOOL_FUNC_ );
			return false;
		}
		return true;
	}
	bool CSJsonTool::convertJsToMapList( const Json::Value &jsValue, list<map<string, string> > &mapListResult, string keyList ){
		mapListResult.clear( );
		map<string, string> content;
		if(  jsValue.isArray() ){
			for(  int i=0; i<jsValue.size(); ++i ){
				convertJsToMap( jsValue[i], content, keyList );
				mapListResult.push_back( content);
			}
		}
		else{
			DEBUG_D( "%s: Json value is not a array.", _JSONTOOL_FUNC_ );
			return false;
		}
		return true;
	}
	string CSJsonTool::convertMapToJsStr( const map<string, string> mapData ){
		string defaultJsStr = "\t{\n";

		map<string, string>::const_iterator citerJsItem = mapData.begin( );
		while(  citerJsItem != mapData.end() ){
			string valueTmp = citerJsItem->second;
			Universal::strReplaceAll( valueTmp, "\"", "\\\"" );
			Universal::strReplaceAll( valueTmp, "\r", "\\r" );
			Universal::strReplaceAll( valueTmp, "\n", "\\n" );
			convertFullToHalf( valueTmp );

			defaultJsStr += "\t\t\"" + citerJsItem->first + "\" : \"" + valueTmp + "\"";
			if(  ++citerJsItem == mapData.end() )
				defaultJsStr += "\n";
			else
				defaultJsStr += ",\n";
		}

		defaultJsStr += "\t}";
		
		return defaultJsStr;
	}
	string CSJsonTool::convertMapListToJsStr( const vector<map<string, string> > &mapList, string arrayName ){
		if(  mapList.size() == 0 )  return "";

		string defaultJsStr;
		defaultJsStr.reserve(  mapList.size() * calmemoryOfObject( *(mapList.begin()) ) * 2 );  // 空间换时间。
		defaultJsStr += "[\n";
		for( vector<map<string,string> >::const_iterator citerJsBlock = mapList.begin(); citerJsBlock != mapList.end(); ){
			defaultJsStr += CSJsonTool::convertMapToJsStr( *citerJsBlock );
			if(  ++citerJsBlock != mapList.end() )
				defaultJsStr += ",";
		}
		defaultJsStr += "\n]\n";

		if(  arrayName.size() != 0 ){
			defaultJsStr = "{\n\t\"" + arrayName + "\" : " + defaultJsStr + "}";
		}
		return defaultJsStr;
	}
	string CSJsonTool::convertMapListToJsStr( const list<map<string, string> > &mapList, string arrayName ){
		if(  mapList.size() == 0 )  return "";

		string defaultJsStr;
		defaultJsStr.reserve(  mapList.size() * calmemoryOfObject( *(mapList.begin()) ) * 2 );  // 空间换时间。
		defaultJsStr += "[\n";
		for( list<map<string,string> >::const_iterator citerJsBlock = mapList.begin(); citerJsBlock != mapList.end(); ){
			defaultJsStr += CSJsonTool::convertMapToJsStr( *citerJsBlock );
			if(  ++citerJsBlock != mapList.end() )
				defaultJsStr += ",";
		}
		defaultJsStr += "\n]\n";

		if(  arrayName.size() != 0 ){
			defaultJsStr = "{\n\t\"" + arrayName + "\" : " + defaultJsStr + "}";
		}
		return defaultJsStr;
	}
	bool CSJsonTool::parseKeyPath( const string &path, vector<string> &keys ){
		return Universal::splitString( path, indexOfSplittingPath, keys );
	}
	bool CSJsonTool::parseKeyList( const string &keyList, vector<string> &keys ){
		if(  !Universal::splitString(keyList, indexOfSplittingKey, keys ) ){
			DEBUG_D( "%s: key list format is wrong.key list is [ %s ]", _JSONTOOL_FUNC_, keyList.c_str() );
			return false;
		}
		return true;
	}
	Json::Value* CSJsonTool::findJsValueByKey( Json::Value &jsValue, string &key ){
		Json::Value *jsValue_p = &jsValue;
		if(  jsValue_p->isString() || jsValue_p->empty() || jsValue_p->isNull() )
			return NULL;
		if(  jsValue_p->isArray() ){
			if( !checkNumber(key) ){
				DEBUG_D( "%s: Key is not a number. Key name is %s.", _JSONTOOL_FUNC_, key.c_str() );
				return NULL;
			}
			int index = CSInside::strToInt( key);
			if(  index >= jsValue_p->size() ) return NULL;
			return &( *jsValue_p)[index];
		}
		else{
			Json::Value::Members vKeys = jsValue_p->getMemberNames( );
			for(  Json::Value::Members::iterator iterJson = vKeys.begin(); iterJson != vKeys.end(); ++iterJson ){
				if(  key == (*iterJson) ){
					return &( *jsValue_p)[key];
				}
			} //for
		}
		return NULL;
	}
} // namespace Universal 


/**********************************************************
 * \file logic/tool/File.cpp
 * \brief 
 * \note 
 * 
 * \version 
 * \author zheng39562@163.com
**********************************************************/
#include "pub_file.h"

#include <fstream>
#include "boost/regex.hpp"
#include "boost/filesystem.hpp"
#include "pub_tool.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;

namespace Universal{
	string readFile(const string &path){
		std::ifstream infile;
		infile.open(path.c_str());
		if(infile){
			ostringstream ostr;
			ostr << infile.rdbuf();
			infile.close();
			string strTmp = ostr.str();

			return strTmp;
		}
		else{
			DEBUG_I("open aciton is failed.Please check arg of path:" << path << ".");
			return _STRINGFALSE;
		}
	}
	bool writeFile(const string &path, const string &content){
		std::ofstream outfile(path.c_str());
		if(outfile){
			outfile << content;
			outfile.close();
			return true;
		}
		else{
			DEBUG_I("open aciton is failed.Please check arg of path:" << path << ".");
		}
		return false;
	}
	bool addContentToFile(const string &path, const string &content){
		std::ofstream outfile(path.c_str(), ios::app);
		if(outfile){
			outfile << content;
			outfile.close();
			return true;
		}
		else{
			DEBUG_I("open aciton is failed.Please check arg of path:" << path << ".");
			return false;
		}
	}

	bool createDir(string filePath){
		return (bool)create_directory(path(filePath));
	}


	bool clearDir(string filePath){
		return (bool)remove_all(path(filePath));
	}


	bool deleteFile(const string &filePath){
		return (bool)remove(path(filePath));
	}

	bool findFileFromDir(string filePath, vector<string> &fileNames, string filePattern){
		path pPath(filePath);
		if (is_directory(pPath)){
			directory_iterator end;
			for (directory_iterator citer(pPath); citer != end; ++citer){
				if(!is_directory(*citer)){
					string fileName = citer->path().string();
					if(!filePattern.empty()){
						boost::regex reg(filePattern);
						if (!boost::regex_match(fileName, reg)){
							continue;
						}
					}
					fileNames.push_back(fileName);
				}
			}
		}

		return true;
	}

	string getAbsPath(string filepath){
		return system_complete(path(filepath)).string();
	}

	string getFileNameByPath(const string &filepath){
		path filePath(filepath);
		if(is_directory(filePath)){
			return filePath.relative_path().string();
		}
		else{
			return filePath.parent_path().string();
		}
	}

	string completePath(const string &path){
		if(!path.empty() && path[path.size()-1 ] != '/'){
			return path + "/";
		}
		return path;
	}
} // namespace



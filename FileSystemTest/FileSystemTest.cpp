// FileSystemTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "boost/filesystem.hpp"   // 包含所有需要的 Boost.Filesystem 声明
#include <iostream>               // 使用 std::cout
namespace fs = boost::filesystem;

// 宏FSTEST：测试f的成员函数，输出成员函数名和结果
#define FSTEST(x) std::cout << #x##": " << f.x << std::endl		


int _tmain(int argc, _TCHAR* argv[])
{
	
	fs::path f("//folder1//folder2//folder3//filename.ext");

	FSTEST(string());
	//FSTEST(file_string());
	//FSTEST(directory_string());
	FSTEST(root_name());
	FSTEST(root_directory());
	FSTEST(root_path());
	FSTEST(relative_path());
	FSTEST(filename());
	FSTEST(parent_path());
	FSTEST(stem());
	FSTEST(extension());

	FSTEST(replace_extension("new"));
	char buf[]="hello";
	FSTEST(append(buf, buf+sizeof(buf)));
	//FSTEST(remove_filename());

	getchar();
	return 0;
}


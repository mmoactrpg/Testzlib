// FileSystemTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "boost/filesystem.hpp"   // ����������Ҫ�� Boost.Filesystem ����
#include <iostream>               // ʹ�� std::cout
namespace fs = boost::filesystem;

// ��FSTEST������f�ĳ�Ա�����������Ա�������ͽ��
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


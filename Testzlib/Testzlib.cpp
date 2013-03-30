// Testzlib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "zlib.h"
#include <Windows.h>

using namespace std;
#pragma comment(lib, "zlibd.lib")


//用read函数文本模式下读取文件，读到字节数小于真实文件大小，
//因为系统读 \r\n 时又转换为 \n, 只是\n被读进来了
//若是binary模式，则会读到实际文件大小的字节
Byte *readFileRead(char *fname, uLong *fileLength, ios_base::openmode om = ios_base::in)
{
	ifstream ifs(fname, om);

	ifs.seekg(0, ios::end);     //移动文件指针到末尾
	std::streamoff fileLen = ifs.tellg();        //获取文件长度字节
	*fileLength = (uLong)fileLen;

	ifs.seekg(ios::beg);        //恢复指针到开始，对于接下来继续读有用

	char *buffer = new char[fileLen+1];
	memset(buffer, 0, sizeof(char)*(fileLen+1) );
	ifs.read(buffer, fileLen);
	ifs.close();
	//
	return (Byte*)buffer;
	
	//////////////////////////////////////////////////////////////////////////
	//if(om & ios_base::binary)
	//{// binary
	//	cout << " binary模式";
	//	//
	//	char *buffer = new char[fileLen+1];
	//	memset(buffer, 0, sizeof(char)*(fileLen+1) );
	//	ifs.read(buffer, fileLen);
	//	ifs.close();
	//	//
	//	return (Byte*)buffer;
	//}
	//else
	//{// text
	//	cout << " text模式";
	//	string strResult="";
	//	string strTemp="";
	//	//
	//	while(!ifs.eof())
	//	{
	//		getline(ifs,strTemp);
	//		strResult+=strTemp;
	//	}
	//	//
	//	ifs.close();
	//	//
	//	return (Byte*)strResult.c_str();
	//}



	//////////////////////////////////////////////////////////////////////////

	/*
	cout << " read()读取了"<< ifs.gcount() << "字节:\n";   
	cout << buffer;*/

	
	//
	/*Byte *outBuffer = new Byte[fileLen];
	memset(outBuffer, 0, fileLen );
	memcpy(outBuffer, buffer, fileLen);*/

	//Byte *_outBuffer = new Byte[fileLen];
	//memset(_outBuffer, 0, fileLen );
	////memcpy(_outBuffer, outBuffer, fileLen);
	//memcpy(_outBuffer, buffer, fileLen);

	////fileByte = _outBuffer;

	////fileByte = _outBuffer;
	//
	////memcpy(outBuffer, _outBuffer, fileLen);
	///**outBuffer = *(Byte*)buffer;*/
	////string outBuffer(buffer);

	//delete [] buffer;
	//buffer = NULL;
	

	//return (Byte*)buffer;
}


#pragma region UTF8字符读取
//////////////////////////////////////////////////////////////////////////
//解决UTF-8类型的文本文档中文乱码读取（思路:将UTF-8转成Unicode然后再转ANSI）
//////////////////////////////////////////////////////////////////////////


//changeTxtEncoding修改字符串的编码
char *changeTxtEncoding(char* szU8)
{ 
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	cout<<wszString<<endl;


	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansiLen + 1];
	::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
	szAnsi[ansiLen] = '\0';
	return szAnsi;
}

//changeTextFromUtf8ToAnsi读取UTF-8格式的文件并将之保存为ANSI格式的文件
void changeTextFromUtf8ToAnsi(const char* filename)
{
	ifstream infile;string strLine="";string strResult="";
	infile.open(filename);
	if (infile)
	{
		while(!infile.eof())
		{
			getline(infile,strLine);
			strResult+=strLine+"\n";
		}
	}
	infile.close();
	char* changeTemp = new char[strResult.length()];
	strcpy(changeTemp,strResult.c_str());
	char* changeResult = changeTxtEncoding(changeTemp);
	strResult=changeResult;

	ofstream outfile;
	outfile.open("I:\\ANSI.txt");
	outfile.write(strResult.c_str(),strResult.length());
	outfile.flush();
	outfile.close();
}

#pragma endregion UTF8类型的文本读取

#pragma region Unicode类型的文本

//////////////////////////////////////////////////////////////////////////
// 解决Unicode类型的文本文档中文乱码读取（此方法经测试不可用于打开ANSI的文本文档）
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 宽字符串转换正常字符串
std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

//////////////////////////////////////////////////////////////////////////
// 读取Unicode文件
std::string readUnicodeFile(char *filename)
{
	ifstream fin;
	fin.open(filename, ios::binary);
	size_t index = 2;
	std::string strRet;
	std::string strLineAnsi;
	std::wstring wstrLine;

	while (!fin.eof())
	{
		fin.seekg(index, ios::beg);
		wchar_t wch;
		fin.read((char *)(&wch), 2);
		if (wch == 0x000D) // 判断回车
		{
			strLineAnsi = ws2s(wstrLine);
			wstrLine.erase(0, wstrLine.size() + 1);
			index += 2; // 跳过回车符和行开头符
			strRet = strRet + strLineAnsi;
		}
		else
		{
			wstrLine.append(1, wch);
			index += 2;
		}
	}

	strLineAnsi = ws2s(wstrLine);
	strRet = strRet + strLineAnsi;
	fin.close();
	//if语句解决文本段末有回车导致最后字符集重复的问题
	if (strLineAnsi!="")
	{
		strRet=strRet.substr(0,strRet.length()-1);
	}
	printf("%s", strRet.c_str());
	return strRet;
}

#pragma endregion Unicode类型的文本

#pragma region ANSI文本类型

void ANSIFile(char *file)
{
	ifstream infile;
	string strResult="";
	string strTemp="";
	infile.open(file);
	if (infile)
	{
		while(!infile.eof())
		{
			getline(infile,strTemp);
			strResult+=strTemp;
		}
	}
	infile.close();
	cout<<strResult;
}
#pragma endregion ANSI文本类型


int _tmain(int argc, _TCHAR* argv[])
{

	/*ifstream readfile;
	readfile.open("t1.txt", ios::binary);*/
	
	//string outstr;
	//string textBuffer;
	float compressPercent = 0;
	

	//while( readfile >> outstr ) 
	//{    
	//	textBuffer += outstr;//cout << "Read from file: " << outstr << endl;  
	//}
	//readfile.close();
	
	//Byte *textBuffer = NULL;


	uLong fileLen;
	Byte *textBuffer = readFileRead("t1.txt", &fileLen, ios::in );

	

	size_t strLen = strlen((char*)textBuffer);
	//size_t strLen = textBuffer.length();

	//ofstream outFile1("b0.bin", ios::binary);
	////outFile << comprBuff;
	//outFile1.write((char*)textBuffer, strLen);
	//outFile1.flush();
	//outFile1.close();

	Bytef *srcContent = new Bytef[strLen];
	srcContent = (Bytef*)textBuffer; //"1234567890"; //原文

	
	
	uLong srcContentLen = strlen((char*)srcContent)+1;// 需要把字符串的结束符'\0'也一并处理
	//uLong destLen = (double)srcLen * 1.001  + 12;

	uLong comprBuffLen = compressBound(srcContentLen); // 压缩后的长度是不会超过blen的

	int err; // 返回的错误代码
	Byte *comprBuff = new Byte[sizeof(char) * comprBuffLen];	// 计算缓冲区大小，并为其分配内存
	
	if(comprBuff == NULL)  
	{
		printf("no enough memory!\n");  
		return -1;  
	}  
	
	//comprLen = sizeof(comprBuff) / sizeof(comprBuff[0]);
	err = compress(comprBuff, &comprBuffLen, (const Bytef*)srcContent, srcContentLen);
	if (err != Z_OK) 
	{ 
		cerr << "compress erro: " << err << '\n ';
		return -1;
	}

	
	// 压缩后 输出
	ofstream outFile("b0.bin", ios::binary);
	//outFile << comprBuff;
	outFile.write((char*)comprBuff, comprBuffLen);
	outFile.flush();
	outFile.close();


	compressPercent = (float)comprBuffLen/srcContentLen;
	cout<<"【原文长度】: " << srcContentLen
		<<"【压缩后长度】:"<< comprBuffLen
		<<"【压缩比】："<<(compressPercent*100)<<'%\n '; 

	//strcpy((char*)uncompr, "garbage ");


	err = uncompress(srcContent, &srcContentLen, comprBuff, comprBuffLen); 

	if (err != Z_OK) 
	{ 
		cerr << "解压错误: " << err << '\n '; 
	} 

	/*cout<< "【原文长度】: " << srcContentLen
		<< "【压缩后长度】:"<< comprBuffLen
		<<"\n 【压缩比】："<<(comprBuffLen/srcContentLen)<<'\n '; */

	if (strcmp((char*)srcContent, (char*)textBuffer))
	{ 
		cerr << "\n解压失败!!!\n "; 
	}
	else 
	{ 
		cout << "\n解压成功!!!\n"; 
	} 

	// 解压后
	/*f= fopen("b1.bin","wb");
	fwrite(uncompr,uncomprLen,1,f);
	fclose(f);*/

	outFile.open("b1.bin", ios::out);
	outFile.write((char*)srcContent, srcContentLen);
	//outFile << srcContent;
	outFile.flush();
	outFile.close();


	delete [] srcContent;
	delete [] comprBuff;


	getchar();
	return 0;
}


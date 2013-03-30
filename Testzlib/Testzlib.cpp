// Testzlib.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "zlib.h"
#include <Windows.h>

using namespace std;
#pragma comment(lib, "zlibd.lib")


//��read�����ı�ģʽ�¶�ȡ�ļ��������ֽ���С����ʵ�ļ���С��
//��Ϊϵͳ�� \r\n ʱ��ת��Ϊ \n, ֻ��\n����������
//����binaryģʽ��������ʵ���ļ���С���ֽ�
Byte *readFileRead(char *fname, uLong *fileLength, ios_base::openmode om = ios_base::in)
{
	ifstream ifs(fname, om);

	ifs.seekg(0, ios::end);     //�ƶ��ļ�ָ�뵽ĩβ
	std::streamoff fileLen = ifs.tellg();        //��ȡ�ļ������ֽ�
	*fileLength = (uLong)fileLen;

	ifs.seekg(ios::beg);        //�ָ�ָ�뵽��ʼ�����ڽ���������������

	char *buffer = new char[fileLen+1];
	memset(buffer, 0, sizeof(char)*(fileLen+1) );
	ifs.read(buffer, fileLen);
	ifs.close();
	//
	return (Byte*)buffer;
	
	//////////////////////////////////////////////////////////////////////////
	//if(om & ios_base::binary)
	//{// binary
	//	cout << " binaryģʽ";
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
	//	cout << " textģʽ";
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
	cout << " read()��ȡ��"<< ifs.gcount() << "�ֽ�:\n";   
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


#pragma region UTF8�ַ���ȡ
//////////////////////////////////////////////////////////////////////////
//���UTF-8���͵��ı��ĵ����������ȡ��˼·:��UTF-8ת��UnicodeȻ����תANSI��
//////////////////////////////////////////////////////////////////////////


//changeTxtEncoding�޸��ַ����ı���
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

//changeTextFromUtf8ToAnsi��ȡUTF-8��ʽ���ļ�����֮����ΪANSI��ʽ���ļ�
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

#pragma endregion UTF8���͵��ı���ȡ

#pragma region Unicode���͵��ı�

//////////////////////////////////////////////////////////////////////////
// ���Unicode���͵��ı��ĵ����������ȡ���˷��������Բ������ڴ�ANSI���ı��ĵ���
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ���ַ���ת�������ַ���
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
// ��ȡUnicode�ļ�
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
		if (wch == 0x000D) // �жϻس�
		{
			strLineAnsi = ws2s(wstrLine);
			wstrLine.erase(0, wstrLine.size() + 1);
			index += 2; // �����س������п�ͷ��
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
	//if������ı���ĩ�лس���������ַ����ظ�������
	if (strLineAnsi!="")
	{
		strRet=strRet.substr(0,strRet.length()-1);
	}
	printf("%s", strRet.c_str());
	return strRet;
}

#pragma endregion Unicode���͵��ı�

#pragma region ANSI�ı�����

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
#pragma endregion ANSI�ı�����


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
	srcContent = (Bytef*)textBuffer; //"1234567890"; //ԭ��

	
	
	uLong srcContentLen = strlen((char*)srcContent)+1;// ��Ҫ���ַ����Ľ�����'\0'Ҳһ������
	//uLong destLen = (double)srcLen * 1.001  + 12;

	uLong comprBuffLen = compressBound(srcContentLen); // ѹ����ĳ����ǲ��ᳬ��blen��

	int err; // ���صĴ������
	Byte *comprBuff = new Byte[sizeof(char) * comprBuffLen];	// ���㻺������С����Ϊ������ڴ�
	
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

	
	// ѹ���� ���
	ofstream outFile("b0.bin", ios::binary);
	//outFile << comprBuff;
	outFile.write((char*)comprBuff, comprBuffLen);
	outFile.flush();
	outFile.close();


	compressPercent = (float)comprBuffLen/srcContentLen;
	cout<<"��ԭ�ĳ��ȡ�: " << srcContentLen
		<<"��ѹ���󳤶ȡ�:"<< comprBuffLen
		<<"��ѹ���ȡ���"<<(compressPercent*100)<<'%\n '; 

	//strcpy((char*)uncompr, "garbage ");


	err = uncompress(srcContent, &srcContentLen, comprBuff, comprBuffLen); 

	if (err != Z_OK) 
	{ 
		cerr << "��ѹ����: " << err << '\n '; 
	} 

	/*cout<< "��ԭ�ĳ��ȡ�: " << srcContentLen
		<< "��ѹ���󳤶ȡ�:"<< comprBuffLen
		<<"\n ��ѹ���ȡ���"<<(comprBuffLen/srcContentLen)<<'\n '; */

	if (strcmp((char*)srcContent, (char*)textBuffer))
	{ 
		cerr << "\n��ѹʧ��!!!\n "; 
	}
	else 
	{ 
		cout << "\n��ѹ�ɹ�!!!\n"; 
	} 

	// ��ѹ��
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


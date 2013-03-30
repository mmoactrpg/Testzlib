// Testzlib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "zlib.h"

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

	/*if(om & ios_base::binary)
	{
	cout << " binary模式";
	}

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
	ifs.close();

	return (Byte*)buffer;
}

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
	outFile << comprBuff;
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

	outFile.open("b1.bin", ios::binary);
	outFile << srcContent;

	outFile.close();


	delete [] srcContent;
	delete [] comprBuff;


	getchar();
	return 0;
}


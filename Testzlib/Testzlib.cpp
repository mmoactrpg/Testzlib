// Testzlib.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "zlib.h"

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

	/*if(om & ios_base::binary)
	{
	cout << " binaryģʽ";
	}

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
	outFile << comprBuff;
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

	outFile.open("b1.bin", ios::binary);
	outFile << srcContent;

	outFile.close();


	delete [] srcContent;
	delete [] comprBuff;


	getchar();
	return 0;
}


#pragma once
#include<opencv2/opencv.hpp>
#include<vector>
#include<string>
#include <io.h>  
#include "wininet.h"
#include <direct.h>
#include<fstream>
#include <list>
#pragma comment(lib,"Wininet.lib")

using namespace std;
using namespace cv;

typedef struct PathElem {
	TCHAR   SrcImgPath[MAX_PATH * 2];
	TCHAR   RstImgPath[MAX_PATH * 2];
}PathElem;

class BasicFuncation
{
public:

	BasicFuncation();
	vector<string> getImagesPath(string folder_path);
	//����·���µ�����ͼƬ����������ͼƬ�ľ���·��
	vector<string> GetPath(string search_path);

	vector<string> GetIndex(string folder_path);

	vector<Mat> getImageMatList();
	//path:ͼƬ�ļ���
	//�����ļ���������ͼƬ�� Mat ���� 
	vector<Mat> getImageMatList(string path);
	//string getString(float src);
	string getString(int src);
	string Transform_Float2String(float number);
	string Transform_Int2String(int number);
	string Transform_Double2String(double number);
	double Transform_String2Double(string input_array);
	float Transform_String2Float(string input_array);
	int Transform_String2int(string input_array);

	void NormalizeString(string input,int output_length,string &output);
	//��䷨����ͼƬ������size��С��Mat
	Mat GetImgMask(Mat inputImage, Size size);
	//��ȡ���ݼ���·����ͼƬ����ǩ
	vector< string> split(string str, string pattern);
	//��ȡtxt�����ݣ����ж�ȡ
	vector<string> ReadTxt(string txt_path);
	~BasicFuncation();
	int FindImgs(char * pSrcImgPath, char * pRstImgPath, std::list<PathElem> &ImgList);
	//��path�ļ����½�����������format��ʽ���ļ������������·����files
	void GetAllFormatFiles(string path, vector<string>& files, string format);

private:
	void GetAllFiles(string path, vector<string>& files);
	vector<Mat> image_list;
};


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
	//查找路径下的所有图片，返回所有图片的绝对路径
	vector<string> GetPath(string search_path);

	vector<string> GetIndex(string folder_path);

	vector<Mat> getImageMatList();
	//path:图片文件夹
	//返回文件夹下所有图片的 Mat 数组 
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
	//填充法处理图片，返回size大小的Mat
	Mat GetImgMask(Mat inputImage, Size size);
	//获取数据集：路径，图片，标签
	vector< string> split(string str, string pattern);
	//获取txt的内容，按行读取
	vector<string> ReadTxt(string txt_path);
	~BasicFuncation();
	int FindImgs(char * pSrcImgPath, char * pRstImgPath, std::list<PathElem> &ImgList);
	//对path文件夹下进行搜索，对format格式的文件，保存其绝对路径至files
	void GetAllFormatFiles(string path, vector<string>& files, string format);

private:
	void GetAllFiles(string path, vector<string>& files);
	vector<Mat> image_list;
};


#include "stdafx.h"
#include "BasicFuncation.h"


BasicFuncation::BasicFuncation()
{
}
vector<string> BasicFuncation::GetIndex(string folder_path){
	std::list<PathElem> ImgList;
	std::list<PathElem>::iterator pImgListTemp;
	char* srcpath = (char*)folder_path.data();
	vector<string> list;
	ImgList.clear();
	FindImgs(srcpath, srcpath, ImgList);
	pImgListTemp = ImgList.begin();
	for (int iik = 1; iik <= ImgList.size(); iik++)
	{
		list.push_back(pImgListTemp->RstImgPath);
		pImgListTemp++;
	}
	return list;
}
vector<string> BasicFuncation::GetPath(string search_path){
	std::list<PathElem> ImgList;
	std::list<PathElem>::iterator pImgListTemp;
	char* srcpath = (char*)search_path.data();
	vector<string> list;
	ImgList.clear();
	FindImgs(srcpath, srcpath, ImgList);
	pImgListTemp = ImgList.begin();
	for (int iik = 1; iik <= ImgList.size(); iik++)
	{
		list.push_back(pImgListTemp->RstImgPath);
		pImgListTemp++;
	}
	return list;
}
vector<string> BasicFuncation::getImagesPath(string folder_path){
	std::list<PathElem> ImgList;
	std::list<PathElem>::iterator pImgListTemp;
	char* srcpath = (char*)folder_path.data();
	fstream file_out;
	file_out.open("images_path.txt",ios::out);
	vector<string> list;
	ImgList.clear();
	FindImgs(srcpath, srcpath, ImgList);
	pImgListTemp = ImgList.begin();

	for (int iik = 1; iik <= ImgList.size(); iik++)
	{
		Mat image = imread(pImgListTemp->RstImgPath);
		if (!image.empty()){
			image_list.push_back(image);
			file_out.write(pImgListTemp->RstImgPath, strlen(pImgListTemp->RstImgPath));
			file_out.write("\n", 1);
		}
		list.push_back(pImgListTemp->RstImgPath);
		pImgListTemp++;
	}
	file_out.close();
	return list;
}
vector<Mat> BasicFuncation::getImageMatList(){
	return image_list;
}
int BasicFuncation::FindImgs(char * pSrcImgPath, char * pRstImgPath, std::list<PathElem> &ImgList){
	//ԴͼƬ���ڵ�Ŀ¼
	TCHAR   szFileT1[MAX_PATH * 2];
	lstrcpy(szFileT1, TEXT(pSrcImgPath));
	lstrcat(szFileT1, TEXT("*.*"));

	//���ͼƬ��ŵ�Ŀ¼
	TCHAR   RstAddr[MAX_PATH * 2];
	lstrcpy(RstAddr, TEXT(pRstImgPath));
	//_mkdir(RstAddr);   //�����ļ���

	WIN32_FIND_DATA   wfd;
	HANDLE   hFind = FindFirstFile(szFileT1, &wfd);

	PathElem stPathElemTemp;
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (wfd.cFileName[0] == TEXT('.'))
				continue;
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY || strcmp("Thumbs.db", TEXT(wfd.cFileName)) == 0)
			{
				;
			}
			else
			{

				TCHAR   SrcImgPath[MAX_PATH * 2];
				lstrcpy(SrcImgPath, pSrcImgPath);
				lstrcat(SrcImgPath, TEXT(wfd.cFileName));

				lstrcpy(stPathElemTemp.SrcImgPath, SrcImgPath);

				TCHAR   AdressTemp[MAX_PATH * 2];
				lstrcpy(AdressTemp, pRstImgPath);

				//lstrcat(AdressTemp, TEXT("/"));  
				lstrcat(AdressTemp, TEXT(wfd.cFileName));
				lstrcpy(stPathElemTemp.RstImgPath, AdressTemp);

				ImgList.push_back(stPathElemTemp);

			}
		} while (FindNextFile(hFind, &wfd));
	}
	else
	{
		return -1;
	}
	return 0;
}
BasicFuncation::~BasicFuncation()
{
}
//string BasicFuncation::getString(float src) {
//	stringstream ss;
//	ss << src;
//	return ss.str();
//}
string BasicFuncation::getString(int src) {
	stringstream ss;
	ss << src;
	return ss.str();
}
string BasicFuncation::Transform_Float2String(float number) {
	stringstream ss;
	ss << number;
	return ss.str();
}
string BasicFuncation::Transform_Int2String(int number) {
	stringstream ss;
	ss << number;
	return ss.str();
}
string BasicFuncation::Transform_Double2String(double number) {
	stringstream ss;
	ss << number;
	return ss.str();
}
double BasicFuncation::Transform_String2Double(string input_array) {
	double num;
	stringstream ss;
	ss << input_array;
	ss >> num;
	if (!ss.good()) {
		cerr << "Transform_String2Double �� ���ִ������������������ȷ�ԡ�" << endl;
	}
	return num;
}
float BasicFuncation::Transform_String2Float(string input_array) {
	float num;
	stringstream ss;
	ss << input_array;
	ss >> num;
	if (!ss.good()) {
		cerr << "Transform_String2Float �� ���ִ������������������ȷ�ԡ�" << endl;
	}
	return num;
}
int BasicFuncation::Transform_String2int(string input_array) {
	int num;
	stringstream ss;
	ss << input_array;
	ss >> num;
	if (!ss.good()) {
		cerr << "Transform_String2int �� ���ִ������������������ȷ�ԡ�" << endl;
	}
	return num;
}
Mat BasicFuncation::GetImgMask(Mat inputImage, Size size){
	int s;
	if (inputImage.rows > inputImage.cols) {
		s = inputImage.rows;
	}
	else {
		s = inputImage.cols;
	}
	Mat image_temp_ep(s, s, CV_8UC3, Scalar(255, 255, 255));
	if (inputImage.channels() == 1) {
		cvtColor(image_temp_ep, image_temp_ep,CV_BGR2GRAY);
	}
	Mat image_temp_ep_roi = image_temp_ep(Rect((s - inputImage.cols) / 2, (s - inputImage.rows) / 2, inputImage.cols, inputImage.rows));
	Mat dstNormImg;
	addWeighted(image_temp_ep_roi, 0., inputImage, 1.0, 0., image_temp_ep_roi);
	resize(image_temp_ep, dstNormImg, size, 0, 0, 1);    //��С��һ��
	return dstNormImg;
}
//�ַ����ָ��
vector< string> BasicFuncation::split(string str, string pattern)
{
	vector<string> ret;
	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);
	while (index != str.npos)
	{
		if (start != index)
			ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}
	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}
vector<string> BasicFuncation::ReadTxt(string txt_path) {
	fstream file_read;
	file_read.open(txt_path, ios::in);
	vector<string> list;
	string line;
	//while(file_read.getline(str))
	while (getline(file_read, line)) {
		list.push_back(line);
	}
	return list;
}
void BasicFuncation::NormalizeString(string input, int output_length, string &output) {
	char* character;
	char* input_data = (char*)input.data();
	string output_data(output_length,' ');
	//character = (char*)output_data.c_str();
	character = (char*)malloc(sizeof(char)*output_length);
	int input_length = input.size();
	if (input_length < output_length) {
		int j = 0;
		for (int i =0;i < output_length; i++) {
			if (i >= output_length / 2 - input_length / 2 && i <= output_length / 2 + input_length / 2) {
				character[i] = input_data[j];
				output_data[i]= input_data[j];
				j++;
			}
			else {
				character[i] = ' ';
			}
		}
		cout << input <<"-"<< output_data <<endl;
		output = output_data;
	}
	else {
		return;
	}
}
//��ȡ���е��ļ���  
void BasicFuncation::GetAllFiles(string path, vector<string>& files)
{

	int hFile = 0;
	//�ļ���Ϣ    
	struct _finddata_t fileinfo;//�����洢�ļ���Ϣ�Ľṹ��    
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)  //��һ�β���  
	{
		cout << hFile << endl;
		cout << fileinfo.name << endl;
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))  //������ҵ������ļ���  
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //�����ļ��в���  
				{
					files.push_back(p.assign(path).append("/").append(fileinfo.name));
					GetAllFiles(p.assign(path).append("/").append(fileinfo.name), files);
				}
			}
			else //������ҵ��Ĳ������ļ���   
			{
				files.push_back(p.assign(fileinfo.name));  //���ļ�·�����棬Ҳ����ֻ�����ļ���:    p.assign(path).append("\\").append(fileinfo.name)  
			}

		} while (_findnext(hFile, &fileinfo));

		_findclose(hFile); //��������  
	}

}

//��ȡ�ض���ʽ���ļ���  
void BasicFuncation::GetAllFormatFiles(string path, vector<string>& files, string format)
{
	files.clear();
	//�ļ����    
	intptr_t hFile = 0;
	//�ļ���Ϣ    
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("/*." + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
					GetAllFormatFiles(p.assign(path).append("/").append(fileinfo.name), files, format);
				}
			}
			else
			{
				files.push_back(p.assign(fileinfo.name));  //���ļ�·�����棬Ҳ����ֻ�����ļ���:    p.assign(path).append("\\").append(fileinfo.name)  
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}
#pragma once
//实现对已识别的图片分组
#include"common.h"
#include <direct.h> 
#include<windows.h>
typedef struct {
	string name;
	string label;
}PIC;

class Cluster
{
public:
	Cluster();
	typedef struct {
		string cluster_name;
		int number;
	}clust;
	void cluster(string outpath);
	vector<Cluster::clust> getClusterMessage();
	void Count(vector<PIC>input,vector<clust>& output);
	void Sort(vector<PIC> input, vector<PIC>& output);
	void Stock(string stock_path,vector<PIC> image_path,vector<clust> image_class);
	~Cluster();
private:


	vector<PIC> images_list;
	vector<clust> image_cluster;
	//字符串分割
	vector<string> split(string str,string pattern);
	//读取图片文件路径 和 类别
	void readImagePathAndClassNameFile(string filepath);
	void sort();
};


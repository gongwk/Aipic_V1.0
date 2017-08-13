#pragma once
#include "common.h"
#include<cstdlib>
#include<opencv2\dnn.hpp>
using namespace cv::dnn;
class Caffe
{
public:
	Caffe();
	void Init(string net_proto,string net_weights,string label);
	//ʶ��ͼƬ��𣬸���ͼƬ���ݹؼ���
	vector<string> recognize();
	string Recognize(Mat image);
	vector<String> Recognize(vector<Mat> image);
	string Predict(Mat image);
	int GetClassID();
	double GetClassProb();
	//����ͼƬ���ݣ�λ���ļ����ڡ��������
	int load(string folderpath);
	int load(Mat image);
	int load(vector<Mat> images);
	//��ȡʶ����ͼƬ���ݱ�ǩ
	void getKeyWorld();
	~Caffe();
private:
	bool Init();
	//����
	dnn::Net net;
	vector<string> labels;
	int class_id = 0;
	double class_prob = 0.0;
	//���ڲ��Ե�ͼƬ�б�
	vector<Mat> image_list;
	//ͼƬʶ�������ݱ�ǩ
	vector<string> image_keyworld;

	/* Find best class for the blob (i. e. class with maximal probability) */
	void getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb);
	//��ȡ��������� vector ��
	std::vector<string> readClassNames(const char *filename );

};


#pragma once
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
#include"BasicFuncation.h"
using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace cv::xfeatures2d;

typedef struct PERSONL {
	int ID;
	string name;
}PERSONL;

class Classifier
{
public:
	Classifier();
	~Classifier();
	
	int loadData(vector<KeyPoint> input_data, int input_data_labels);
	int loadData(vector<vector<KeyPoint>> input_data,vector<int> input_data_labels);
	int loadData(Mat descriptor,int label);
	int loadData(vector<Mat> descriptors,vector<int> labels);

	void ANN_MLP_Train();
	void ANN_MLP_PredictInit();
	int ANN_MLP_Predict(Mat descriptor);
	void SVM_Train();
	void SVM_PredictInit();
	int SVM_Predict(Mat descriptor);

	void SVMSGD_Train();
	int SVMSGD_Predict(Mat descriptor);

	void KNN_Train(); 
	int KNN_Predict(Mat descriptor);

	void NormalBayesClassifier_Train();
	int NormalBayesClassifier_Predict(Mat descriptor);

	vector<Mat> ComputeBowImageDescriptors(vector<string> img_path,vector<vector<KeyPoint>> img_keypoints,vector<Mat> img_descriptors);
	Mat BuildBowDictionary(vector<Mat> img_descriptors);
	//void getTrainData(string data_path);
private:
	Ptr<SVM> svm ;
	Ptr<SVM> svm_train;
	Ptr<SVM> svm_test;
	Ptr<ml::ANN_MLP> ann_mlp_train;
	Ptr<ml::ANN_MLP> ann_mlp_test;
	cv::Ptr<SVMSGD> svmsgd;
	cv::Ptr<SVMSGD> svmsgd_train;
	cv::Ptr<SVMSGD> svmsgd_val;
	void computeWithContinious(Mat& _src, Mat& _dst, float scale);
	Mat g_dictionary;
	// For train and test or pridect
	//vector<KeyPoint> data;
	vector<int> labels;
	Mat train_data;
	Mat train_label;
	//vector<Mat> images_train;
	//vector<int> labels_train;
	//vector<PERSONL> personl_list;
};


#pragma once
#include<iostream>
#include<string>
#include<opencv2/opencv.hpp>
#include<opencv2/xfeatures2d.hpp>
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
class FeatureExtract
{
public:
	//支持的特征
	enum Method { _SURF, _SIFT, _ORB, _HIST };
	//支持的LBP特征类型
	enum LBP_TYPE{LBP_S,LBP_M,LBP_M_LOCAL,LBP_M_LOCAL_WHOLE,LBP_C,LBPC_LOCAL,LBP_C_LOCAL_WHOLE};
	Mat GetDescriptor();
	FeatureExtract();
	//input_image:输入图像
	//method: 特征提取方法
	Mat Extract(Mat input_image, int method);
	//一些列图像的特征：关键点信息 按照图像顺序存放
	vector<vector<KeyPoint>> GetFeatures();
	// 计算 图像关键点特征 的 特征描述子
	vector<Mat> ComputeDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints);
	vector<Mat> ComputeSIFTDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints);
	vector<Mat> ComputeSURFDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints);
	vector<Mat> ComputeORBDescriptor(vector<Mat> images, vector<vector<KeyPoint>> keypoints);
	// 处理图像特征点 使其满足统一的数量信息
	vector<vector<KeyPoint>> ProcessFeatures(vector<vector<KeyPoint>> keypoints, int types, int nfeatures);
	vector<KeyPoint> OptimizeFeature(vector<KeyPoint> keypoint,int cut_number);
	~FeatureExtract();
private:
	Mat srcImage;
	Mat featureImage;
	Mat descriptor;
	vector<vector<KeyPoint>> feature_keypoints;
	Mat hist_feature;
	//-------------------------------------------------------------------------------
	//lbp
	double global_average_difference(const cv::Mat& src, int radius = 1, int neighbors = 8);
	double local_average_difference(const cv::Mat& src, int i, int j, int radius = 1, int neighbors = 8);
	double global_average_gray(const cv::Mat& src);
	double local_average_gray(const cv::Mat& src, int i, int j, int radius = 1, int neighbors = 8);
	Mat lbps(const cv::Mat& src, int radius = 1, int neighbors = 8);
	Mat lbpm(const cv::Mat& src, int radius = 1, int neighbors = 8);
	Mat lbpm_local(const cv::Mat& src, int radius = 1, int neighbors = 8);
	Mat lbpm_local_whole(const cv::Mat& src, int radius = 1, int neighbors = 8);
	Mat lbpc(const cv::Mat& src);
	Mat lbpc_local(const cv::Mat& src, int radius = 1, int neighbors = 8);
	Mat lbpc_local_whole(const cv::Mat& src, int radius = 1, int neighbors = 8);
	// hog 特征图
	Mat get_hogdescriptor_visual_image(Mat& origImg,
		vector<float>& descriptorValues,//hog特征向量
		Size winSize,//图片窗口大小
		Size cellSize,
		int scaleFactor,//缩放背景图像的比例
		double viz_factor);//缩放hog特征的线长比例
public:
	//各方法
	Mat SURF(
		Mat input_image, 
		double 	hessianThreshold = 100,
		int 	nOctaves = 4,
		int 	nOctaveLayers = 3,
		bool 	extended = false,
		bool 	upright = false,
		Scalar	scalar = Scalar::all(-1),
		int		drawFlags= DrawMatchesFlags::DEFAULT		
	);
	Mat LBP(Mat input_image,int lbp_type=LBP_C);
	Mat LBP(Mat input_image, int lbp_type = LBP_C,int raduis=1,int neighbors=8);
	Mat SIFT(Mat input_image,int nfeatures = 0, int nOctaveLayers = 3, double contrastThreshold = 0.04, double edgeThreshold = 10, double sigma = 1.6);
	Mat ORB(
		Mat input_image, 
		int 	nfeatures = 500,
		float 	scaleFactor = 1.2f,
		int 	nlevels = 8,
		int 	edgeThreshold = 31,
		int 	firstLevel = 0,
		int 	WTA_K = 2,
		int 	scoreType = cv::ORB::HARRIS_SCORE,
		int 	patchSize = 31,
		int 	fastThreshold = 20,
		Scalar  scalar=Scalar::all(-1),
		int		drawFlags = DrawMatchesFlags::DEFAULT
	);
	Mat HOG(Mat input_image);
	Mat GetHOGFeature();
	Mat Hist(Mat input_image);
	Mat GetHistFeature();
};

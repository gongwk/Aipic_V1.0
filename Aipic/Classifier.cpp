#include "stdafx.h"
#include "Classifier.h"


Classifier::Classifier()
{
}
Classifier::~Classifier()
{
}
int Classifier::loadData(vector<vector<KeyPoint>> input_data, vector<int> input_data_labels) {
	return -1;
}
int Classifier::loadData(Mat descriptor, int label) {
	train_data.push_back(descriptor.reshape(1, 1));
	train_label.push_back(label);
	return 1;
}
int Classifier::loadData(vector<Mat> descriptors, vector<int> labels) {
	for (int i = 0; i < descriptors.size(); i++) {
		//cout << descriptors.size() << endl;
		train_data.push_back(descriptors.at(i).reshape(1,1));
		train_label.push_back(labels.at(i));
	}
	return descriptors.size();
}
Mat Classifier::BuildBowDictionary(vector<Mat> img_descriptors) {
	cout << "开始计算Bow 词典！" << endl;
	BOWKMeansTrainer bowTraining(500);   //定义聚类中心2000个，其余的默认参数；
	Mat all_des;
	for (int i = 0; i < img_descriptors.size(); i++) {
		if (img_descriptors.at(i).size().height==0|| img_descriptors.at(i).size().width == 0) {
			for (int insert = i; insert > 0; insert--) {
				if (img_descriptors.at(insert).size().height != 0 && img_descriptors.at(insert).size().width != 0) {
					bowTraining.add(img_descriptors.at(insert));
					break;
				}
			}
		}
		else {
			bowTraining.add(img_descriptors.at(i));
		}
		//all_des.push_back(img_descriptors.at(i));
	}
	//bowTraining.add(all_des);
	Mat dictionary = bowTraining.cluster();  //这一步的时间根据特征的维度以及定义的词典中心的个数相关。
	//imshow("dictionary", dictionary);
	//waitKey(33);
	cout << "已生成Bow 词典！" << endl;
	g_dictionary = dictionary;
	return dictionary;
}
vector<Mat> Classifier::ComputeBowImageDescriptors(vector<string> img_path, vector<vector<KeyPoint>> img_keypoints, vector<Mat> img_descriptors) {
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
	BOWImgDescriptorExtractor bowExtractor(matcher);
	bowExtractor.setVocabulary(g_dictionary);
	vector<Mat> bow_features;
	for (int i = 0; i < img_keypoints.size(); i++) {
		Mat image = imread(img_path.at(i));
		Mat imageDescriptor;
		bowExtractor.compute(img_descriptors.at(i), imageDescriptor);
		//bowExtractor.compute(image, img_keypoints.at(i),imageDescriptor);
		//normalize(imageDescriptor, imageDescriptor,1.0,0.0,NORM_MINMAX);
		bow_features.push_back(imageDescriptor);
		//cout << "imageDescriptor: "<<imageDescriptor.size() << endl;
	}
	return bow_features;
}
void Classifier::ANN_MLP_Train() {

}
void Classifier::ANN_MLP_PredictInit() {

}
int Classifier::ANN_MLP_Predict(Mat descriptor) {

}

void Classifier::SVM_PredictInit() {
	svm_test= Algorithm::load<SVM>("svm.xml");
}
void Classifier::SVM_Train() {
	//SVM 初始化
	svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 1e-6));
	printf("训练数据信息：行数 %d 列数 %d \n ", train_data.rows, train_data.cols);
	svm->train(train_data, ROW_SAMPLE, train_label);
	svm->save("svm.xml");
}
int Classifier::SVM_Predict(Mat descriptor) {
	//cout << "svm predict:descriptor size:" << descriptor.size() << endl;
	float result = svm_test->predict(descriptor.reshape(1, 1));
	//printf("SVM result:%f \n", result);
	return (int)result;
}
void Classifier::KNN_Train() {
	Ptr < cv::ml::KNearest > knn= cv::ml::KNearest::create();
	Mat lable_mat(train_label);
	Mat data_mat(train_data);
	Ptr<TrainData> train_data = TrainData::create(data_mat, ROW_SAMPLE, lable_mat);
	knn->train(train_data);
	knn->save("knn.xml");
}
int Classifier::KNN_Predict(Mat descriptor) {
	Ptr<cv::ml::KNearest> knn = Algorithm::load<cv::ml::KNearest>("knn.xml");
	float result = knn->predict(descriptor.reshape(1, 1));
	printf("KNearest result:%f \n", result);
	return (int)result;
}
void Classifier::NormalBayesClassifier_Train() {
	Ptr<cv::ml::NormalBayesClassifier> bayes = cv::ml::NormalBayesClassifier::create();
	Mat lable_mat(train_label);
	Mat data_mat(train_data);
	Ptr<TrainData> train_data = TrainData::create(data_mat, ROW_SAMPLE, lable_mat);
	bayes->train(train_data);
	bayes->save("nbayes.xml");
}
int Classifier::NormalBayesClassifier_Predict(Mat descriptor) {
	Ptr<cv::ml::NormalBayesClassifier> nbayes = Algorithm::load<cv::ml::NormalBayesClassifier>("nbayes.xml");
	float result = nbayes->predict(descriptor.reshape(1, 1));
	printf(" NormalBayesClassifier result:%f \n", result);
	return (int)result;
}
void Classifier::SVMSGD_Train() {
	printf("训练数据信息：行数 %d 列数 %d \n ", train_data.rows, train_data.cols);
	svmsgd = SVMSGD::create();
	Mat lable_mat(train_label);
	Mat data_mat(train_data);
	Ptr<TrainData> train_data = TrainData::create(data_mat, ROW_SAMPLE, lable_mat);
	svmsgd->train(train_data);
	svmsgd->save("svmsgd.xml");
}
int Classifier::SVMSGD_Predict(Mat descriptor) {
	Ptr<SVMSGD> svm_sgd = Algorithm::load<SVMSGD>("svmsgd.xml");
	float result=svm_sgd->predict(descriptor.reshape(1, 1));
	cout <<"结果准确度： "<< result << endl;
	return (int)result;
}
void Classifier::computeWithContinious(Mat& _src, Mat& _dst, float scale)
{
	_dst = cv::Scalar::all(0);
	int i, j;
	Size size = _src.size();
	int chns = _src.channels();
	if (_src.isContinuous() && _dst.isContinuous())
	{
		size.width *= size.height;
		size.height = 1;
	}

	for (i = 0; i < size.height; i++)
	{
		const  unsigned char* src = (const unsigned char*)(_src.data + _src.step*i);
		unsigned char* dst = (unsigned char*)(_dst.data + _dst.step*i);
		for (j = 0; j < size.width; j++)
		{
			if (chns == 1)
			{
				dst[j] = src[j];
				cout << (int)dst[j] << endl;
			}
			else
			{
				dst[j*chns] = (unsigned char)(src[j*chns] * scale);
				dst[j*chns + 1] = (unsigned char)(src[j*chns + 1] * scale);
				dst[j*chns + 2] = (unsigned char)(src[j*chns + 2] * scale);
			}
		}
	}
}
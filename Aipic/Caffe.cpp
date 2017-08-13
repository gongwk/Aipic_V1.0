#include "stdafx.h"
#include "Caffe.h"


Caffe::Caffe()
{
	//Init();
}
bool Caffe::Init(){
	//����ģ�ͺ������ļ�
	String modelTxt = "caffemodel/cifar10_quick.prototxt";
	String modelBin = "caffemodel/cifar10_quick_iter_25000.caffemodel";
	//! [Create the importer of Caffe model]
	Ptr<dnn::Importer> importer;
	try                                     //Try to import Caffe model
	{
		importer = dnn::createCaffeImporter(modelTxt, modelBin);
	}
	catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
	{
		std::cerr << err.msg << std::endl;
	}
	//! [Create the importer of Caffe model]

	if (!importer)
	{
		std::cerr << "Can't load network by using the following files: " << std::endl;
		std::cerr << "prototxt:   " << modelTxt << std::endl;
		std::cerr << "caffemodel: " << modelBin << std::endl;
		exit(-1);
	}
	//��ʼ������
	importer->populateNet(net);
	importer.release();                     //We don't need importer anymore
}
void Caffe::Init(string net_proto, string net_weights, string label) {
	String modelTxt = net_proto;
	String modelBin = net_weights;
	Ptr<dnn::Importer> importer;
	try                                     //Try to import Caffe model
	{
		importer = dnn::createCaffeImporter(modelTxt, modelBin);
	}
	catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
	{
		std::cerr << err.msg << std::endl;
	}
	//! [Create the importer of Caffe model]

	if (!importer)
	{
		std::cerr << "Can't load network by using the following files: " << std::endl;
		std::cerr << "prototxt:   " << modelTxt << std::endl;
		std::cerr << "caffemodel: " << modelBin << std::endl;
		exit(-1);
	}
	//��ʼ������
	importer->populateNet(net);
	importer.release();                     //We don't need importer anymore

	labels= readClassNames(label.data());
}
string Caffe::Predict(Mat image) {
	//׼��blob
	Mat img = image.clone();
	//resize(img, img, Size(32, 32));
	dnn::Blob inputBlob = dnn::Blob::fromImages(img);
	//dnn::Blob inputBlob = dnn::Blob(img);
	net.setBlob(".data", inputBlob);
	//ǰ�򴫲�
	net.forward();
	dnn::Blob prob = net.getBlob("prob");
	//����ǩ
	int classID;
	double classProb;
	getMaxClass(prob, &classID, &classProb);
	class_id = classID;
	class_prob = classProb;
	return labels.at(classID);
}
int Caffe::GetClassID() {
	return class_id;
}
double Caffe::GetClassProb() {
	return class_prob;
}
string Caffe::Recognize(Mat image){
		//׼��blob
		Mat img = image.clone();
		resize(img, img, Size(32, 32));
		dnn::Blob inputBlob = dnn::Blob::fromImages(img);
		//dnn::Blob inputBlob = dnn::Blob(img);
		net.setBlob(".data", inputBlob);
		//ǰ�򴫲�
		net.forward();
		dnn::Blob prob = net.getBlob("prob");
		//����ǩ
		int classID;
		double classProb;
		getMaxClass(prob, &classID, &classProb);
		std::vector<string> classNames = readClassNames("caffemodel/label.txt");
		return classNames.at(classID);
}
vector<string> Caffe::recognize(){
	fstream file_log;
	fstream file_path;
	fstream file_path_and_classname;

	file_log.open("log_classify_result.txt",ios::out);
	file_path.open("images_path.txt",ios::in);
	file_path_and_classname.open("images_path_classname.txt",ios::out);
    //��ȡͼƬ·���ļ�
	vector<string> file_path_vec;
	string line;
	while (getline(file_path, line)) {		
		file_path_vec.push_back(line);
	}
	file_path.close();
	//��ȡ��ϣ������file_path_vec��

	//����ģ�ͺ������ļ�
	String modelTxt = "caffemodel/cifar10_quick.prototxt";
	String modelBin = "caffemodel/cifar10_quick_iter_25000.caffemodel";
	//! [Create the importer of Caffe model]
	Ptr<dnn::Importer> importer;
	try                                     //Try to import Caffe model
	{
		importer = dnn::createCaffeImporter(modelTxt, modelBin);
	}
	catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
	{
		std::cerr << err.msg << std::endl;
	}
	//! [Create the importer of Caffe model]

	if (!importer)
	{
		std::cerr << "Can't load network by using the following files: " << std::endl;
		std::cerr << "prototxt:   " << modelTxt << std::endl;
		std::cerr << "caffemodel: " << modelBin << std::endl;
		exit(-1);
	}
	//��ʼ������
	dnn::Net net;
	importer->populateNet(net);
	importer.release();                     //We don't need importer anymore

	for (int i = 0; i < image_list.size(); i++){
		//׼��blob
		Mat img = image_list.at(i);
		dnn::Blob inputBlob = dnn::Blob(img);
		net.setBlob(".data",inputBlob);
		//ǰ�򴫲�
		net.forward();
		dnn::Blob prob = net.getBlob("prob");

		//����ǩ
		int classID;
		double classProb;
		getMaxClass(prob,&classID,&classProb);
		//ת��double��string ����
		//string str;
		//stringstream ss;
		//ss << classProb;
		//ss >> str;
		//-----
		std::vector<string> classNames = readClassNames("caffemodel/label.txt");
		//std::cout << "Best class: #" << classID<< " '" << classNames.at(classID) << "'" << std::endl;
		image_keyworld.push_back(classNames.at(classID));
		//д�ļ�
		file_log << classNames.at(classID) <<endl;
		if (i < image_list.size() - 1) {
			file_path_and_classname << file_path_vec.at(i) << " " << classNames.at(classID) <<" "<< endl;
		}
		else {
			file_path_and_classname << file_path_vec.at(i) << " " << classNames.at(classID) << " ";
		}//std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
		
	}
	return image_keyworld;
}
int Caffe::load(Mat image){
	if (!image.empty()){
		image_list.push_back(image);
		return 0;
	}
	else{
		//TODO:��־�ļ���¼
		return 1;
	}
}
int Caffe::load(vector<Mat> images){
	for (int i = 0; i < images.size(); i++){
		return load(images.at(i));
	}

}
std::vector<string>  Caffe::readClassNames(const char *filename  )
{
	std::vector<string> classNames;

	std::ifstream fp(filename);
	if (!fp.is_open())
	{
		std::cerr << "File with classes labels not found: " << filename << std::endl;
		exit(-1);
	}

	std::string name;
	while (!fp.eof())
	{
		std::getline(fp, name);
		if (name.length())
			classNames.push_back(name.substr(name.find(' ') + 1));
	}

	fp.close();
	return classNames;
}
void  Caffe::getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
	Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
	Point classNumber;
	minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
	*classId = classNumber.x;
}
Caffe::~Caffe()
{
}

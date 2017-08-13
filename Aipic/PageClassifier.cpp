// PageClassifier.cpp : 实现文件
#include "stdafx.h"
#include "Aipic.h"
#include "PageClassifier.h"
#include "afxdialogex.h"
// PageClassifier 对话框
IMPLEMENT_DYNAMIC(PageClassifier, CDialogEx)
PageClassifier::PageClassifier(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TrainClassifier, pParent)
{
}
void PageClassifier::Init() {
	command = (CEdit*)GetDlgItem(IDC_EDIT_Command);
	CComboBox* box;
	box = (CComboBox*)GetDlgItem(IDC_COMBO_Percentage);
	box->AddString("10%");
	box->AddString("20%");
	box->SetCurSel(0);
	box->EnableWindow(FALSE);
	box = (CComboBox*)GetDlgItem(IDC_COMBO_Classifier);
	box->AddString("SVM");
	box->AddString("NormalBayesClassifier");
	box->AddString("KNearest");
	box->AddString("SVMSGD");
	box->SetCurSel(0);
	box = (CComboBox*)GetDlgItem(IDC_COMBO_Feature);
	box->AddString("SURF");
	box->AddString("ORB");
	box->AddString("SIFT");
	box->SetCurSel(0);

	feature_process_method.AddString("BOW");
	feature_process_method.AddString("归一化");
	feature_process_method.SetCurSel(0);
	box = (CComboBox*)GetDlgItem(IDC_COMBO_Percentage);
	CEdit *edit_box;
	edit_box = (CEdit*)GetDlgItem(IDC_EDIT_TestDataSet);
	CButton *button;
	button = (CButton*)GetDlgItem(IDC_BUTTON_TestBrowse);
	box->EnableWindow(TRUE);
	edit_box->EnableWindow(FALSE);
	button->EnableWindow(FALSE);

}
PageClassifier::~PageClassifier()
{
}
void PageClassifier::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_Test, check_box);
	//DDX_Control(pDX, IDC_EDIT_CommandRunTime, command_runtime);
	DDX_Control(pDX, IDC_COMBO_FeatureProcessMethod, feature_process_method);
}
BEGIN_MESSAGE_MAP(PageClassifier, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TrainBrows, &PageClassifier::OnBnClickedButtonTrainbrows)
	ON_BN_CLICKED(IDC_BUTTON_TestBrowse, &PageClassifier::OnBnClickedButtonTestbrowse)
	ON_BN_CLICKED(IDC_CHECK_Test, &PageClassifier::OnBnClickedCheckTest)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_Feature, &PageClassifier::OnCbnSelchangeComboFeature)
	ON_BN_CLICKED(IDC_BUTTON_Train, &PageClassifier::OnBnClickedButtonTrain)
	ON_BN_CLICKED(IDC_BUTTON_SetFeatureParam, &PageClassifier::OnBnClickedButtonSetfeatureparam)
	ON_WM_TIMER()
END_MESSAGE_MAP()
// PageClassifier 消息处理程序
void PageClassifier::OnBnClickedButtonTrain()
{
	Log("----------------------------------------");
	// Step 1. 数据准备
	CComboBox* box;
	box = (CComboBox*)GetDlgItem(IDC_COMBO_Percentage);
	if (BST_CHECKED == check_box.GetCheck()) {
		Log("[ 数据 ]：验证数据集来自文件。");
		train_img_path = img_path;
		train_img_labels = img_labels;
	}
	else if (BST_UNCHECKED == check_box.GetCheck()) {
		Log("[ 数据 ]：自动分割训练、验证数据集。");
		train_img_path.clear();
		train_img_labels.clear();
		CString num;
		CComboBox* box_num = (CComboBox*)GetDlgItem(IDC_COMBO_Percentage);
		box_num->GetLBText(box_num->GetCurSel(), num);
		if (num == "10%") {
			for (int i = 0; i < img_path.size(); i++) {
				if (i < img_path.size() / 10) {
					test_img_path.push_back(img_path.at(i));
					test_img_labels.push_back(img_labels.at(i));
				}
				else {
					train_img_path.push_back(img_path.at(i));
					train_img_labels.push_back(img_labels.at(i));
				}
			}
			Log("[ 数据 ]：90% 训练 ， 10% 验证");
		}
		else if (num == "20%") {
			for (int i = 0; i < img_path.size(); i++) {
				if (i < img_path.size() / 5) {
					test_img_path.push_back(img_path.at(i));
					test_img_labels.push_back(img_labels.at(i));
				}
				else {
					train_img_path.push_back(img_path.at(i));
					train_img_labels.push_back(img_labels.at(i));
				}
			}
			Log("[ 数据 ]：80% 训练 ， 20% 验证");
		}
	}
	if (train_img_path.size() == 0) {
		MessageBox("训练数据集未导入！");
		return;
	}
	if (test_img_path.size() == 0) {
		MessageBox("测试数据集未导入！");
		return;
	}
	Log("[ 数据 ]：数据加载完成");
	vector<vector<KeyPoint>> train_kp,test_kp;
	vector<Mat> train_des,test_des;
	FeatureExtract fe,train_fe,test_fe;
	BasicFuncation bf;
	Classifier cl;
	CString str;
	CComboBox* box_feature = (CComboBox*)GetDlgItem(IDC_COMBO_Feature);
	CComboBox* box_classifier = (CComboBox*)GetDlgItem(IDC_COMBO_Classifier);
	CString feature_type;
	CString classifier_type;
	//Step 2. 提取特征
	box_feature->GetLBText(box_feature->GetCurSel(), feature_type);
	box_classifier->GetLBText(box_classifier->GetCurSel(), classifier_type);
	int feature_numbers = 30;
	vector<Mat> train_images;
	vector<Mat> test_images;
	if (feature_type == "SURF") {
		PageSetParam_SURF::SURF_Param params = page_setSURFParam->GetParam();
		Log("[ 特征提取 ]：SURF");
		for (int i = 0; i < train_img_path.size(); i++) {
			Mat image = imread(train_img_path.at(i), 0);			
			if (image.empty()) {
				MessageBox("图像数据有误，请检查数据！请使用绝对路径！");
			}
			train_images.push_back(image);
			train_fe.SURF(
				image, 
				params.hessianThreshold, 
				params.nOctaves, 
				params.nOctaveLayers, 
				params.extended, 
				params.upright, 
				params.scalar, 
				params.draw_matches_flags);
			if (i % 100 == 0) {
				cout << "SURF train:"<<i << endl;
			}
		}
		for (int i=0;i<test_img_path.size();i++)
		{
			Mat image = imread(test_img_path.at(i), 0);
			if (image.empty()) {
				MessageBox("图像数据有误，请检查数据！请使用绝对路径！");
			}
			test_images.push_back(image);
			test_fe.SURF(
				image, 
				params.hessianThreshold, 
				params.nOctaves, 
				params.nOctaveLayers, 
				params.extended, 
				params.upright, 
				params.scalar, 
				params.draw_matches_flags);
			if (i % 100 == 0) {
				cout << "SURF test:" << i << endl;
			}
		}
	}
	else if (feature_type=="SIFT")
	{
		PageSetParam_SIFT::SIFT_Params params = page_setSIFTParam->GetParam();
		Log("[ 特征提取 ]：SIFT");
		for (int i=0;i<train_img_path.size();i++)
		{
			Mat image = imread(train_img_path.at(i));
			if (image.empty()) {
				MessageBox("图像数据有误，请检查数据！请使用绝对路径！");
			}
			train_images.push_back(image);
			train_fe.SIFT(image,params.nfeatures,params.nOctaveLayers,params.contrastThreshold,params.edgeThreshold,params.sigma);
			if (i % 100 == 0) {
				cout << "SIFT train:" << i << endl;
			}
		}
		for (int i = 0; i < test_img_path.size(); i++) {
			Mat image = imread(test_img_path.at(i), 0);
			if (image.empty()) {
				MessageBox("图像数据有误，请检查数据！请使用绝对路径！");
			}
			test_images.push_back(image);
			test_fe.SIFT(image, params.nfeatures, params.nOctaveLayers, params.contrastThreshold, params.edgeThreshold, params.sigma);
			if (i % 100 == 0) {
				cout << "SIFT test:" << i << endl;
			}
		}
	}
	else if(feature_type=="ORB") {
		PageSetParam_ORB::ORB_Param params = page_setORBParam->GetParam();
		feature_numbers = params.nfeatures;
		Log("[ 特征提取 ]：ORB");
		for (int i = 0; i < train_img_path.size(); i++) {
			//Log_Runtime("[ 特征提取 ]：ORB "+bf.Transform_Int2String(i)+"/"+bf.Transform_Int2String(train_img_path.size()));
			cout << "[ 特征提取 ]：ORB " + bf.Transform_Int2String(i) + "/" + bf.Transform_Int2String(train_img_path.size()) << endl;
			Mat image = imread(train_img_path.at(i), 0);
			if (image.empty()) {
				MessageBox("图像数据有误，请检查数据！请使用绝对路径！");
			}
			train_images.push_back(image);
			train_fe.ORB(image, params.nfeatures, params.scaleFactor, params.nlevels, params.edgeThreshold, params.firstLevel, params.WTA_K, params.scoreType, params.patchSize, params.fastThreshold, params.scalar, params.draw_matches_flags);
			if (i % 100 == 0) {
				cout << "ORB train:" << i << endl;
			}
		}
		for (int i = 0; i < test_img_path.size(); i++)
		{
			cout << "[ 特征提取 ]：ORB " + bf.Transform_Int2String(i) + "/" + bf.Transform_Int2String(test_img_path.size()) << endl;
			Mat image = imread(test_img_path.at(i), 0);
			if (image.empty()) {
				MessageBox("图像数据有误，请检查数据！请使用绝对路径！");
			}
			test_images.push_back(image);
			test_fe.ORB(image,params.nfeatures,params.scaleFactor,params.nlevels,params.edgeThreshold,params.firstLevel,params.WTA_K,params.scoreType,params.patchSize,params.fastThreshold,params.scalar,params.draw_matches_flags);
			if (i % 100 == 0) {
				cout << "ORB test:" << i << endl;
			}
		}
	}
	train_kp = train_fe.GetFeatures();
	test_kp = test_fe.GetFeatures();
	int kp_avg = 0;
	for (int i = 0; i < train_kp.size(); i++) {
		kp_avg+= train_kp.at(i).size();
	}
	kp_avg = kp_avg / train_kp.size();
	cout << "src_kp_avg：" << kp_avg << endl;
	Log("[ 特征提取 ]：特征点平均数量="+bf.Transform_Int2String(kp_avg));

	CString method;
	feature_process_method.GetLBText(feature_process_method.GetCurSel(),method);
	if (method == "归一化") {
		train_kp = train_fe.ProcessFeatures(train_kp, 0, kp_avg);
		test_kp = test_fe.ProcessFeatures(train_kp, 0, kp_avg);
	}
	train_des = train_fe.ComputeDescriptor(train_images, train_kp);	
	test_des = test_fe.ComputeDescriptor(test_images, test_kp);
	//Bow
	if (method == "BOW") {
		Log("[ 特征提取 ]：正在计算BOW描述子...");
		cl.BuildBowDictionary(train_des);
		train_des=cl.ComputeBowImageDescriptors(img_path,train_kp,train_des);
		test_des= cl.ComputeBowImageDescriptors(img_path, test_kp, test_des);
		Log("[ 特征提取 ]：已生成BOW描述子");
	}
	//Step 3. 训练分类器
	cl.loadData(train_des, train_img_labels);
	if (classifier_type == "SVM") {
		cl.SVM_Train();
		cl.SVM_PredictInit();
		float accuracy = 0.0;
		for (int i = 0; i < train_des.size(); i++) {
			if (cl.SVM_Predict(train_des.at(i)) == train_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		string acc = bf.Transform_Float2String((float)(accuracy / train_des.size()));
		Log("[ 分类器 ]：SVM 完成训练");
		Log("[ 分类器 ]：训练集上准确度 = "+acc);
		accuracy = 0;
		for (int i = 0; i < test_des.size(); i++) {
			int classID = cl.SVM_Predict(test_des.at(i));
			//cout << "图像路径：" << test_img_path.at(i) << endl;
			//cout << "预测标签：" << classID << " 真实标签：" << test_img_labels.at(i) << endl;
			if ( classID== test_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		acc = bf.Transform_Float2String((float)(accuracy / test_des.size()));
		Log("[ 分类器 ]：SVM 完成测试");
		Log("[ 分类器 ]：测试集上准确度 = " + acc);
	}
	else if (classifier_type == "SVMSGD") {
		cl.SVMSGD_Train();
		float accuracy = 0.0;
		for (int i = 0; i < train_des.size(); i++) {
			if (cl.SVMSGD_Predict(train_des.at(i)) == train_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		string acc = bf.Transform_Float2String((float)(accuracy / train_des.size()));
		Log("[ 分类器 ]：SVMSGD 完成训练");
		Log("[ 分类器 ]：训练集上准确度 = " + acc);
		accuracy = 0;
		for (int i =0; i < test_des.size(); i++) {
			if (cl.SVMSGD_Predict(test_des.at(i)) == test_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		acc = bf.Transform_Float2String((float)(accuracy / test_des.size()));
		Log("[ 分类器 ]：SVMSGD 完成测试");
		Log("[ 分类器 ]：测试集上准确度 = " + acc);
	}
	else if (classifier_type == "NormalBayesClassifier") {
		cl.NormalBayesClassifier_Train();
		float accuracy = 0.0;
		for (int i = 0; i < train_des.size(); i++) {
			if (cl.NormalBayesClassifier_Predict(train_des.at(i)) == train_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		string acc = bf.Transform_Float2String((float)(accuracy / train_des.size()));
		Log("[ 分类器 ]：NormalBayesClassifier 完成训练");
		Log("[ 分类器 ]：训练集上准确度 = " + acc);
		accuracy = 0;
		for (int i = 0; i < test_des.size(); i++) {
			if (cl.NormalBayesClassifier_Predict(test_des.at(i)) == test_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		acc = bf.Transform_Float2String((float)(accuracy / test_des.size()));
		Log("[ 分类器 ]：NormalBayesClassifier 完成测试");
		Log("[ 分类器 ]：测试集上准确度 = " + acc);
	}
	else if (classifier_type == "KNearest") {
		cl.KNN_Train();
		float accuracy = 0.0;
		for (int i = 0; i < train_des.size(); i++) {
			if (cl.KNN_Predict(train_des.at(i))==train_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		string acc = bf.Transform_Float2String((float)(accuracy / train_des.size()));
		Log("[ 分类器 ]：KNearest 完成训练");
		Log("[ 分类器 ]：训练集上准确度 = " + acc);
		accuracy = 0;
		for (int i = 0; i < test_des.size(); i++) {
			if (cl.KNN_Predict(test_des.at(i)) == test_img_labels.at(i)) {
				accuracy += 1;
			}
		}
		acc = bf.Transform_Float2String((float)(accuracy / test_des.size()));
		Log("[ 分类器 ]：KNearest 完成测试");
		Log("[ 分类器 ]：测试集上准确度 = " + acc);
	}
}
void PageClassifier::OnBnClickedButtonSetfeatureparam()
{
	CComboBox* box_feature = (CComboBox*)GetDlgItem(IDC_COMBO_Feature);
	CString feature_type;
	box_feature->GetLBText(box_feature->GetCurSel(), feature_type);
	if (feature_type == "SURF") {
		page_setSURFParam = new PageSetParam_SURF();
		page_setSURFParam->Create(IDD_DIALOG_Param_SURF, this);
		page_setSURFParam->ShowWindow(SW_SHOWNORMAL);
		page_setSURFParam->Init();
	}
	else if (feature_type == "ORB") {
		page_setORBParam = new PageSetParam_ORB();
		page_setORBParam->Create(IDD_DIALOG_Param_ORB, this);
		page_setORBParam->ShowWindow(SW_SHOWNORMAL);
		page_setORBParam->Init();
	}
	else if (feature_type == "SIFT") {
		page_setSIFTParam = new PageSetParam_SIFT();
		page_setSIFTParam->Create(IDD_DIALOG_Param_SIFT, this);
		page_setSIFTParam->ShowWindow(SW_SHOWNORMAL);
		page_setSIFTParam->Init();
	}
}
void PageClassifier::OnBnClickedButtonTrainbrows()
{
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		CEdit* pBoxOne;
		pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_TrainDataSet);
		pBoxOne->SetWindowText(_T(strFile));
		GetTrainData((string)strFile);
	}
}
void PageClassifier::OnBnClickedButtonTestbrowse()
{
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		CEdit* pBoxOne;
		pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_TestDataSet);
		pBoxOne->SetWindowText(_T(strFile));
		GetTestData((string)strFile);
	}
}
void PageClassifier::OnBnClickedCheckTest()
{
	CComboBox* box;
	box = (CComboBox*)GetDlgItem(IDC_COMBO_Percentage);
	CEdit *edit_box;
	edit_box = (CEdit*)GetDlgItem(IDC_EDIT_TestDataSet);
	CButton *button;
	button = (CButton*)GetDlgItem(IDC_BUTTON_TestBrowse);
	if (BST_CHECKED == check_box.GetCheck()) {
		box->EnableWindow(FALSE);
		edit_box->EnableWindow(TRUE);
		button->EnableWindow(TRUE);
	}
	if (BST_UNCHECKED == check_box.GetCheck()) {
		box->EnableWindow(TRUE);
		edit_box->EnableWindow(FALSE);
		button->EnableWindow(FALSE);
	}
}
HBRUSH PageClassifier::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
void PageClassifier::GetTrainData(string input_file_path) {
	BasicFuncation bf;
	CString str;
	vector<string> split;
	vector<string> temp = bf.ReadTxt(input_file_path);
	Log("[ 数据 ]：载入数据共" + bf.getString(temp.size()) + "个");
	for (int i = 0; i < temp.size(); i++) {
		split = bf.split(temp.at(i), " ");
		if (split.size() > 0) {
			img_path.push_back(split.at(0));
			img_labels.push_back(_ttoi(split.at(1).data()));
		}
	}
}
void PageClassifier::GetTestData(string input_file_path) {
	BasicFuncation bf;
	CString str;
	vector<string> split;
	vector<string> temp = bf.ReadTxt(input_file_path);
	Log("[ 数据 ]：载入测试数据共" + bf.getString(temp.size()) + "个");
	for (int i = 0; i < temp.size(); i++) {
		split = bf.split(temp.at(i), " ");
		if (split.size() > 0) {
			test_img_path.push_back(split.at(0));
			test_img_labels.push_back(_ttoi(split.at(1).data()));
		}
	}
}
void PageClassifier::Process(string input_image_path, string input_image_label) {
	BasicFuncation bf;
	img_path = bf.ReadTxt(input_image_path);
	vector<string> temp = bf.ReadTxt(input_image_label);
	for (int i = 0; i < temp.size(); i++) {
		img_labels.push_back(_ttoi((char*)temp.at(i).data()));
	}
	vector<string> split = bf.split(input_image_path, "\\");
	command->SetSel(-1, -1);
	CString str;
	for (int i = 0; i < split.size(); i++) {
		str = (char*)split.at(i).data();
		command->ReplaceSel(str + "\r\n");
	}
}
void PageClassifier::OnCbnSelchangeComboFeature()
{
}
void PageClassifier::Log(string cmd) {
	if (cmd.size() > 0) {
		CString command_str = (CString)cmd.data();
		command->ReplaceSel(command_str + "\r\n");
	}
	else {
		return;
	}
}
void PageClassifier::Log_Runtime(string cmd) {
	/*if (cmd.size() > 0) {
		CString command_str = (CString)cmd.data();
		command_runtime.Clear();
		command_runtime.ReplaceSel(command_str);
	}
	else {
		return;
	}*/
}

void PageClassifier::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
}

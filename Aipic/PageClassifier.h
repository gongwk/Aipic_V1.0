#pragma once
#include "afxwin.h"
#include"BasicFuncation.h"
#include"FeatureExtract.h"
#include"Classifier.h"
// PageClassifier 对话框
#include"PageSetParam_SURF.h"
#include"PageSetParam_ORB.h"
#include"PageSetParam_SIFT.h"
class PageClassifier : public CDialogEx
{
	DECLARE_DYNAMIC(PageClassifier)

public:
	PageClassifier(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageClassifier();
	void Init();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TrainClassifier };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTrainbrows();
	afx_msg void OnBnClickedButtonTestbrowse();
	afx_msg void OnBnClickedCheckTest();
	CButton check_box;
private:
	//参数
	PageSetParam_SIFT *page_setSIFTParam;
	PageSetParam_SURF *page_setSURFParam;
	PageSetParam_ORB *page_setORBParam;
	//命令提示窗口 控件
	CEdit *command; 
	//CEdit command_runtime;

	CString command_str;
	//基础功能函数
	//数据集.txt 的文件夹路径
	string folder_path;
	string train_folder_path;
	string test_folder_path;
	//数据集中图片的相对路径:*/*.jpg
	vector<string> img_path;
	vector<string> train_img_path;
	vector<string> test_img_path;
	//数据集中图片的类别标签:整数类型 0...n
	vector<int> img_labels;
	vector<int> train_img_labels;
	vector<int> test_img_labels;

	//输入txt的绝对路径，获取 folder_path  img_path  img_labels等信息
	void GetTrainData(string input_file_path);
	void GetTestData(string input_file_path);
	void Process(string input_image_path, string input_image_label);
	void Log(string cmd);
	void Log_Runtime(string cmd);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboFeature();
	afx_msg void OnBnClickedButtonTrain();
	afx_msg void OnBnClickedButtonSetfeatureparam();
	CComboBox feature_process_method;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

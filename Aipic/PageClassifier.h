#pragma once
#include "afxwin.h"
#include"BasicFuncation.h"
#include"FeatureExtract.h"
#include"Classifier.h"
// PageClassifier �Ի���
#include"PageSetParam_SURF.h"
#include"PageSetParam_ORB.h"
#include"PageSetParam_SIFT.h"
class PageClassifier : public CDialogEx
{
	DECLARE_DYNAMIC(PageClassifier)

public:
	PageClassifier(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageClassifier();
	void Init();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TrainClassifier };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTrainbrows();
	afx_msg void OnBnClickedButtonTestbrowse();
	afx_msg void OnBnClickedCheckTest();
	CButton check_box;
private:
	//����
	PageSetParam_SIFT *page_setSIFTParam;
	PageSetParam_SURF *page_setSURFParam;
	PageSetParam_ORB *page_setORBParam;
	//������ʾ���� �ؼ�
	CEdit *command; 
	//CEdit command_runtime;

	CString command_str;
	//�������ܺ���
	//���ݼ�.txt ���ļ���·��
	string folder_path;
	string train_folder_path;
	string test_folder_path;
	//���ݼ���ͼƬ�����·��:*/*.jpg
	vector<string> img_path;
	vector<string> train_img_path;
	vector<string> test_img_path;
	//���ݼ���ͼƬ������ǩ:�������� 0...n
	vector<int> img_labels;
	vector<int> train_img_labels;
	vector<int> test_img_labels;

	//����txt�ľ���·������ȡ folder_path  img_path  img_labels����Ϣ
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

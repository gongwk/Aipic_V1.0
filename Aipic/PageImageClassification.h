#pragma once
#include<string>
#include<iostream>
#include<vector>
#include<iomanip>
#include <windows.h> 
#include "afxwin.h"
#include"BasicFuncation.h"
#include"Caffe.h"
#include"Cluster.h"
#include "afxcmn.h"
using namespace std;
#define BUF_SIZE 1025 
//获取到的caffe classifier result: 序列号 类别号 类别标签 可信度
typedef struct CCR {
	int serial_num;
	int class_id;
	string class_name;
	double accuracy;
}CCR;
// PageImageClassification 对话框

class PageImageClassification : public CDialogEx
{
	DECLARE_DYNAMIC(PageImageClassification)

public:
	PageImageClassification(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageImageClassification();
	void Init();
	void ImportDataSet(vector<string> image_file_path);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ImageClassification };
#endif
private:
	string OnButton_GetFilePath();
	string classifier="null";
	string classifier_define = "null";
	string classifier_label = "null";
	string classifier_mean = "null";
	bool USE_LOADED_DATA = false;
	bool START_CAFFE = false;
	vector<string> labels;
	BasicFuncation bf;
	Caffe caffe;
	Cluster cluster;
	vector<PIC> classifier_result;
	//图片绝对路径 列表
	vector<string> images_absolutepath;
	vector<PIC> classifier_result_temp;
	vector<Cluster::clust> clust_temp;
	//共享内存 获取数据
	// 创建共享文件句柄 
	HANDLE hMapFile;
	char *pBuf;
	string old_message;
	string new_message;
	CCR Analyse(string message);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListLabel();
	afx_msg void OnBnClickedButtonStart();
	CComboBox classifier_type;
	afx_msg void OnBnClickedButtonModelbrowse();
	afx_msg void OnBnClickedButtonDefinebrowse();
	afx_msg void OnBnClickedButtonLablebrowse();
	afx_msg void OnBnClickedButtonImagebrowse();
	afx_msg void OnCbnSelchangeComboClassifiertype();
	afx_msg void OnBnClickedCheckLoadeddata();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic label_text;
	CStatic accuracy_text;
	CStatic classID_text;
	CListBox listbox_label;
	//CListBox listbox_number;
	afx_msg void OnBnClickedButtonStock();
	CProgressCtrl classifier_progress;
	CStatic rate_text;
	CStatic jindu_text;
	CButton loaded_data_checkbox;
	CEdit edit_contral_image_source;
	afx_msg void OnBnClickedButtonMeanbrowse();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

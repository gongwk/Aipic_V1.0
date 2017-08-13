
// AipicDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include<string>
#include "afxcmn.h"
#include "PageFeatrueExtract.h"
#include "PageClassifier.h"
#include "PageDrawNet.h"
#include "PageImageClassification.h"
// CAipicDlg 对话框
class CAipicDlg : public CDialogEx
{
// 构造
public:
	CAipicDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD___DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// //识别的类别
	CStatic rec_ID;
	afx_msg void OnBnClickedExit();
private:
	bool FLAG_IMAGE_CHOOSE;
	bool BUTTUON_CONTRAL_CLUSTER;
	bool FLAG_CONSOLE;
	//窗口： 用以图像特征提取
	PageFeatrueExtract *pageFeatrueExtract;
	//分类器训练
	PageClassifier *pageClassifier;
	//绘制caffe net 图
	PageDrawNet *page_draw_net;
	//
	PageImageClassification page_image_classification;
	CListBox lb_imgPath;
	//每一个图片的绝对路径信息
	vector<string> imagePath;
	//被选择的图片序列号
	int selectedImgIndex;
	//给每个图片贴上背景Mask，归一化
	Mat GetImgMask(Mat image, Size size);
	//获取缓存的图片
	Mat GetImgTemp(int index);
	//当前展示的图片
	Mat current_showed_image;
	Mat current_show_src_image;
public:
	afx_msg void OnBnClickedButtonCluster();
	CStatic static_text_show_label;
	afx_msg void OnBnClickedFolderIn();
	afx_msg void OnBnClickedFolderOut();
	CListBox list_box_cluster;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLbnDblclkListCluster();
	afx_msg void OnBnClickedButtonDerepetit();
	CListBox list_box_cluster_num;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CProgressCtrl progress_bar;
	afx_msg void OnBnClickedButtonRecg();
	afx_msg void ReadDatasetFromFolder();
	afx_msg void SelectOutputFolder();
	afx_msg void StartRecognize();
	afx_msg void StartCluster();
	afx_msg void StartDerepet();
	afx_msg void GUI_Quit();
	afx_msg void OnStnClickedStaticWindowPicshow();
	afx_msg void OnBnClickedPagerecstart();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonLast();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnMenuFeatureextract();
	afx_msg void OnTrainclassifier();
	afx_msg void OnConsole();
	afx_msg void OnCaffeDrawnet();
	afx_msg void OnImageClassification();
};

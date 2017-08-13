#pragma once
#include<opencv2/opencv.hpp>
#include "afxwin.h"
using namespace cv;
// PageSetParam_ORB 对话框

class PageSetParam_ORB : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_ORB)

public:
	typedef struct ORB_Params {
		int 	nfeatures = 500;
		float 	scaleFactor = 1.2f;
		int 	nlevels = 8;
		int 	edgeThreshold = 31;
		int 	firstLevel = 0;
		int 	WTA_K = 2;
		int 	scoreType = cv::ORB::HARRIS_SCORE;
		int 	patchSize = 31;
		int 	fastThreshold = 20;
		cv::Scalar	scalar = cv::Scalar::all(-1);
		int draw_matches_flags = DrawMatchesFlags::DEFAULT;
	}ORB_Param;

	void Init();
	//获取状态参数：
	//true 参数更改了，需要重新计算SURF
	//false 参数未更改，不需要重新计算SURF
	bool GetParamState();
	PageSetParam_ORB::ORB_Params GetParam();
	//设置 param_state 为 false
	void SetParamStateFalse();

	PageSetParam_ORB(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageSetParam_ORB();
private:
	//用途： 参数 是否 已经改变？
	bool param_state = false;

	ORB_Params orb_params;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_ORB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// //排序算法类型
	CComboBox score_type;
	afx_msg void OnBnClickedButtonSetdefault();
	afx_msg void OnBnClickedButtonSetok();
	// //特征点类型
	CComboBox draw_matches_flags;
	// //颜色
	CComboBox scalars;
	afx_msg void OnBnClickedButtonClose();
};

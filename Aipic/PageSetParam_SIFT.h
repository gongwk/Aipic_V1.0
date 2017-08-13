#pragma once
#include "afxwin.h"
#include<opencv2/opencv.hpp>
// PageSetParam_SIFT 对话框

class PageSetParam_SIFT : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_SIFT)

public:
	PageSetParam_SIFT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageSetParam_SIFT();
	typedef struct SIFT_Params {
		int 	nfeatures = 0;
		int 	nOctaveLayers = 3;
		double 	contrastThreshold = 0.04;
		double 	edgeThreshold = 10;
		double	sigma = 1.6;
		cv::Scalar	scalar = cv::Scalar::all(-1);
	}SIFT_Params;
	void Init();
	//获取状态参数：
	//true 参数更改了，需要重新计算SURF
	//false 参数未更改，不需要重新计算SURF
	bool GetParamState();
	PageSetParam_SIFT::SIFT_Params GetParam();
	//设置 param_state 为 false
	void SetParamStateFalse();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_SIFT };
#endif
private:
	//用途： 参数 是否 已经改变？
	bool param_state = false;
	SIFT_Params sift_params;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 特征点颜色
	CComboBox scalars;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonClose();
};

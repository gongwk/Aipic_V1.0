#pragma once
#include "afxwin.h"
//#include "I:\VS2013\VC\atlmfc\include\afxwin.h"

#include<opencv2/opencv.hpp>
// PageSetParam_SURF 对话框

class PageSetParam_SURF : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_SURF)

public:
	typedef struct SURF_Params{
		double 	hessianThreshold=100;
		int 	nOctaves=4;
		int 	nOctaveLayers=3;
		bool 	extended=false;
		bool 	upright=false;
		int		draw_matches_flags = 0;
		cv::Scalar	scalar = cv::Scalar::all(-1);
	}SURF_Param;

	PageSetParam_SURF(CWnd* pParent = NULL);   // 标准构造函数
	//获取状态参数：
	//true 参数更改了，需要重新计算SURF
	//false 参数未更改，不需要重新计算SURF
	bool GetParamState();
	PageSetParam_SURF::SURF_Param GetParam();
	//设置 param_state 为 false
	void SetParamStateFalse();
	virtual ~PageSetParam_SURF();

	void Init();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_SURF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonDefault();
	CComboBox SURFParam_upright;
	CComboBox SURFParam_extend;
private:
	//用途： 参数 是否 已经改变？
	bool param_state=false;

	SURF_Param surf_params;
public:
	CComboBox draw_matches_flags;
	CComboBox scalars;
	afx_msg void OnBnClickedButtonClose();
};

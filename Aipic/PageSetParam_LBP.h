#pragma once
#include "afxwin.h"


// PageSetParam_LBP 对话框
enum LBP_TYPE { LBP_S, LBP_M, LBP_M_LOCAL, LBP_M_LOCAL_WHOLE, LBP_C, LBPC_LOCAL, LBP_C_LOCAL_WHOLE };

class PageSetParam_LBP : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_LBP)
public:
	PageSetParam_LBP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageSetParam_LBP();
	typedef struct LBP_Params {
		int radius = 1;
		int neighbors = 8;
		int lbp_type = LBP_S;
	}LBP_Param;
	void Init();
	//设置 param_state 为 false
	void SetParamStateFalse();
	//获取状态参数：
	//true 参数更改了，需要重新计算SURF
	//false 参数未更改，不需要重新计算SURF
	bool GetParamState();
	PageSetParam_LBP::LBP_Param GetParam();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_LBP };
#endif
private:
	LBP_Param params;
	//用途： 参数 是否 已经改变？
	bool param_state = false;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox lbp_type;
	CEdit lbp_radius;
	CEdit lbp_neighbors;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonClose();
};

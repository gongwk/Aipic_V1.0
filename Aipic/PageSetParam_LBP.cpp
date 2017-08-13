// PageSetParam_LBP.cpp : 实现文件
//

#include "stdafx.h"
#include "Aipic.h"
#include "PageSetParam_LBP.h"
#include "afxdialogex.h"


// PageSetParam_LBP 对话框

IMPLEMENT_DYNAMIC(PageSetParam_LBP, CDialogEx)

PageSetParam_LBP::PageSetParam_LBP(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Param_LBP, pParent)
{

}

PageSetParam_LBP::~PageSetParam_LBP()
{
}

void PageSetParam_LBP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Type, lbp_type);
	DDX_Control(pDX, IDC_EDIT_Radius, lbp_radius);
	DDX_Control(pDX, IDC_EDIT_Neighbors, lbp_neighbors);
}
void PageSetParam_LBP::Init() {
	lbp_type.AddString("LBP_S");
	lbp_type.AddString("LBP_M");
	lbp_type.AddString("LBP_M_LOCAL");
	lbp_type.AddString("LBP_M_LOCAL_WHOLE");
	lbp_type.AddString("LBP_C");
	lbp_type.AddString("LBPC_LOCAL");
	lbp_type.AddString("LBP_C_LOCAL_WHOLE");

	lbp_radius.SetWindowText(_T("1"));
	lbp_neighbors.SetWindowText(_T("8"));

	lbp_type.SetCurSel(0);
}

BEGIN_MESSAGE_MAP(PageSetParam_LBP, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &PageSetParam_LBP::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_Default, &PageSetParam_LBP::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_Close, &PageSetParam_LBP::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// PageSetParam_LBP 消息处理程序


void PageSetParam_LBP::OnBnClickedButtonOk()
{
	CString str;
	lbp_neighbors.GetWindowText(str);
	params.neighbors= _ttoi(str);
	lbp_radius.GetWindowTextA(str);
	params.radius = _ttoi(str);
	lbp_type.GetWindowTextA(str);
	if (str == "LBP_S") {
		params.lbp_type = LBP_S;
	}
	else if (str == "LBP_M") {
		params.lbp_type = LBP_M;
	}
	else if (str == "LBP_M_LOCAL") {
		params.lbp_type = LBP_M_LOCAL;
	}
	else if (str == "LBP_M_LOCAL_WHOLE") {
		params.lbp_type = LBP_M_LOCAL_WHOLE;
	}
	else if (str == "LBP_C") {
		params.lbp_type = LBP_C;
	}
	else if (str == "LBPC_LOCAL") {
		params.lbp_type = LBPC_LOCAL;
	}
	else if (str == "LBP_C_LOCAL_WHOLE") {
		params.lbp_type = LBP_C_LOCAL_WHOLE;
	}
	param_state = true;
}


void PageSetParam_LBP::OnBnClickedButtonDefault()
{
	lbp_type.SetCurSel(0);
	lbp_radius.SetWindowText(_T("1"));
	lbp_neighbors.SetWindowText(_T("8"));
	param_state = true;
}
//设置 param_state 为 false
void PageSetParam_LBP::SetParamStateFalse() {
	param_state = false;
}
//获取状态参数：
//true 参数更改了，需要重新计算SURF
//false 参数未更改，不需要重新计算SURF
bool PageSetParam_LBP::GetParamState() {
	return param_state;
}
PageSetParam_LBP::LBP_Param PageSetParam_LBP::GetParam() {
	return params;
}

void PageSetParam_LBP::OnBnClickedButtonClose()
{
	DestroyWindow();
}

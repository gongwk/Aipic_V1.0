// PageSetParam_SIFT.cpp : 实现文件
//

#include "stdafx.h"
#include "Aipic.h"
#include "PageSetParam_SIFT.h"
#include "afxdialogex.h"


// PageSetParam_SIFT 对话框

IMPLEMENT_DYNAMIC(PageSetParam_SIFT, CDialogEx)

PageSetParam_SIFT::PageSetParam_SIFT(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Param_SIFT, pParent)
{

}

PageSetParam_SIFT::~PageSetParam_SIFT()
{
}
void PageSetParam_SIFT::Init() {
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_nfeatures);
	pBoxOne->SetWindowText(_T("0"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_nOctaveLayers);
	pBoxOne->SetWindowText(_T("3"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_contrastThresold);
	pBoxOne->SetWindowText(_T("0.04"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_edgeThreshold);
	pBoxOne->SetWindowText(_T("10"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_sigma);
	pBoxOne->SetWindowText(_T("1.6"));

	scalars.AddString("随即");
	scalars.AddString("红");
	scalars.AddString("绿");
	scalars.AddString("蓝");
	scalars.AddString("白");
	scalars.AddString("黑");
	scalars.SetCurSel(5);
}
bool PageSetParam_SIFT::GetParamState() {
	return param_state;
}
PageSetParam_SIFT::SIFT_Params PageSetParam_SIFT::GetParam() {
	return sift_params;
}
void PageSetParam_SIFT::SetParamStateFalse() {
	param_state = false;
}
void PageSetParam_SIFT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SIFT_Color, scalars);
}


BEGIN_MESSAGE_MAP(PageSetParam_SIFT, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &PageSetParam_SIFT::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_Default, &PageSetParam_SIFT::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_Close, &PageSetParam_SIFT::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// PageSetParam_SIFT 消息处理程序


void PageSetParam_SIFT::OnBnClickedButtonOk()
{
	CEdit* pBoxOne;
	CString str;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_nfeatures);
	pBoxOne->GetWindowText(str);
	int valueof_nfeatures = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_nOctaveLayers);
	pBoxOne->GetWindowText(str);
	int valueof_nOctaveLayers = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_contrastThresold);
	pBoxOne->GetWindowText(str);
	double valueof_contrastThresold = _ttof(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_edgeThreshold);
	pBoxOne->GetWindowText(str);
	double valueof_edgeThreshold = _ttof(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_sigma);
	pBoxOne->GetWindowText(str);
	double valueof_sigma = _ttof(str);

	sift_params.nfeatures = valueof_nfeatures;
	sift_params.nOctaveLayers = valueof_nOctaveLayers;
	sift_params.contrastThreshold = valueof_contrastThresold;
	sift_params.edgeThreshold = valueof_edgeThreshold;
	sift_params.sigma = valueof_sigma;
	//按下确认键，就说明参数改变了
	param_state = true;
}


void PageSetParam_SIFT::OnBnClickedButtonDefault()
{
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_nfeatures);
	pBoxOne->SetWindowText(_T("0"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_nOctaveLayers);
	pBoxOne->SetWindowText(_T("3"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_contrastThresold);
	pBoxOne->SetWindowText(_T("0.04"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_edgeThreshold);
	pBoxOne->SetWindowText(_T("10"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_SIFT_sigma);
	pBoxOne->SetWindowText(_T("1.6"));
	param_state = true;
}


void PageSetParam_SIFT::OnBnClickedButtonClose()
{
	DestroyWindow();
}

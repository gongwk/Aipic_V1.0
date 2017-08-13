// PageSetParam_ORB.cpp : 实现文件
//

#include "stdafx.h"
#include "Aipic.h"
#include "PageSetParam_ORB.h"
#include "afxdialogex.h"


// PageSetParam_ORB 对话框

IMPLEMENT_DYNAMIC(PageSetParam_ORB, CDialogEx)

PageSetParam_ORB::PageSetParam_ORB(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Param_ORB, pParent)
{

}

PageSetParam_ORB::~PageSetParam_ORB()
{
}

void PageSetParam_ORB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_scoreType, score_type);
	DDX_Control(pDX, IDC_COMBO_featureType, draw_matches_flags);
	DDX_Control(pDX, IDC_COMBO_featureScalar, scalars);
}


BEGIN_MESSAGE_MAP(PageSetParam_ORB, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SetDefault, &PageSetParam_ORB::OnBnClickedButtonSetdefault)
	ON_BN_CLICKED(IDC_BUTTON_SetOK, &PageSetParam_ORB::OnBnClickedButtonSetok)
	ON_BN_CLICKED(IDC_BUTTON_Close, &PageSetParam_ORB::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// PageSetParam_ORB 消息处理程序
void PageSetParam_ORB::Init() {
	score_type.AddString("FAST_SCORE");
	score_type.AddString("HARRIS_SCORE");
	scalars.AddString("随即");
	scalars.AddString("红");
	scalars.AddString("绿");
	scalars.AddString("蓝");
	scalars.AddString("白");
	scalars.AddString("黑");
	draw_matches_flags.AddString("DEFAULT");
	//draw_matches_flags.AddString("DRAW_OVER_OUTIMG");
	draw_matches_flags.AddString("DRAW_RICH_KEYPOINTS");
	//draw_matches_flags.AddString("NOT_DRAW_SINGLE_POINTS");
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nfeatures);
	pBoxOne->SetWindowText(_T("500"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_scaleFactor);
	pBoxOne->SetWindowText(_T("1.2"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nlevels);
	pBoxOne->SetWindowText(_T("8"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_edgeThreshold);
	pBoxOne->SetWindowText(_T("31"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_firstLevel);
	pBoxOne->SetWindowText(_T("0"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_WTA_K);
	pBoxOne->SetWindowText(_T("2"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_patchSize);
	pBoxOne->SetWindowText(_T("31"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_fastThreshold);
	pBoxOne->SetWindowText(_T("20"));

	score_type.SetCurSel(0);
	draw_matches_flags.SetCurSel(0);
	scalars.SetCurSel(5);
}
bool PageSetParam_ORB::GetParamState() {
	return param_state;
}
PageSetParam_ORB::ORB_Params PageSetParam_ORB::GetParam() {
	return orb_params;
}
void PageSetParam_ORB::SetParamStateFalse() {
	param_state = false;
}

void PageSetParam_ORB::OnBnClickedButtonSetdefault()
{
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nfeatures);
	pBoxOne->SetWindowText(_T("500"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_scaleFactor);
	pBoxOne->SetWindowText(_T("1.2"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nlevels);
	pBoxOne->SetWindowText(_T("8"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_edgeThreshold);
	pBoxOne->SetWindowText(_T("31"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_firstLevel);
	pBoxOne->SetWindowText(_T("0"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_WTA_K);
	pBoxOne->SetWindowText(_T("2"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_patchSize);
	pBoxOne->SetWindowText(_T("31"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_fastThreshold);
	pBoxOne->SetWindowText(_T("20"));

	score_type.SetCurSel(0);
	draw_matches_flags.SetCurSel(0);
	scalars.SetCurSel(5);
	param_state = true;
}


void PageSetParam_ORB::OnBnClickedButtonSetok()
{
	//获得EDIT
	CEdit* pBoxOne;
	CString str;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nfeatures);
	pBoxOne->GetWindowText(str);
	orb_params.nfeatures = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_scaleFactor);
	pBoxOne->GetWindowText(str);
	orb_params.scaleFactor = _ttof(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nlevels);
	pBoxOne->GetWindowText(str);
	orb_params.nlevels = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_edgeThreshold);
	pBoxOne->GetWindowText(str);
	orb_params.edgeThreshold = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_firstLevel);
	pBoxOne->GetWindowText(str);
	orb_params.firstLevel = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_WTA_K);
	pBoxOne->GetWindowText(str);
	orb_params.WTA_K = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_patchSize);
	pBoxOne->GetWindowText(str);
	orb_params.patchSize = _ttoi(str);
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_fastThreshold);
	pBoxOne->GetWindowText(str);
	orb_params.fastThreshold = _ttoi(str);

	score_type.GetLBText(score_type.GetCurSel(), str);
	if (str == "FAST_SCORE") {
		orb_params.scoreType = 1;
	}
	else if (str = "HARRIS_SCORE") {
		orb_params.scoreType = 0;
	}
	draw_matches_flags.GetLBText(draw_matches_flags.GetCurSel(), str);
	if (str == "DEFAULT") {
		orb_params.draw_matches_flags = 0;
	}
	else if (str == "DRAW_OVER_OUTIMG") {
		orb_params.draw_matches_flags = 1;
	}
	else if (str == "DRAW_RICH_KEYPOINTS") {
		orb_params.draw_matches_flags = 4;
	}
	else if (str == "NOT_DRAW_SINGLE_POINTS") {
		orb_params.draw_matches_flags = 2;
	}
	scalars.GetLBText(scalars.GetCurSel(), str);
	if (str == "随即") {
		orb_params.scalar = cv::Scalar::all(-1);
	}
	else if (str == "红") {
		orb_params.scalar = cv::Scalar(0, 0, 255);
	}
	else if (str == "绿") {
		orb_params.scalar = cv::Scalar(0, 255, 0);
	}
	else if (str == "蓝") {
		orb_params.scalar = cv::Scalar(255, 0, 0);
	}
	else if (str == "黑") {
		orb_params.scalar = cv::Scalar(0, 0, 0);
	}
	else if (str == "白") {
		orb_params.scalar = cv::Scalar(255, 255, 255);
	}
	param_state = true;
}


void PageSetParam_ORB::OnBnClickedButtonClose()
{
	DestroyWindow();
}

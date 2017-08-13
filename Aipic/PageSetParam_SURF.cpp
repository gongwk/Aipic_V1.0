// PageSetParam_SURF.cpp : 实现文件
//

#include "stdafx.h"
#include "Aipic.h"
#include "PageSetParam_SURF.h"
#include "afxdialogex.h"


// PageSetParam_SURF 对话框

IMPLEMENT_DYNAMIC(PageSetParam_SURF, CDialogEx)

PageSetParam_SURF::PageSetParam_SURF(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Param_SURF, pParent)
{

}

PageSetParam_SURF::~PageSetParam_SURF()
{
}

void PageSetParam_SURF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_upright, SURFParam_upright);
	DDX_Control(pDX, IDC_COMBO_extend, SURFParam_extend);
	DDX_Control(pDX, IDC_COMBO1, draw_matches_flags);
	DDX_Control(pDX, IDC_COMBO_extend2, scalars);
}


BEGIN_MESSAGE_MAP(PageSetParam_SURF, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &PageSetParam_SURF::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_Default, &PageSetParam_SURF::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_Close, &PageSetParam_SURF::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// PageSetParam_SURF 消息处理程序
void PageSetParam_SURF::Init() {

	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_Hessian);
	pBoxOne->SetWindowText(_T("400"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nOctaves);
	pBoxOne->SetWindowText(_T("4"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nOctaveLayers);
	pBoxOne->SetWindowText(_T("3"));


	SURFParam_upright.AddString("true");
	SURFParam_upright.AddString("false");

	SURFParam_extend.AddString("true");
	SURFParam_extend.AddString("false");

	draw_matches_flags.AddString("DEFAULT");
	//draw_matches_flags.AddString("DRAW_OVER_OUTIMG");
	draw_matches_flags.AddString("DRAW_RICH_KEYPOINTS");
	//draw_matches_flags.AddString("NOT_DRAW_SINGLE_POINTS");

	scalars.AddString("随即");
	scalars.AddString("红");
	scalars.AddString("绿");
	scalars.AddString("蓝");
	scalars.AddString("白");
	scalars.AddString("黑");

	SURFParam_extend.SetCurSel(0);
	SURFParam_upright.SetCurSel(0);
	draw_matches_flags.SetCurSel(0);
	scalars.SetCurSel(5);
}

void PageSetParam_SURF::OnBnClickedButtonOk()
{
	//获得EDIT
	CEdit* pBoxOne;
	CString str;

	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_Hessian);
	pBoxOne->GetWindowText(str);
	int valueofhessian = _ttoi(str);
	surf_params.hessianThreshold = valueofhessian;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nOctaves);
	pBoxOne->GetWindowText(str);
	int valueofnOctaves = _ttoi(str);
	surf_params.nOctaves = valueofnOctaves;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nOctaveLayers);
	pBoxOne->GetWindowText(str);
	int valueofnOctaveLayers = _ttoi(str);
	surf_params.nOctaveLayers = valueofnOctaveLayers;

	//SURFParam_extend.GetCurSel();
	SURFParam_extend.GetLBText(SURFParam_extend.GetCurSel(), str);
	if (str == "true") {
		surf_params.extended = true;
	}
	else if (str == "false") {
		surf_params.extended = false;
	}
	SURFParam_upright.GetLBText(SURFParam_upright.GetCurSel(), str);
	if (str == "true") {
		surf_params.upright = true;
	}
	else if (str == "false") {
		surf_params.upright = false;
	}

	draw_matches_flags.GetLBText(draw_matches_flags.GetCurSel(), str);
	if (str == "DEFAULT") {
		surf_params.draw_matches_flags = 0;
	}
	else if (str == "DRAW_OVER_OUTIMG") {
		surf_params.draw_matches_flags = 1;
	}
	else if (str == "DRAW_RICH_KEYPOINTS") {
		surf_params.draw_matches_flags = 4;
	}
	else if (str == "NOT_DRAW_SINGLE_POINTS") {
		surf_params.draw_matches_flags = 2;
	}

	scalars.GetLBText(scalars.GetCurSel(), str);
	if (str == "随即") {
		surf_params.scalar = cv::Scalar::all(-1);
	}
	else if (str == "红") {
		surf_params.scalar = cv::Scalar(0,0,255);
	}
	else if (str == "绿") {
		surf_params.scalar = cv::Scalar(0,255, 0);
	}
	else if (str == "蓝") {
		surf_params.scalar = cv::Scalar(255, 0, 0);
	}
	else if (str == "黑") {
		surf_params.scalar = cv::Scalar(0, 0, 0);
	}
	else if (str == "白") {
		surf_params.scalar = cv::Scalar(255, 255, 255);
	}
	str.ReleaseBuffer();

	//按下确认键，就说明参数改变了
	param_state = true;
}
void PageSetParam_SURF::OnBnClickedButtonDefault()
{
	CEdit* pBoxOne;
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_Hessian);
	pBoxOne-> SetWindowText( _T("400") );
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nOctaves);
	pBoxOne->SetWindowText(_T("4"));
	pBoxOne = (CEdit*)GetDlgItem(IDC_EDIT_nOctaveLayers);
	pBoxOne->SetWindowText(_T("3"));
	SURFParam_extend.SetCurSel(0);
	SURFParam_upright.SetCurSel(0);
	//按下确认键，就说明参数改变了
	param_state = true;
}
bool PageSetParam_SURF::GetParamState() {
	return param_state;
}
void PageSetParam_SURF::SetParamStateFalse() {
    param_state=false;
}
PageSetParam_SURF::SURF_Param PageSetParam_SURF::GetParam() {
	return surf_params;
}

void PageSetParam_SURF::OnBnClickedButtonClose()
{
	DestroyWindow();
}

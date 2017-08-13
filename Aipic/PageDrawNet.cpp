// PageDrawNet.cpp : 实现文件
//

#include "stdafx.h"
#include "Aipic.h"
#include "PageDrawNet.h"
#include "afxdialogex.h"


// PageDrawNet 对话框

IMPLEMENT_DYNAMIC(PageDrawNet, CDialogEx)

PageDrawNet::PageDrawNet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DrawNet, pParent)
{

}

PageDrawNet::~PageDrawNet()
{
}

void PageDrawNet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, caffe_net_prototxt_path);
}


BEGIN_MESSAGE_MAP(PageDrawNet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Browse, &PageDrawNet::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDOK, &PageDrawNet::OnBnClickedOk)
END_MESSAGE_MAP()


// PageDrawNet 消息处理程序


void PageDrawNet::OnBnClickedButtonBrowse()
{
	CString strFile = _T("");

	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.prototxt)|*.prototxt|All Files (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	net_path = strFile;

	caffe_net_prototxt_path.SetWindowText(strFile);
}


void PageDrawNet::OnBnClickedOk()
{
	std::string command = "python " + draw_net_py_path+" " + net_path+" "+"net.jpg";
	system(command.data());
}

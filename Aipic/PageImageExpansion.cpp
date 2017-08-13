// PageImageExpansion.cpp : 实现文件
//

#include "stdafx.h"
#include "Aipic.h"
#include "PageImageExpansion.h"
#include "afxdialogex.h"


// PageImageExpansion 对话框

IMPLEMENT_DYNAMIC(PageImageExpansion, CDialogEx)

PageImageExpansion::PageImageExpansion(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ImageExpansion, pParent)
{

}

PageImageExpansion::~PageImageExpansion()
{
}

void PageImageExpansion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PageImageExpansion, CDialogEx)
END_MESSAGE_MAP()


// PageImageExpansion 消息处理程序

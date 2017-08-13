#pragma once
#include "afxwin.h"
#include<string>

// PageDrawNet 对话框

class PageDrawNet : public CDialogEx
{
	DECLARE_DYNAMIC(PageDrawNet)

public:
	PageDrawNet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageDrawNet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DrawNet };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedOk();
private:
	std::string net_path;
	std::string draw_net_py_path = "tools/draw_net.py";
public:
	CEdit caffe_net_prototxt_path;
};

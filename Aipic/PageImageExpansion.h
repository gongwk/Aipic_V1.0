#pragma once


// PageImageExpansion 对话框

class PageImageExpansion : public CDialogEx
{
	DECLARE_DYNAMIC(PageImageExpansion)

public:
	PageImageExpansion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageImageExpansion();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ImageExpansion };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

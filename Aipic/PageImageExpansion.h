#pragma once


// PageImageExpansion �Ի���

class PageImageExpansion : public CDialogEx
{
	DECLARE_DYNAMIC(PageImageExpansion)

public:
	PageImageExpansion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageImageExpansion();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ImageExpansion };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

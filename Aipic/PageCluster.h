#pragma once
#include"Cluster.h"
#include "afxwin.h"

// PageCluster �Ի���

class PageCluster : public CDialogEx
{
	DECLARE_DYNAMIC(PageCluster)

public:
	PageCluster(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageCluster();
	void SetPath(String path);
// �Ի�������
	//enum { IDD = IDD_DIALOG_Cluster };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	string folder_write_path;
public:
	afx_msg void OnBnClickedPageclusterstart();
	afx_msg void OnBnClickedPageclusterquit();
	// //����ǩ ListBox
	CListBox lb_label;
	// //������� ListBox
	CListBox lb_number;
	afx_msg void OnLbnDblclkListCluster();
};

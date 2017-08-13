#pragma once
#include<vector>
#include "Caffe.h"
#include "afxcmn.h"
using namespace std;
// PageMain �Ի���

class PageMain : public CDialogEx
{
	DECLARE_DYNAMIC(PageMain)

public:
	PageMain(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageMain();
	void SetImgVec(vector<Mat> images);
// �Ի�������
	enum { IDD = IDD_DIALOG_Rec };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	vector<Mat> images;
public:
	afx_msg void OnBnClickedPagerecstart();
	afx_msg void OnBnClickedPagerecquit();
	// //Ŀ��ʶ��ҳ�Ľ�����
	CProgressCtrl pageRecProgress;
};

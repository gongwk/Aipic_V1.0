#pragma once
#include "afxwin.h"
//#include "I:\VS2013\VC\atlmfc\include\afxwin.h"

#include<opencv2/opencv.hpp>
// PageSetParam_SURF �Ի���

class PageSetParam_SURF : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_SURF)

public:
	typedef struct SURF_Params{
		double 	hessianThreshold=100;
		int 	nOctaves=4;
		int 	nOctaveLayers=3;
		bool 	extended=false;
		bool 	upright=false;
		int		draw_matches_flags = 0;
		cv::Scalar	scalar = cv::Scalar::all(-1);
	}SURF_Param;

	PageSetParam_SURF(CWnd* pParent = NULL);   // ��׼���캯��
	//��ȡ״̬������
	//true ���������ˣ���Ҫ���¼���SURF
	//false ����δ���ģ�����Ҫ���¼���SURF
	bool GetParamState();
	PageSetParam_SURF::SURF_Param GetParam();
	//���� param_state Ϊ false
	void SetParamStateFalse();
	virtual ~PageSetParam_SURF();

	void Init();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_SURF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonDefault();
	CComboBox SURFParam_upright;
	CComboBox SURFParam_extend;
private:
	//��;�� ���� �Ƿ� �Ѿ��ı䣿
	bool param_state=false;

	SURF_Param surf_params;
public:
	CComboBox draw_matches_flags;
	CComboBox scalars;
	afx_msg void OnBnClickedButtonClose();
};

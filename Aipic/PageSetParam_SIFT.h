#pragma once
#include "afxwin.h"
#include<opencv2/opencv.hpp>
// PageSetParam_SIFT �Ի���

class PageSetParam_SIFT : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_SIFT)

public:
	PageSetParam_SIFT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageSetParam_SIFT();
	typedef struct SIFT_Params {
		int 	nfeatures = 0;
		int 	nOctaveLayers = 3;
		double 	contrastThreshold = 0.04;
		double 	edgeThreshold = 10;
		double	sigma = 1.6;
		cv::Scalar	scalar = cv::Scalar::all(-1);
	}SIFT_Params;
	void Init();
	//��ȡ״̬������
	//true ���������ˣ���Ҫ���¼���SURF
	//false ����δ���ģ�����Ҫ���¼���SURF
	bool GetParamState();
	PageSetParam_SIFT::SIFT_Params GetParam();
	//���� param_state Ϊ false
	void SetParamStateFalse();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_SIFT };
#endif
private:
	//��;�� ���� �Ƿ� �Ѿ��ı䣿
	bool param_state = false;
	SIFT_Params sift_params;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��������ɫ
	CComboBox scalars;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonClose();
};

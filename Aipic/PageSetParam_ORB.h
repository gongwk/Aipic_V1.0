#pragma once
#include<opencv2/opencv.hpp>
#include "afxwin.h"
using namespace cv;
// PageSetParam_ORB �Ի���

class PageSetParam_ORB : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_ORB)

public:
	typedef struct ORB_Params {
		int 	nfeatures = 500;
		float 	scaleFactor = 1.2f;
		int 	nlevels = 8;
		int 	edgeThreshold = 31;
		int 	firstLevel = 0;
		int 	WTA_K = 2;
		int 	scoreType = cv::ORB::HARRIS_SCORE;
		int 	patchSize = 31;
		int 	fastThreshold = 20;
		cv::Scalar	scalar = cv::Scalar::all(-1);
		int draw_matches_flags = DrawMatchesFlags::DEFAULT;
	}ORB_Param;

	void Init();
	//��ȡ״̬������
	//true ���������ˣ���Ҫ���¼���SURF
	//false ����δ���ģ�����Ҫ���¼���SURF
	bool GetParamState();
	PageSetParam_ORB::ORB_Params GetParam();
	//���� param_state Ϊ false
	void SetParamStateFalse();

	PageSetParam_ORB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageSetParam_ORB();
private:
	//��;�� ���� �Ƿ� �Ѿ��ı䣿
	bool param_state = false;

	ORB_Params orb_params;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_ORB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// //�����㷨����
	CComboBox score_type;
	afx_msg void OnBnClickedButtonSetdefault();
	afx_msg void OnBnClickedButtonSetok();
	// //����������
	CComboBox draw_matches_flags;
	// //��ɫ
	CComboBox scalars;
	afx_msg void OnBnClickedButtonClose();
};

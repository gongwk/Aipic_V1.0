#pragma once
#include "afxwin.h"


// PageSetParam_LBP �Ի���
enum LBP_TYPE { LBP_S, LBP_M, LBP_M_LOCAL, LBP_M_LOCAL_WHOLE, LBP_C, LBPC_LOCAL, LBP_C_LOCAL_WHOLE };

class PageSetParam_LBP : public CDialogEx
{
	DECLARE_DYNAMIC(PageSetParam_LBP)
public:
	PageSetParam_LBP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageSetParam_LBP();
	typedef struct LBP_Params {
		int radius = 1;
		int neighbors = 8;
		int lbp_type = LBP_S;
	}LBP_Param;
	void Init();
	//���� param_state Ϊ false
	void SetParamStateFalse();
	//��ȡ״̬������
	//true ���������ˣ���Ҫ���¼���SURF
	//false ����δ���ģ�����Ҫ���¼���SURF
	bool GetParamState();
	PageSetParam_LBP::LBP_Param GetParam();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Param_LBP };
#endif
private:
	LBP_Param params;
	//��;�� ���� �Ƿ� �Ѿ��ı䣿
	bool param_state = false;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox lbp_type;
	CEdit lbp_radius;
	CEdit lbp_neighbors;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonClose();
};

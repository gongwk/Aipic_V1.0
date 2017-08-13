#pragma once
#include "afxwin.h"
#include"common.h"
#include"PageSetParam_LBP.h"
#include"PageSetParam_SIFT.h"
#include"PageSetParam_SURF.h"
#include"PageSetParam_ORB.h"
#include"FeatureExtract.h"
#include"BasicFuncation.h"
//#include "I:\VS2013\VC\atlmfc\include\afxcmn.h"
// PageFeatrueExtract �Ի���

class PageFeatrueExtract : public CDialogEx
{
	DECLARE_DYNAMIC(PageFeatrueExtract)

public:
	PageFeatrueExtract(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageFeatrueExtract();
	//��ʼ�����ڲ���
	void Init();
	//����ͼƬ
	void SetImage(Mat input_image);
// �Ի�������
	enum { IDD = IDD_DIALOG_FeatrueExtract };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	//������ȡ ʵ����
	FeatureExtract fe;
	BasicFuncation bf;
	//��Ҫ����������ȡ��ͼ��
	Mat image;
	//ͼ����������ӻ�
	Mat image_feature;
	//�Ƿ�ѡ����ĳһ������
	bool enableSIFT = false;
	bool enableSURF = false;
	bool enableORB = false;
	bool enableHOG = false;
	bool enableLBP = false;
	//���������� ���ڳߴ��С
	CRect des_win_rect;
	Mat des_win_bg;
	//����
	PageSetParam_LBP *page_setLBPParam;
	PageSetParam_SIFT *page_setSIFTParam;
	PageSetParam_SURF *page_setSURFParam;
	PageSetParam_ORB *page_setORBParam;
	//��¼�ϴδ򿪵Ĵ���
	vector<string> history_dialog;
	//�ߴ����֧��
	vector<string> scale;
	int scale_cul;//��ǰ�ߴ�

	bool DestroyWindow(string param_name);
	bool DestroyWindow(int param_type);
public:
	// //ͼ������ѡ��
	CComboBox ImgFeature;
	afx_msg void OnBnClickedButtonDoextractfeatrue();
	// //ͼ������ ѡ����
	CTreeCtrl tree_features;
	//ѡ�нڵ�ı�ʱ��������ӦTVN_SELCHANGED֪ͨ��Ϣ��
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//��껮�������ڵ����ĳ�����ڵ�ʱ����ʾ��Ӧ��Tip��ʾ��Ϣ
	afx_msg void OnTvnGetInfoTipTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1Enlargescale();
	afx_msg void OnBnClickedButtonDecreasescale();
};

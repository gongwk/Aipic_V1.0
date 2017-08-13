
// AipicDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include<string>
#include "afxcmn.h"
#include "PageFeatrueExtract.h"
#include "PageClassifier.h"
#include "PageDrawNet.h"
#include "PageImageClassification.h"
// CAipicDlg �Ի���
class CAipicDlg : public CDialogEx
{
// ����
public:
	CAipicDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD___DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// //ʶ������
	CStatic rec_ID;
	afx_msg void OnBnClickedExit();
private:
	bool FLAG_IMAGE_CHOOSE;
	bool BUTTUON_CONTRAL_CLUSTER;
	bool FLAG_CONSOLE;
	//���ڣ� ����ͼ��������ȡ
	PageFeatrueExtract *pageFeatrueExtract;
	//������ѵ��
	PageClassifier *pageClassifier;
	//����caffe net ͼ
	PageDrawNet *page_draw_net;
	//
	PageImageClassification page_image_classification;
	CListBox lb_imgPath;
	//ÿһ��ͼƬ�ľ���·����Ϣ
	vector<string> imagePath;
	//��ѡ���ͼƬ���к�
	int selectedImgIndex;
	//��ÿ��ͼƬ���ϱ���Mask����һ��
	Mat GetImgMask(Mat image, Size size);
	//��ȡ�����ͼƬ
	Mat GetImgTemp(int index);
	//��ǰչʾ��ͼƬ
	Mat current_showed_image;
	Mat current_show_src_image;
public:
	afx_msg void OnBnClickedButtonCluster();
	CStatic static_text_show_label;
	afx_msg void OnBnClickedFolderIn();
	afx_msg void OnBnClickedFolderOut();
	CListBox list_box_cluster;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLbnDblclkListCluster();
	afx_msg void OnBnClickedButtonDerepetit();
	CListBox list_box_cluster_num;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CProgressCtrl progress_bar;
	afx_msg void OnBnClickedButtonRecg();
	afx_msg void ReadDatasetFromFolder();
	afx_msg void SelectOutputFolder();
	afx_msg void StartRecognize();
	afx_msg void StartCluster();
	afx_msg void StartDerepet();
	afx_msg void GUI_Quit();
	afx_msg void OnStnClickedStaticWindowPicshow();
	afx_msg void OnBnClickedPagerecstart();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonLast();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnMenuFeatureextract();
	afx_msg void OnTrainclassifier();
	afx_msg void OnConsole();
	afx_msg void OnCaffeDrawnet();
	afx_msg void OnImageClassification();
};

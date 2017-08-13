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
// PageFeatrueExtract 对话框

class PageFeatrueExtract : public CDialogEx
{
	DECLARE_DYNAMIC(PageFeatrueExtract)

public:
	PageFeatrueExtract(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageFeatrueExtract();
	//初始化窗口参数
	void Init();
	//输入图片
	void SetImage(Mat input_image);
// 对话框数据
	enum { IDD = IDD_DIALOG_FeatrueExtract };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	//特征提取 实现类
	FeatureExtract fe;
	BasicFuncation bf;
	//将要进行特征提取的图像
	Mat image;
	//图像的特征可视化
	Mat image_feature;
	//是否选择了某一个特征
	bool enableSIFT = false;
	bool enableSURF = false;
	bool enableORB = false;
	bool enableHOG = false;
	bool enableLBP = false;
	//特征描述子 窗口尺寸大小
	CRect des_win_rect;
	Mat des_win_bg;
	//参数
	PageSetParam_LBP *page_setLBPParam;
	PageSetParam_SIFT *page_setSIFTParam;
	PageSetParam_SURF *page_setSURFParam;
	PageSetParam_ORB *page_setORBParam;
	//记录上次打开的窗口
	vector<string> history_dialog;
	//尺寸调整支持
	vector<string> scale;
	int scale_cul;//当前尺寸

	bool DestroyWindow(string param_name);
	bool DestroyWindow(int param_type);
public:
	// //图像特征选择
	CComboBox ImgFeature;
	afx_msg void OnBnClickedButtonDoextractfeatrue();
	// //图像特征 选择树
	CTreeCtrl tree_features;
	//选中节点改变时，可以响应TVN_SELCHANGED通知消息。
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//鼠标划过除根节点外的某个树节点时，显示相应的Tip提示信息
	afx_msg void OnTvnGetInfoTipTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1Enlargescale();
	afx_msg void OnBnClickedButtonDecreasescale();
};

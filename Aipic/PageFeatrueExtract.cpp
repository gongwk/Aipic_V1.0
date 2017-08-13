// PageFeatrueExtract.cpp : 实现文件
//

#include "stdafx.h"
#include "Aipic.h"
#include "PageFeatrueExtract.h"
#include "afxdialogex.h"


// PageFeatrueExtract 对话框

IMPLEMENT_DYNAMIC(PageFeatrueExtract, CDialogEx)

PageFeatrueExtract::PageFeatrueExtract(CWnd* pParent /*=NULL*/)
	: CDialogEx(PageFeatrueExtract::IDD, pParent)
{
}
void PageFeatrueExtract::Init(){
	scale.push_back("x 1/4");
	scale.push_back("x 1/3");
	scale.push_back("x 1/2");
	scale.push_back("原图");
	scale.push_back("x 2");
	scale.push_back("x 3");
	scale.push_back("x 4");
	//树 节点参数设置
	tree_features.InsertItem("HOG", NULL, NULL);
	tree_features.InsertItem("SURF", NULL, NULL);
	tree_features.InsertItem("SIFT", NULL, NULL);
	//tree_features.InsertItem("Harris角点", NULL, NULL);
	tree_features.InsertItem("ORB", NULL, NULL);
	tree_features.InsertItem("颜色直方图", NULL, NULL);
	tree_features.InsertItem("LBP", NULL, NULL);
	//-------------------------------------------------------------------------------
	//图像特征展示窗口
	namedWindow("window_page_featureextract", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("window_page_featureextract");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_ImgShowWindow)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	//图像特征描述子展示窗口
	//namedWindow("window_page_feature_descriptor", WINDOW_AUTOSIZE);
	//HWND _hWnd = (HWND)cvGetWindowHandle("window_page_feature_descriptor");
	//HWND _hParent = ::GetParent(_hWnd);
	//::SetParent(_hWnd, GetDlgItem(IDC_STATIC_ImgDes)->m_hWnd);
	//::ShowWindow(_hParent, SW_HIDE);
	//原始图像展示窗口
	namedWindow("window_page_src_picture", WINDOW_AUTOSIZE);
	HWND __hWnd = (HWND)cvGetWindowHandle("window_page_src_picture");
	HWND __hParent = ::GetParent(__hWnd);
	::SetParent(__hWnd, GetDlgItem(IDC_STATIC_SrcPic)->m_hWnd);
	::ShowWindow(__hParent, SW_HIDE);
	//-------------------------------------------------------------------------------
	imshow("window_page_featureextract", image);
	Mat small_image;
	small_image = bf.GetImgMask(image, Size(200, 200));
	CWnd *pWnd = GetDlgItem(IDC_STATIC_SrcPic);//IDC_picture为picture控件ID
	CRect rect;
	pWnd->GetClientRect(&rect);//rc为控件的大小
	resize(small_image,small_image,Size(rect.Width(), rect.Height()));
	imshow("window_page_src_picture", small_image);

	CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_Scale);
	edit->SetWindowText("原图");
	scale_cul = 3;
	SetTimer(1, 30, NULL);
}
void PageFeatrueExtract::SetImage(Mat input_image){
	image = input_image;
}
PageFeatrueExtract::~PageFeatrueExtract()
{
}

void PageFeatrueExtract::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, tree_features);
}


BEGIN_MESSAGE_MAP(PageFeatrueExtract, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON_DoExtractFeatrue, &PageFeatrueExtract::OnBnClickedButtonDoextractfeatrue)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &PageFeatrueExtract::OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_GETINFOTIP, IDC_TREE1, &PageFeatrueExtract::OnTvnGetInfoTipTree1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1_EnlargeScale, &PageFeatrueExtract::OnBnClickedButton1Enlargescale)
	ON_BN_CLICKED(IDC_BUTTON_DecreaseScale, &PageFeatrueExtract::OnBnClickedButtonDecreasescale)
END_MESSAGE_MAP()


// PageFeatrueExtract 消息处理程序
//提取图像特征，并显示
void PageFeatrueExtract::OnBnClickedButtonDoextractfeatrue()
{
}

void PageFeatrueExtract::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;
	//获取窗口尺寸
	CWnd *pWnd = GetDlgItem(IDC_STATIC_ImgShowWindow);//IDC_picture为picture控件ID
	CRect rect_window;
	pWnd->GetClientRect(&rect_window);//rc为控件的大小
	// 树节点的标签文本字符串
	CString strText; 
	// 获取当前选中节点的句柄
	HTREEITEM hItem = tree_features.GetSelectedItem();
	// 获取选中节点的标签文本字符串
	strText = tree_features.GetItemText(hItem);
	// 将字符串显示到编辑框中
	//SetDlgItemText(IDC_BUTTON_SetParams, strText);
	CPaintDC dc(this); // device context for painting
	CRect rc;
	//GetDlgItem(IDC_STATIC_ImgDes)->GetWindowRect(&rc);
	//dc.FillSolidRect(rc, RGB(0, 0, 0));
	GetDlgItem(IDC_STATIC_ImgShowWindow)->GetWindowRect(&rc);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
	Invalidate();
	if (strText == "SURF") {
		enableORB = false;
		enableHOG = false;
		enableSURF = true;
		enableLBP = false;
		if (history_dialog.size()>0&&history_dialog.at(history_dialog.size() - 1)!="SURF") {
			DestroyWindow(history_dialog.at(history_dialog.size() - 1));
		}
		history_dialog.push_back("SURF");
		page_setSURFParam = new PageSetParam_SURF();
		page_setSURFParam->Create(IDD_DIALOG_Param_SURF, this);
		CRect rect;
		GetDlgItem(IDC_STATIC_LocationForParam)->GetWindowRect(&rect);
		ScreenToClient(&rect);   //千万注意别用成ClientToScreen(&rect); 
		page_setSURFParam->MoveWindow(rect);
		page_setSURFParam->ShowWindow(SW_SHOWNORMAL);
		page_setSURFParam->Init();
		
		imshow("window_page_featureextract", bf.GetImgMask(fe.SURF(image),Size(rect_window.Width(), rect_window.Height())));
		//imshow("window_page_feature_descriptor", fe.GetDescriptor().clone());
	}
	else if (strText == "SIFT") {
		enableORB = false;
		enableHOG = false;
		enableSIFT = true;
		enableSURF = false;
		enableLBP = false;
		if (history_dialog.size() > 0 && history_dialog.at(history_dialog.size() - 1) != "SIFT") {
			DestroyWindow(history_dialog.at(history_dialog.size() - 1));
		}
		history_dialog.push_back("SIFT");
		page_setSIFTParam = new PageSetParam_SIFT();
		page_setSIFTParam->Create(IDD_DIALOG_Param_SIFT, this);
		CRect rect;
		GetDlgItem(IDC_STATIC_LocationForParam)->GetWindowRect(&rect);
		ScreenToClient(&rect);   //千万注意别用成ClientToScreen(&rect); 
		page_setSIFTParam->MoveWindow(rect);
		page_setSIFTParam->ShowWindow(SW_SHOWNORMAL);
		page_setSIFTParam->Init();

		imshow("window_page_featureextract", bf.GetImgMask(fe.SIFT(image), Size(rect_window.Width(), rect_window.Height())));
	}
	else if (strText =="ORB") {
		enableORB = true;
		enableHOG = false;
		enableSIFT = false;
		enableSURF = false;		
		enableLBP = false;
		if (history_dialog.size() > 0 && history_dialog.at(history_dialog.size() - 1) != "ORB") {
			DestroyWindow(history_dialog.at(history_dialog.size() - 1));
		}
		history_dialog.push_back("ORB");
		page_setORBParam = new PageSetParam_ORB();
		page_setORBParam->Create(IDD_DIALOG_Param_ORB, this);
		CRect rect;
		GetDlgItem(IDC_STATIC_LocationForParam)->GetWindowRect(&rect);
		ScreenToClient(&rect);   //千万注意别用成ClientToScreen(&rect); 
		page_setORBParam->MoveWindow(rect);
		page_setORBParam->ShowWindow(SW_SHOWNORMAL);
		page_setORBParam->Init();
		imshow("window_page_featureextract", bf.GetImgMask(fe.ORB(image),Size(rect_window.Width(), rect_window.Height())));
	}
	else if (strText == "HOG") {
		enableORB = false;
		enableHOG = true;
		enableSIFT = false;
		enableSURF = false;
		enableLBP = false;
		if (history_dialog.size() > 0 && history_dialog.at(history_dialog.size() - 1) != "HOG") {
			DestroyWindow(history_dialog.at(history_dialog.size() - 1));
		}
		history_dialog.push_back("HOG");
		imshow("window_page_featureextract", bf.GetImgMask(fe.HOG(bf.GetImgMask(image,Size(rect_window.Width(), rect_window.Height()))),Size(rect_window.Width(), rect_window.Height())));
	}
	else if (strText=="颜色直方图") {
		if (history_dialog.size() > 0 && history_dialog.at(history_dialog.size() - 1) != "颜色直方图") {
			DestroyWindow(history_dialog.at(history_dialog.size() - 1));
		}
		history_dialog.push_back("颜色直方图");
		Mat im;
		resize(fe.Hist(image),im,Size(rect_window.Width(), rect_window.Height()));
		imshow("window_page_featureextract",im);
		//Mat hist=fe.GetHistFeature();
		//imshow("hist",hist);
	}
	else if (strText == "LBP") {
		enableORB = false;
		enableHOG = false;
		enableSIFT = false;
		enableSURF = false;
		enableLBP = true;
		if (history_dialog.size() > 0 && history_dialog.at(history_dialog.size() - 1) != "LBP") {
			DestroyWindow(history_dialog.at(history_dialog.size() - 1));
		}
		history_dialog.push_back("LBP");
		page_setLBPParam = new PageSetParam_LBP();
		page_setLBPParam->Create(IDD_DIALOG_Param_LBP, this);
		CRect rect;
		GetDlgItem(IDC_STATIC_LocationForParam)->GetWindowRect(&rect);
		ScreenToClient(&rect);   //千万注意别用成ClientToScreen(&rect); 
		page_setLBPParam->MoveWindow(rect);
		page_setLBPParam->ShowWindow(SW_SHOWNORMAL);
		page_setLBPParam->Init();

		Mat gray;
		cvtColor(image, gray, CV_BGR2GRAY);
		Mat result = fe.LBP(gray, fe.LBP_C, 1, 8);
		imshow("window_page_featureextract", bf.GetImgMask(result, Size(rect_window.Width(), rect_window.Height())));
	}
	else {
	}
}
void PageFeatrueExtract::OnTvnGetInfoTipTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMTVGETINFOTIP* pTVTipInfo = (NMTVGETINFOTIP*)pNMHDR;   // 将传入的pNMHDR转换为NMTVGETINFOTIP指针类型
	HTREEITEM hRoot = tree_features.GetRootItem();      // 获取树的根节点
	CString strText;     // 每个树节点的提示信息
	if (pTVTipInfo->hItem == hRoot) {
		// 如果鼠标划过的节点是根节点，则提示信息为空
		strText = _T("");
	}
	else {
		// 如果鼠标划过的节点不是根节点，则将该节点的附加32位数据格式化为字符串
		strText.Format(_T("%d"), pTVTipInfo->lParam);
	}
	// 将strText字符串拷贝到pTVTipInfo结构体变量的pszText成员中，这样就能显示内容为strText的提示信息
	string str = pTVTipInfo->pszText;
	pTVTipInfo->pszText = "Hello";
}
void PageFeatrueExtract::OnTimer(UINT_PTR nIDEvent)
{
	//获取窗口尺寸
	CWnd *pWnd = GetDlgItem(IDC_STATIC_ImgShowWindow);//IDC_picture为picture控件ID
	CRect rect_window;
	pWnd->GetClientRect(&rect_window);//rc为控件的大小
	if (enableSURF&&page_setSURFParam->GetParamState()) {
		PageSetParam_SURF::SURF_Param params=page_setSURFParam->GetParam();

		//CPaintDC dc(this); // device context for painting
		//CRect rc;
		//GetDlgItem(IDC_STATIC_ImgDes)->GetWindowRect(&rc);
		//dc.FillSolidRect(rc, RGB(0, 0, 0));
		Invalidate();
		imshow("window_page_featureextract", bf.GetImgMask(fe.SURF(image,params.hessianThreshold,params.nOctaves,params.nOctaveLayers,params.extended,params.upright,params.scalar,params.draw_matches_flags),Size(rect_window.Width(), rect_window.Height())));
		//imshow("window_page_feature_descriptor", fe.GetDescriptor().clone());
		page_setSURFParam->SetParamStateFalse();
	}
	else if (enableSIFT&&page_setSIFTParam->GetParamState()) {
		PageSetParam_SIFT::SIFT_Params params = page_setSIFTParam->GetParam();
		Invalidate();
		imshow("window_page_featureextract",
			bf.GetImgMask(
				fe.SIFT(
					image,
					params.nfeatures,
					params.nOctaveLayers,
					params.contrastThreshold,
					params.edgeThreshold,
					params.sigma),
				Size(rect_window.Width(), rect_window.Height())
			)
		);
		page_setSIFTParam->SetParamStateFalse();
	}
	else if (enableORB&&page_setORBParam->GetParamState()) {
		PageSetParam_ORB::ORB_Param params = page_setORBParam->GetParam();

		//CPaintDC dc(this); // device context for painting
		//CRect rc;
		//GetDlgItem(IDC_STATIC_ImgDes)->GetWindowRect(&rc);
		//dc.FillSolidRect(rc, RGB(0, 0, 0));
		Invalidate();
		imshow("window_page_featureextract", 
			bf.GetImgMask(
				fe.ORB(
					image, 
					params.nfeatures, 
					params.scaleFactor, 
					params.nlevels, 
					params.edgeThreshold, 
					params.firstLevel, 
					params.WTA_K, 
					params.scoreType,
					params.patchSize,
					params.fastThreshold,
					params.scalar,
					params.draw_matches_flags),
				Size(rect_window.Width(), rect_window.Height())
				)
			);
		//imshow("window_page_feature_descriptor", fe.GetDescriptor().clone());
		page_setORBParam->SetParamStateFalse();
	}
	else if (enableLBP&&page_setLBPParam->GetParamState()) {
		PageSetParam_LBP::LBP_Param params = page_setLBPParam->GetParam();
		//CPaintDC dc(this); // device context for painting
		//CRect rc;
		//GetDlgItem(IDC_STATIC_ImgDes)->GetWindowRect(&rc);
		//dc.FillSolidRect(rc, RGB(0, 0, 0));
		Invalidate();
		Mat gray;
		cvtColor(image, gray, CV_BGR2GRAY);
		imshow("window_page_featureextract", bf.GetImgMask(fe.LBP(gray, params.lbp_type,params.radius,params.neighbors),Size(rect_window.Width(), rect_window.Height())));
		//imshow("window_page_feature_descriptor", fe.GetDescriptor().clone());
		page_setLBPParam->SetParamStateFalse();
	}
	CDialogEx::OnTimer(nIDEvent);
}
void PageFeatrueExtract::OnBnClickedButton1Enlargescale()
{
	scale_cul++;
	if (scale_cul >= 0 && scale_cul <= scale.size()) {
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_Scale);
		edit->SetWindowText(scale.at(scale_cul).data());
		resize(image, image, Size(image.rows * 2, image.cols * 2));
	}
	imshow("window_page_featureextract", image);
}
void PageFeatrueExtract::OnBnClickedButtonDecreasescale()
{
	scale_cul--;
	if (scale_cul >= 0 && scale_cul <= scale.size()) {
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_Scale);
		edit->SetWindowText(scale.at(scale_cul).data());
		resize(image, image, Size(image.rows / 2, image.cols / 2));
	}
	imshow("window_page_featureextract", image);
}
bool PageFeatrueExtract::DestroyWindow(string param_name) {
	if (param_name == "ORB") {
		page_setORBParam->DestroyWindow();
	}
	else if (param_name == "SURF") {
		page_setSURFParam->DestroyWindow();
	}
	else if (param_name=="SIFT") {
		page_setSIFTParam->DestroyWindow();
	}
	else if (param_name == "LBP") {
		page_setLBPParam->DestroyWindow();
	}
	return true;
}
bool PageFeatrueExtract::DestroyWindow(int param_type) {

	return true;
}
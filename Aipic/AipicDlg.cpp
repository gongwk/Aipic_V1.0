// AipicDlg.cpp : 实现文件
#include "stdafx.h"
#include "Aipic.h"
#include "AipicDlg.h"
#include "afxdialogex.h"
#include "common.h"
#include "Caffe.h"
#include "Cluster.h"
#include "BasicFuncation.h"
#include"ImageMatch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
string folder_read_path;
string folder_write_path;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAipicDlg 对话框



CAipicDlg::CAipicDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAipicDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAipicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, lb_imgPath);

	//DDX_Control(pDX, IDC_ShowLabel, static_text_show_label);
	//DDX_Control(pDX, IDC_LIST_CLuster, list_box_cluster);
	//DDX_Control(pDX, IDC_LIST2, list_box_cluster_num);
	//DDX_Control(pDX, IDC_PROGRESS2, progress_bar);
}

BEGIN_MESSAGE_MAP(CAipicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_EXIT, &CAipicDlg::OnBnClickedExit)		
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_32773, &CAipicDlg::ReadDatasetFromFolder)
	ON_COMMAND(ID_32784, &CAipicDlg::SelectOutputFolder)
	ON_COMMAND(ID_32785, &CAipicDlg::StartRecognize)
	ON_COMMAND(ID_32787, &CAipicDlg::StartCluster)
	ON_COMMAND(ID_32786, &CAipicDlg::StartDerepet)
	ON_COMMAND(ID_32775, &CAipicDlg::GUI_Quit)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAipicDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_Last, &CAipicDlg::OnBnClickedButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_Next, &CAipicDlg::OnBnClickedButtonNext)
	ON_COMMAND(ID_Menu_FeatureExtract, &CAipicDlg::OnMenuFeatureextract)
	ON_COMMAND(ID_TrainClassifier, &CAipicDlg::OnTrainclassifier)
	ON_COMMAND(ID_Console, &CAipicDlg::OnConsole)
	ON_COMMAND(ID_CAFFE_DrawNet, &CAipicDlg::OnCaffeDrawnet)
	ON_COMMAND(ID_ImageClassification, &CAipicDlg::OnImageClassification)
END_MESSAGE_MAP()


// CAipicDlg 消息处理程序

BOOL CAipicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	//pageMain->ShowWindow(SW_SHOWNORMAL);
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	BUTTUON_CONTRAL_CLUSTER = false;
	FLAG_CONSOLE = false;
	SetTimer(1,10,NULL);

	namedWindow("window", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("window");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_Pic_PageMain)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	Mat bg_window(1, 1, CV_8UC3, Scalar(255, 255, 255));
	imshow("window", bg_window);

	namedWindow("window_temp", WINDOW_AUTOSIZE);
	hWnd = (HWND)cvGetWindowHandle("window_temp");
	hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_pageMainPic_temp)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	Mat bg_window_temp(1, 1, CV_8UC3, Scalar(255, 255, 255));
	imshow("window_temp", bg_window_temp);
	//添加按钮图片
	//HBITMAP   hBitmap;
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP6)); // IDB_BITMAP_TEST为资源图片ID 
	//((CButton *)GetDlgItem(IDC_BUTTON_DeRepetit))->SetBitmap(hBitmap);
	//
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP7)); // IDB_BITMAP_TEST为资源图片ID 
	//((CButton *)GetDlgItem(IDC_Folder_Out))->SetBitmap(hBitmap);
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP9)); // IDB_BITMAP_TEST为资源图片ID 
	//((CButton *)GetDlgItem(IDC_BUTTON_CLUSTER))->SetBitmap(hBitmap);
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP10)); // IDB_BITMAP_TEST为资源图片ID 
	//((CButton *)GetDlgItem(IDC_BUTTON_test))->SetBitmap(hBitmap);
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP11)); // IDB_BITMAP_TEST为资源图片ID 
	//((CButton *)GetDlgItem(ID_EXIT))->SetBitmap(hBitmap);
	lb_imgPath.SetHorizontalExtent(1000);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAipicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAipicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		CPaintDC   dc(this);              
		CRect   rect;
		GetClientRect(&rect);		
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1); //IDB_BITMAP是你自己的图对应的ID   ，由于我刚刚加入的位图资源                        
		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAipicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CAipicDlg::OnBnClickedExit()
{
	// TODO:  在此添加控件通知处理程序代码
	if (MessageBox("确认退出", "", MB_YESNO | MB_ICONQUESTION) == IDYES){
		this->SendMessage(WM_CLOSE);
	}
}

void CAipicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnTimer(nIDEvent);
}

HBRUSH CAipicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	//if (nCtlColor == CTLCOLOR_LISTBOX)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}
	if (nCtlColor == CTLCOLOR_BTN)
	{
		CFont m_font;   //声明变量
		m_font.CreatePointFont(250, "宋体"); //设置字体大小和类型
		pDC->SelectObject(&m_font);       //设置字体 
		pDC->SetTextColor(RGB(255, 255, 255)); //设置字体颜色
		pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //不返回画刷
	}
	if (pWnd->GetDlgCtrlID() == IDC_ShowLabel)//可以用CTLCOLOR_STATIC表示静态控件
	{
		CFont m_font;   //声明变量
		m_font.CreatePointFont(250, "宋体"); //设置字体大小和类型
		pDC->SelectObject(&m_font);       //设置字体 
		pDC->SetTextColor(RGB(255, 255, 255)); //设置字体颜色
		pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //不返回画刷
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TitleText)//可以用CTLCOLOR_STATIC表示静态控件
	{
		CFont m_font;   //声明变量
		m_font.CreatePointFont(300, "华文行楷"); //设置字体大小和类型
		pDC->SelectObject(&m_font);       //设置字体 
		pDC->SetTextColor(RGB(255, 255, 255)); //设置字体颜色
		pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //不返回画刷
	}
	//if (pWnd->GetDlgCtrlID() == IDC_STATIC_Window_Picshow || pWnd->GetDlgCtrlID() == IDC_STATIC || pWnd->GetDlgCtrlID() == IDC_STATIC_Text_Num)//可以用CTLCOLOR_STATIC表示静态控件
	//{
	//	CFont m_font;   //声明变量
	//	m_font.CreatePointFont(100, "宋体"); //设置字体大小和类型
	//	pDC->SelectObject(&m_font);       //设置字体 
	//	pDC->SetTextColor(RGB(255, 255, 255)); //设置字体颜色
	//	pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH); //不返回画刷
	//}
	// TODO:  在此更改 DC 的任何特性
	//if (nCtlColor == CTLCOLOR_STATIC){
	//	pDC->SetTextColor(RGB(255, 255, 255)); //设置字体为白色
	//	pDC->SetBkMode(TRANSPARENT);         //设置背景为透明
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	//}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CAipicDlg::ReadDatasetFromFolder()
{
	// TODO:  在此添加命令处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	TCHAR   szFolderPath[MAX_PATH] = { 0 };
	CString strFolderPath = TEXT("");

	BROWSEINFO sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;
	//显示文件夹选择对话框
	LPITEMIDLIST lpidBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidBrowse != NULL) {
		//取得文件夹名
		if (::SHGetPathFromIDList(lpidBrowse, szFolderPath)) {
			strFolderPath = szFolderPath;
			folder_read_path = strFolderPath;
		}
	}
	if (lpidBrowse != NULL) {
		::CoTaskMemFree(lpidBrowse);
	}
	//显示图片绝对路径信息到ListBox上。
	BasicFuncation basic;
	imagePath = basic.GetIndex(folder_read_path + "\\");
	for (int i = 0; i < imagePath.size(); i++){
		lb_imgPath.AddString(imagePath.at(i).c_str());
	}

}


void CAipicDlg::SelectOutputFolder()
{
	// TODO: 在此添加控件通知处理程序代码
	string path;
	TCHAR   szFolderPath[MAX_PATH] = { 0 };
	CString strFolderPath = TEXT("");

	BROWSEINFO sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;
	//显示文件夹选择对话框
	LPITEMIDLIST lpidBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidBrowse != NULL) {
		//取得文件夹名
		if (::SHGetPathFromIDList(lpidBrowse, szFolderPath)) {
			strFolderPath = szFolderPath;
			folder_write_path = strFolderPath;
		}
	}
	if (lpidBrowse != NULL) {
		::CoTaskMemFree(lpidBrowse);
	}
}


void CAipicDlg::StartRecognize()
{
}
void CAipicDlg::StartCluster()
{
}
void CAipicDlg::OnMenuFeatureextract()
{
	pageFeatrueExtract = new PageFeatrueExtract();
	pageFeatrueExtract->Create(IDD_DIALOG_FeatrueExtract, this);
	Mat image;
	double scale = 1.0;
	if (current_show_src_image.rows > current_show_src_image.cols) {
		scale = 240/(double)current_show_src_image.rows ;
	}
	else {
		scale = 240/(double)current_show_src_image.cols ;
	}
	cv::resize(current_show_src_image, image,Size((int)current_show_src_image.cols*scale, (int)current_show_src_image.rows*scale));
	if (!current_show_src_image.empty()) {
		pageFeatrueExtract->ShowWindow(SW_SHOWNORMAL);
		pageFeatrueExtract->SetImage(current_show_src_image);
		pageFeatrueExtract->Init();
	}
	else {
	}
}


void CAipicDlg::StartDerepet()
{
}


void CAipicDlg::GUI_Quit()
{
	// TODO:  在此添加控件通知处理程序代码
	if (MessageBox("确认退出", "", MB_YESNO | MB_ICONQUESTION) == IDYES){
		this->SendMessage(WM_CLOSE);
	}
}

void CAipicDlg::OnLbnSelchangeList1()
{
	int nIndex = lb_imgPath.GetCurSel();
	selectedImgIndex = nIndex;
	if (nIndex >= 0) {
		CString cs;
		lb_imgPath.GetText(nIndex, cs);
		string str = imagePath.at(nIndex);
		// 在这里操作
		Mat image = imread((string)cs);
		current_showed_image = image;
		current_show_src_image = image.clone();
		CWnd *pWnd = GetDlgItem(IDC_Pic_PageMain);//IDC_picture为picture控件ID
		CRect rect;
		pWnd->GetClientRect(&rect);//rc为控件的大小
		imshow("window", GetImgMask(image, Size(rect.Height(), rect.Width())));
		waitKey(10);
	}
}


void CAipicDlg::OnBnClickedButtonLast()
{
	selectedImgIndex--;
	if (selectedImgIndex >= 0&& selectedImgIndex<imagePath.size()) {
		string str = imagePath.at(selectedImgIndex);
		// 在这里操作
		Mat image = imread(str);
		current_showed_image = image;
		current_show_src_image = image.clone();
		CWnd *pWnd = GetDlgItem(IDC_Pic_PageMain);//IDC_picture为picture控件ID
		CRect rect;
		pWnd->GetClientRect(&rect);//rc为控件的大小
		imshow("window", GetImgMask(image, Size(rect.Height(), rect.Width())));
		//判断当前的图片位置
		if (selectedImgIndex < (imagePath.size() - 8))
		{
			Mat image_temp;
			cv::resize(GetImgTemp(selectedImgIndex), image_temp, Size(540, 74));
			imshow("window_temp", image_temp);
		}
		else if (selectedImgIndex >(imagePath.size() - 8)) {
			Mat image_temp;
			cv::resize(GetImgTemp(imagePath.size() - 8), image_temp, Size(540, 74));
			imshow("window_temp", image_temp);
		}
	}
	else {
		MessageBox("已经到达列表顶部！");
		selectedImgIndex =0;
	}
}
void CAipicDlg::OnBnClickedButtonNext()
{
	selectedImgIndex++;
	if (selectedImgIndex>=0&&selectedImgIndex < imagePath.size()) {
		string str = imagePath.at(selectedImgIndex);
		CWnd *pWnd = GetDlgItem(IDC_Pic_PageMain);//IDC_picture为picture控件ID
		CRect rect;
		pWnd->GetClientRect(&rect);//rc为控件的大小
		// 在这里操作
		Mat image = imread(str);
		current_showed_image = image;
		current_show_src_image = image.clone();
		imshow("window", GetImgMask(image, Size(rect.Height(), rect.Width())));
		if (selectedImgIndex > 0 && selectedImgIndex < (imagePath.size() - 8))
		{
			Mat image_temp;
			cv::resize(GetImgTemp(selectedImgIndex), image_temp, Size(540, 74));
			imshow("window_temp", image_temp);
		}
		else if (selectedImgIndex > (imagePath.size() - 8)) {
			Mat image_temp;
			cv::resize(GetImgTemp(imagePath.size() - 8), image_temp, Size(540, 74));
			imshow("window_temp", image_temp);
		}
		waitKey(10);
	}
	else {
		MessageBox("已经到达列表底部！");
		selectedImgIndex = imagePath.size()-1;
	}
}
Mat CAipicDlg::GetImgMask(Mat image, Size size){
	int s;
	if (image.rows > image.cols) {
		s = image.rows;
	}
	else {
		s = image.cols;
	}
	Mat image_temp_ep(s, s, CV_8UC3, Scalar(255, 255, 255));
	Mat image_temp_ep_roi = image_temp_ep(Rect((s - image.cols) / 2, (s - image.rows) / 2, image.cols, image.rows));
	Mat dstNormImg;
	cv::addWeighted(image_temp_ep_roi, 0., image, 1.0, 0., image_temp_ep_roi);
	cv::resize(image_temp_ep, dstNormImg, size, 0, 0, 1);    //大小归一化
	return dstNormImg;
}
Mat CAipicDlg::GetImgTemp(int index){
	Mat image_temp_ep;
	if (imagePath.size() < 8) {
		image_temp_ep=Mat(55, 55*imagePath.size(), CV_8UC3, Scalar(255, 255, 255));
	}
	else {
		image_temp_ep = Mat(55, 475, CV_8UC3, Scalar(255, 255, 255));
	}
	Mat result;
	for (int i = 0; i < 8&&i<imagePath.size(); i++){
		string str = imagePath.at(index+i);
		Mat image = imread(str);
		Mat img;
		image=GetImgMask(image, Size(270, 270));
		cv::resize(image, img, Size(55,55), 0, 0, 1);    //大小归一化
		Mat image_temp_ep_roi = image_temp_ep(Rect(i* 60,0, 55, 55));
		cv::addWeighted(image_temp_ep_roi, 0., img, 1.0, 0., image_temp_ep_roi);
	}
	return image_temp_ep;
}
void CAipicDlg::OnTrainclassifier()
{
	pageClassifier = new PageClassifier();
	pageClassifier->Create(IDD_DIALOG_TrainClassifier, this);
	pageClassifier->ShowWindow(SW_SHOWNORMAL);
	pageClassifier->Init();
}
void CAipicDlg::OnConsole()
{
	if (!FLAG_CONSOLE) {
		FLAG_CONSOLE = true;
		GetDlgItem(ID_Console);
		AllocConsole();
		freopen("CONOUT$", "w+t", stdout);
		freopen("CONIN$", "r+t", stdin);
	}
	else if (FLAG_CONSOLE) {
		FLAG_CONSOLE = false;
		FreeConsole();
		MessageBox("请于现在关闭cmd窗口！");
	}
}


void CAipicDlg::OnCaffeDrawnet()
{
	page_draw_net = new PageDrawNet();
	page_draw_net->Create(IDD_DIALOG_DrawNet, this);
	page_draw_net->ShowWindow(SW_SHOWNORMAL);
}


void CAipicDlg::OnImageClassification()
{
	page_image_classification.Create(IDD_DIALOG_ImageClassification);
	page_image_classification.ShowWindow(SW_SHOWNORMAL);
	page_image_classification.ImportDataSet(imagePath);
	page_image_classification.Init();
}

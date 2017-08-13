// PageImageClassification.cpp : 实现文件
#include "stdafx.h"
#include "Aipic.h"
#include "PageImageClassification.h"
#include "afxdialogex.h"
char szName[] = "NameOfMappingObject";    // 共享内存的名字  
#define CC L"tools\\_CaffeSeg.exe"  
// PageImageClassification 对话框
IMPLEMENT_DYNAMIC(PageImageClassification, CDialogEx)
PageImageClassification::PageImageClassification(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ImageClassification, pParent)
{

}
PageImageClassification::~PageImageClassification()
{
}
void PageImageClassification::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ClassifierType, classifier_type);
	DDX_Control(pDX, IDC_STATIC_Label, label_text);
	DDX_Control(pDX, IDC_STATIC_Accuracy, accuracy_text);
	DDX_Control(pDX, IDC_STATIC_ID, classID_text);
	DDX_Control(pDX, IDC_LIST_Label, listbox_label);
	//DDX_Control(pDX, IDC_LIST_LabelNumber, listbox_number);
	DDX_Control(pDX, IDC_PROGRESS, classifier_progress);
	//DDX_Control(pDX, IDC_RateText, rate_text);
	DDX_Control(pDX, IDC_STATIC_jindu, jindu_text);
	DDX_Control(pDX, IDC_CHECK_LoadedData, loaded_data_checkbox);
	DDX_Control(pDX, IDC_EDIT_ImageSource, edit_contral_image_source);
}
BEGIN_MESSAGE_MAP(PageImageClassification, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Start, &PageImageClassification::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_ModelBrowse, &PageImageClassification::OnBnClickedButtonModelbrowse)
	ON_BN_CLICKED(IDC_BUTTON_DefineBrowse, &PageImageClassification::OnBnClickedButtonDefinebrowse)
	ON_BN_CLICKED(IDC_BUTTON_LableBrowse, &PageImageClassification::OnBnClickedButtonLablebrowse)
	ON_BN_CLICKED(IDC_BUTTON_ImageBrowse, &PageImageClassification::OnBnClickedButtonImagebrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_ClassifierType, &PageImageClassification::OnCbnSelchangeComboClassifiertype)
	ON_BN_CLICKED(IDC_CHECK_LoadedData, &PageImageClassification::OnBnClickedCheckLoadeddata)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Stock, &PageImageClassification::OnBnClickedButtonStock)
	ON_BN_CLICKED(IDC_BUTTON_MeanBrowse, &PageImageClassification::OnBnClickedButtonMeanbrowse)
	ON_BN_CLICKED(IDCANCEL, &PageImageClassification::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
// PageImageClassification 消息处理程序
void PageImageClassification::Init() {
	classifier_type.AddString("Caffe");
	classifier_type.AddString("OpenCV_ML");
	classifier_type.SetCurSel(0);
	namedWindow("image", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("image");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_Image)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	SetTimer(1, 10, NULL);
}
void PageImageClassification::OnBnClickedButtonStart()
{
	if (images_absolutepath.size() == 0) {
		MessageBox("未发现有效图片，请导入图片后开启本页面或自行加载数据");
		return;
	}
	//获取当前参数
	CEdit *edit_contral;
	CString str;
	CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_COMBO_ClassifierType);
	CString type;
	combobox->GetWindowTextA(type);
	edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierModelFile);
	edit_contral->GetWindowText(str);
	classifier = str;
	edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierLabelFile);
	edit_contral->GetWindowText(str);
	classifier_label = str;
	if (classifier.size() < 2 || classifier_label.size() < 2) {
		MessageBox(" 参数设置错误 ");
		return;
	}
	classifier_result_temp.clear();
	clust_temp.clear();
	classifier_progress.SetRange(0, images_absolutepath.size());
	classifier_progress.SetStep(1);
	if (type == "Caffe") {
		edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierDefineFile);
		edit_contral->GetWindowText(str);
		classifier_define = str;
		if (classifier_define.size() < 2) {
			MessageBox("参数设置错误");
			return;
		}
		STARTUPINFO si;
		GetStartupInfo(&si);
		PROCESS_INFORMATION pi;
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		string command = " "+classifier_define + " " + classifier + " " + classifier_mean + " " + classifier_label+" "+"image_abs_path.txt";
		cout << command << endl;
		LPTSTR lp = const_cast<char*>(command.c_str());
		if (!CreateProcess("tools\\_CaffeSeg.exe", lp, NULL, NULL, FALSE, CREATE_NO_WINDOW,
			NULL, NULL, &si, &pi)) {
			wprintf(L"CreateProcess error:%d\n", GetLastError());
			cout << "error" << endl;
		}
		START_CAFFE = true;
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // 物理文件句柄  
			NULL,                    // 默认安全级别  
			PAGE_READWRITE,          // 可读可写  
			0,                       // 高位文件大小  
			BUF_SIZE,                // 地位文件大小  
			szName                   // 共享内存名称  
			);
		pBuf = (char *)MapViewOfFile(
			hMapFile,            // 共享内存的句柄  
			FILE_MAP_ALL_ACCESS, // 可读写许可  
			0,
			0,
			BUF_SIZE
			);
	}
	else if (type == "OpenCV_ML") {
		//MessageBox("OpenCV_ML");

	}	
}
void PageImageClassification::OnBnClickedButtonModelbrowse()
{
	CEdit *edit_contral;
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.caffemodel)|*.caffemodel|*.xml|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierModelFile);
	edit_contral->SetWindowText(strFile);
	classifier = strFile;
}
void PageImageClassification::OnBnClickedButtonDefinebrowse()
{
	CEdit *edit_contral;
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.prototxt)|*.prototxt|*.xml|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierDefineFile);
	edit_contral->SetWindowText(strFile);
	classifier_define = strFile;
}
void PageImageClassification::OnBnClickedButtonLablebrowse()
{
	CEdit *edit_contral;
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.txt)|*.txt|*.txt|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierLabelFile);
	edit_contral->SetWindowText(strFile);
	classifier_label = strFile;
}
void PageImageClassification::OnBnClickedButtonImagebrowse()
{
	//图像路径保存
	fstream imagePathFile;
	imagePathFile.open("image_abs_path.txt", ios::out);
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
		}
	}
	if (lpidBrowse != NULL) {
		::CoTaskMemFree(lpidBrowse);
	}
	string search_path = strFolderPath;
	GetDlgItem(IDC_EDIT_ImageSource)->SetWindowText(strFolderPath);
	bf.GetAllFormatFiles(search_path, images_absolutepath, "jpg");
	for (int i = 0; i < images_absolutepath.size(); i++) {
		images_absolutepath[i] = search_path + "\\" + images_absolutepath[i];
		cout << images_absolutepath.at(i) << endl;
		imagePathFile<< images_absolutepath.at(i) << endl;
	}
	imagePathFile.close();
	cout << "文件读取完毕" << endl;
}
void PageImageClassification::OnBnClickedButtonMeanbrowse()
{
	CEdit *edit_contral;
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.binaryproto)|*.binaryproto|*.binaryproto|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierMeanFile);
	edit_contral->SetWindowText(strFile);
	classifier_mean = strFile;
}
string PageImageClassification::OnButton_GetFilePath() {
	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	return strFile;
}
void PageImageClassification::OnCbnSelchangeComboClassifiertype()
{
}
void PageImageClassification::OnBnClickedCheckLoadeddata()
{
	CEdit *edit_contral;
	edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ImageSource);
	CButton *button;
	button = (CButton*)GetDlgItem(IDC_BUTTON_ImageBrowse);
	CButton *check_box;
	check_box = (CButton*)GetDlgItem(IDC_CHECK_LoadedData);
	if (BST_CHECKED == check_box->GetCheck()) {
		edit_contral->EnableWindow(FALSE);
		button->EnableWindow(FALSE);
		USE_LOADED_DATA = true;
		if (images_absolutepath.size() == 0) {
			MessageBox("未发现有效图片，请导入图片后开启本页面或自行加载数据");
		}
	}
	if (BST_UNCHECKED == check_box->GetCheck()) {
		edit_contral->EnableWindow(TRUE);
		button->EnableWindow(TRUE);
		USE_LOADED_DATA = false;
	}
}
void PageImageClassification::OnTimer(UINT_PTR nIDEvent)
{
	CComboBox *combobox = (CComboBox*)GetDlgItem(IDC_COMBO_ClassifierType);
	CEdit *edit_contral;
	CString type;
	CButton *button;
	CWnd *pWnd = GetDlgItem(IDC_Image);//IDC_picture为picture控件ID
	CRect rect_window;
	pWnd->GetClientRect(&rect_window);//rc为控件的大小
	combobox->GetWindowTextA(type);
	if (type == "OpenCV_ML") {
		edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierDefineFile);
		edit_contral->EnableWindow(FALSE);
		button = (CButton*)GetDlgItem(IDC_BUTTON_DefineBrowse);
		button->EnableWindow(FALSE);
		edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierMeanFile);
		edit_contral->EnableWindow(FALSE);
		button = (CButton*)GetDlgItem(IDC_BUTTON_MeanBrowse);
		button->EnableWindow(FALSE);
	}
	else if (type == "Caffe") {
		edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierDefineFile);
		edit_contral->EnableWindow(TRUE);
		button = (CButton*)GetDlgItem(IDC_BUTTON_DefineBrowse);
		button->EnableWindow(TRUE);
		edit_contral = (CEdit*)GetDlgItem(IDC_EDIT_ClassifierMeanFile);
		edit_contral->EnableWindow(TRUE);
		button = (CButton*)GetDlgItem(IDC_BUTTON_MeanBrowse);
		button->EnableWindow(TRUE);
	}
	bool START_COUNT = false;
	if (START_CAFFE) {
		new_message = pBuf;
		if (new_message != old_message) {
			old_message = pBuf;
			CCR ccr=Analyse(new_message);
			cout << ccr.serial_num << "+" << ccr.class_id << "+" << ccr.class_name << "+" << ccr.accuracy << endl;
			Mat image = imread(images_absolutepath.at(ccr.serial_num));
			label_text.SetWindowText(ccr.class_name.data());
			accuracy_text.SetWindowText(bf.Transform_Double2String(ccr.accuracy).data());
			classID_text.SetWindowText((bf.Transform_Int2String(ccr.class_id)).data());
			resize(image, image, Size(rect_window.Width(), rect_window.Height()));
			imshow("image",image);
			classifier_result.push_back(PIC{ images_absolutepath.at(ccr.serial_num), ccr.class_name });
			classifier_progress.StepIt();
			double rate = ccr.serial_num / images_absolutepath.size()*100;
			string text = bf.Transform_Int2String(ccr.serial_num+1) + "/" + bf.Transform_Int2String(images_absolutepath.size());
			jindu_text.SetWindowTextA(text.data());
			if (ccr.serial_num == (images_absolutepath.size()-1)) {
				START_COUNT = true;
			}
		}
	}
	if (START_COUNT) {
		cluster.Sort(classifier_result, classifier_result_temp);
		cluster.Count(classifier_result_temp, clust_temp);
		for (int i = 0; i < clust_temp.size(); i++) {
			string num = bf.Transform_Int2String(clust_temp.at(i).number);
			string label = clust_temp.at(i).cluster_name;
			string str1, str2;
			bf.NormalizeString(label, 14, str1);
			bf.NormalizeString(num, 5, str2);
			string str = label +"( " +num + " )";
			listbox_label.AddString(str.c_str());
		}
		START_COUNT = false;
	}
	CDialogEx::OnTimer(nIDEvent);
}
CCR PageImageClassification::Analyse(string message) {
	CCR ccr;
	vector<string> str_list_1=bf.split(message, ":");
	ccr.serial_num = bf.Transform_String2int(str_list_1.at(0));
	vector<string> str_list_2 = bf.split(str_list_1.at(1)," ");
	for (int i = 0; i < str_list_2.size();i++) {
		vector<string> str_list_3 = bf.split(str_list_2.at(i),"=");
		if (str_list_3.at(0) == "id") {
			ccr.class_id = bf.Transform_String2int(str_list_3.at(1));
		}
		else if (str_list_3.at(0) == "name") {
			ccr.class_name = str_list_3.at(1);
		}
		else if (str_list_3.at(0) == "accuracy") {
			ccr.accuracy = bf.Transform_String2Double(str_list_3.at(1));
		}
	}
	return ccr;
	//cout << ccr.serial_num << "+" << ccr.class_id << "+" << ccr.class_name << "+" << ccr.accuracy << endl;
}
void PageImageClassification::ImportDataSet(vector<string> image_file_path) {
	images_absolutepath = image_file_path;
}

void PageImageClassification::OnBnClickedButtonStock()
{
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
		}
	}
	if (lpidBrowse != NULL) {
		::CoTaskMemFree(lpidBrowse);
	}
	string stock_path = strFolderPath;
	cluster.Stock(stock_path, classifier_result_temp, clust_temp);
}




void PageImageClassification::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
	START_CAFFE = false;

	CDialogEx::OnCancel();
}


HBRUSH PageImageClassification::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

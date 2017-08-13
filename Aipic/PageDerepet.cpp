// PageDerepet.cpp : 实现文件
//

#include "stdafx.h"
#include "基于深度学习的智能图像分类系统.h"
#include "PageDerepet.h"
#include "afxdialogex.h"


// PageDerepet 对话框

IMPLEMENT_DYNAMIC(PageDerepet, CDialogEx)

PageDerepet::PageDerepet(CWnd* pParent /*=NULL*/)
	: CDialogEx(PageDerepet::IDD, pParent)
{

}

PageDerepet::~PageDerepet()
{
}

void PageDerepet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PageDerepet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PageDerepet_StartDerepet, &PageDerepet::OnBnClickedButtonPagederepetStartderepet)
END_MESSAGE_MAP()


// PageDerepet 消息处理程序


void PageDerepet::OnBnClickedButtonPagederepetStartderepet()
{
	namedWindow("pageDerepetWindow", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("pageDerepetWindow");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PageDerepet_ImgWin)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	//AllocConsole();
	//freopen("CONOUT$", "w+t", stdout);
	//freopen("CONIN$", "r+t", stdin);
	// TODO:  在此添加控件通知处理程序代码
	string fold_path = folderPath + "/folder_name.txt";
	fstream file;
	vector<string> folder_path;
	string str;
	file.open(fold_path, ios::in);
	while (getline(file, str)){
		//cout << str.size() << endl;
		//cout << "正在对" << str << "类别的图像进行去重工作！" << endl;
		str = folderPath + "\\" + str+"\\";
		vector<string> image_path=base_funcation.GetPath(str);
		//cout << image_path.size() << endl;

		for (int i = 0; i < image_path.size(); i++){
			for (int j = i + 1; j < image_path.size(); j++){
				Mat image1 = imread(image_path.at(i));
				Mat image2 = imread(image_path.at(j));

				match.match(image1, image2, match.SURF);
				Mat matchImg = match.GetMatchedImage();
				if (match.getSimilarity() < 0.1){
					//system((char*)("del " + image_path.at(j)).data());
					//cout << "图片路径：" << image_path.at(j) << "  已删除！" << endl;
				}
				
				imshow("pageDerepetWindow", base_funcation.GetImgMask(matchImg, Size(200, 200)));
				waitKey(10);
			}
		}
		//for (int i = 0; i < image_path.size(); i++){
		//	Mat image = imread(image_path.at(i));
		//	imshow("pageDerepetWindow", image);
		//	waitKey(10);
		//	//cout << image_path.at(i) << endl;
		//}
	}
}
void PageDerepet::SetFolderPath(string search_path){
	folderPath = search_path;
}
// PageDerepet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�������ѧϰ������ͼ�����ϵͳ.h"
#include "PageDerepet.h"
#include "afxdialogex.h"


// PageDerepet �Ի���

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


// PageDerepet ��Ϣ�������


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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	string fold_path = folderPath + "/folder_name.txt";
	fstream file;
	vector<string> folder_path;
	string str;
	file.open(fold_path, ios::in);
	while (getline(file, str)){
		//cout << str.size() << endl;
		//cout << "���ڶ�" << str << "����ͼ�����ȥ�ع�����" << endl;
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
					//cout << "ͼƬ·����" << image_path.at(j) << "  ��ɾ����" << endl;
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
// PageCluster.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "�������ѧϰ������ͼ�����ϵͳ.h"
#include "PageCluster.h"
#include "afxdialogex.h"


// PageCluster �Ի���

IMPLEMENT_DYNAMIC(PageCluster, CDialogEx)

PageCluster::PageCluster(CWnd* pParent /*=NULL*/)
	: CDialogEx(PageCluster::IDD, pParent)
{

}

PageCluster::~PageCluster()
{
}

void PageCluster::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLuster, lb_label);
	DDX_Control(pDX, IDC_LIST2, lb_number);
}


BEGIN_MESSAGE_MAP(PageCluster, CDialogEx)
	ON_BN_CLICKED(IDC_PageClusterStart, &PageCluster::OnBnClickedPageclusterstart)
	ON_BN_CLICKED(IDC_PageClusterQuit, &PageCluster::OnBnClickedPageclusterquit)
	ON_LBN_DBLCLK(IDC_LIST_CLuster, &PageCluster::OnLbnDblclkListCluster)
END_MESSAGE_MAP()


// PageCluster ��Ϣ�������


void PageCluster::OnBnClickedPageclusterstart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Cluster cluster;
	cluster.cluster(folder_write_path);
	vector<Cluster::clust> clust_message = cluster.getClusterMessage();
	for (int i = 0; i < clust_message.size(); i++) {
		stringstream ss;
		string str;
		ss << clust_message.at(i).number;
		ss >> str;
		lb_label.AddString((clust_message.at(i).cluster_name).c_str());
		lb_number.AddString(str.c_str());
	}
	system((char*)("dir " + folder_write_path + "\ / b > " + folder_write_path + "/folder_name.txt").data());
}

void PageCluster::SetPath(String path){
	folder_write_path = path;
}
void PageCluster::OnBnClickedPageclusterquit()
{
	if (MessageBox("ȷ���˳�", "", MB_YESNO | MB_ICONQUESTION) == IDYES){
		this->SendMessage(WM_CLOSE);
	}
}


void PageCluster::OnLbnDblclkListCluster()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nIndex;

	LPCSTR path;
	nIndex = lb_label.GetCurSel();
	if (nIndex >= 0) {
		CString cs;
		lb_label.GetText(nIndex, cs);
		// ���������
		ShellExecute(NULL, "open", (folder_write_path + "/").c_str() + cs, NULL, NULL, 1);

		//AfxMessageBox(cs);
	}
}

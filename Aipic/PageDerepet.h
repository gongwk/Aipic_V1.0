#pragma once
#include"ImageMatch.h"
#include"BasicFuncation.h"
// PageDerepet �Ի���

class PageDerepet : public CDialogEx
{
	DECLARE_DYNAMIC(PageDerepet)

public:
	PageDerepet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PageDerepet();

// �Ի�������
	enum { IDD = IDD_DIALOG_Derepet };

	//���� �ļ���·���� ������Ŀ¼�µ��ļ����ڵ�ͼƬ
	void SetFolderPath(string search_path);
private:
	//����·��
	string folderPath;
	//ʵ���� ͼ��ƥ����
	ImageMatch match;
	BasicFuncation base_funcation;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPagederepetStartderepet();
};

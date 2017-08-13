#pragma once
#include"ImageMatch.h"
#include"BasicFuncation.h"
// PageDerepet 对话框

class PageDerepet : public CDialogEx
{
	DECLARE_DYNAMIC(PageDerepet)

public:
	PageDerepet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PageDerepet();

// 对话框数据
	enum { IDD = IDD_DIALOG_Derepet };

	//设置 文件夹路径： 仅处理目录下的文件夹内的图片
	void SetFolderPath(string search_path);
private:
	//搜索路径
	string folderPath;
	//实例化 图像匹配类
	ImageMatch match;
	BasicFuncation base_funcation;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPagederepetStartderepet();
};

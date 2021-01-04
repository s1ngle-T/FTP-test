#pragma once

class CFClientDlg;
// AddNew 对话框

class AddNew : public CDialogEx
{
	DECLARE_DYNAMIC(AddNew)

public:
	AddNew(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AddNew();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void  AddNew::Adddoc(CFClientDlg* pPTFTrans);
	afx_msg void OnBnClickedOk();
};




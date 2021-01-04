#pragma once

class CFClientDlg;
// AddServer 对话框

class AddServer : public CDialogEx
{
	DECLARE_DYNAMIC(AddServer)

public:
	AddServer(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AddServer();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	void  Adddoc(CFClientDlg* pPTFTrans);
	afx_msg void OnBnClickedOk();
	
};

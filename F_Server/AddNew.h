﻿#pragma once

class CFServerDlg;
// AddNew 对话框

//extern int Flag;

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
	void AddNew::Add(CFServerDlg* pPTFTrans);
	afx_msg void OnBnClickedButton1();
};


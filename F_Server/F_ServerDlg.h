
// F_ServerDlg.h: 头文件
//

#pragma once
#include "Server.h"
//#include "Ser.h"
#include <vector>
#include<iostream>
using namespace std;
// CFServerDlg 对话框


//#define DEFAULT_HOME_DIR    "D:\\ftp\\" 

class FILE_FTP_INFO
{
public:
	char szFileName[256];
	DWORD    dwFileAttributes;
	char szFileDate[20];
	char szFileSize[20];
	DWORD    nFileSizeLow;
	BOOL nType;
};





class AddNew;


class CFServerDlg : public CDialogEx
{
// 构造
public:
	CFServerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_F_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	int dicon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl File_List;
	CListCtrl User_List;
	CImageList m_ImageList;
	CImageList    m_SmallIcon;
 // CString WorkPathlocal = WorkPath;
	char szDrives[MAX_PATH];

public:
	SOCKET SlistenSocket;
	SOCKET sev_file;
	HANDLE ListenThread;
	BOOL ShutDown;
    CString	localpath;
	AddNew* newwindow;
	int Flag;
public:
	void Init();
	afx_msg void OnBnClickedStartBtn();

	afx_msg void OnBnClickedAddBtn();
	
	afx_msg void OnCbnSelchangeCombo1();


	void ShowFileList(CString csDirPath);
	void Findfile(CString csDirPath/*, vector<CString>& vctPath*/);
	double GetDirSize(CString strFullPath);
	void GetDiskInfo();
	
	//void OnInsertFile(CListCtrl* pListCtrl, FILE_FTP_INFO* pIn);
	static void GetFileIcon(CString& fileName, int* iIcon, int* iIconSel);
	void OnInsertFile(CListCtrl* pListCtrl, FILE_FTP_INFO* pIn);
	void showdir();
	void CFServerDlg::DeleteDirectory(CString strPath);

	afx_msg void OnBnClickedFlashBtn();
	//afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	//CComboBox m_locallist;
	CComboBox m_localpath;
	afx_msg void OnBnClickedBackBtn();
	afx_msg void OnBnClickedViewBtn();
	afx_msg void OnCbnSelchangeSelectdir();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void Ondelete();
	afx_msg void OnAddNew();
};
DWORD WINAPI ListenThreadFunc(LPVOID pParam);
BOOL Socket_Select(SOCKET hSocket, int nTimeOut, BOOL bRead);

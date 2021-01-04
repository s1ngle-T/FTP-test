
// F_ClientDlg.h: 头文件
//

#pragma once

#define DEFAULT_HOME_DIR    "E:\\FTP\\"

//
//typedef struct
//{
//	char szFileName[256];
//	char szFileDate[20];
//	char szFileSize[20];
//	int nType;
//}FILE_FTP_INFO;

class AddNew;
class AddServer;
class FILE_FTP_INFO {
public:
	char szFileName[256];
	char szFileDate[20];
	char szFileSize[20];
	BOOL nType;
};





//typedef struct
//{
//	BOOL	m_bDirectory;
//	WORD	m_bSec;
//	CString length;
//	CString m_strSec;
//	CString m_strDate;
//	CString m_strName;
//	CString m_strLen;
//	int m_index;
//}FILE_FTP_INFO2;


// CFClientDlg 对话框
class CFClientDlg : public CDialogEx
{
// 构造
public:
	CFClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_F_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton4();


	void CFClientDlg::init();
	void CFClientDlg::GetDiskInfo();
	CListCtrl locallist;
	CListCtrl serverlist;
	CImageList m_ImageList;
	CComboBox m_localpath;
	BOOL Dirpath;
public:
	/*HICON m_hIcon;*/
	int dicon;
	CString	localpath;
	CString	Serverpath;
	CString	Currentpath;

	int Flag;
public:
	HANDLE ConnectThread;
	SOCKET ConnectSocket;
	HANDLE DownloadThread;
	HANDLE UpThread;
	HANDLE BackThread;
	HANDLE ChangeThread;
	AddNew* newwindow;
	AddServer* newdocwindow;
	////////////////////////////////////

public:
	void CFClientDlg::Findfile(CString csDirPath/*, vector<CString>& vctPath*/);
	void CFClientDlg::GetFileIcon(CString& fileName, int* iIcon, int* iIconSel);
	void CFClientDlg::OnInsertFile(CListCtrl* pListCtrl, FILE_FTP_INFO* pIn);
	void CFClientDlg::ShowFileList(CString csDirPath);
	double CFClientDlg::GetDirSize(CString strFullPath);
	void CFClientDlg::showdir();
	bool CFClientDlg::RecvAll(SOCKET& sock, char* buffer, int size);
	void CFClientDlg::show(CFClientDlg* pClient);

	void CFClientDlg::DownloadFile(CFClientDlg* pClient, BOOL ModleP);

	void CFClientDlg::UpFile(CFClientDlg* pClient, BOOL ModleP);
	UINT CFClientDlg::SendAll(SOCKET& sock, char* buffer, int size);
	//void CFClientDlg::Close(CFClientDlg* pClient);
	void  CFClientDlg::ChangeDir(CString ServerPath);
	void  CFClientDlg::Back(CString ServerPath);
	void  CFClientDlg::CurrentDir(CString ServerPath);
	/////////////////////////////////////////////////////////////////////
	void CFClientDlg::GetFileIcon2(CString& fileName, int* iIcon, int* iIconSel);

	bool CFClientDlg::InsertServerFile(FILE_FTP_INFO temp);

	void CFClientDlg::DeleteDirectory(CString strPath);

	BOOL  CFClientDlg::createDir(CString ServerPath);

	//BOOL CFClientDlg::GetFileItem(int& ndx);


public:
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedFlashLocalBtn();
	afx_msg void OnBnClickedLocalBackBtn();
	afx_msg void OnBnClickedLocalViewBtn();
	afx_msg void OnCbnSelchangeSelectdir();
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedLoginBtn();
	CComboBox serverpath;
	afx_msg void OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDownloadMenu();
	afx_msg void OnServerDelete();
	afx_msg void OnLocalUp();
	afx_msg void OnLocalDelete(CString strPath);
	afx_msg void OnBnClickedCloseBtn();
	afx_msg void OnBnClickedServerBackBtn();
	afx_msg void OnCbnSelchangeServerDir();
	afx_msg void OnNMDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedServerFlush();
	afx_msg void Onlocaldelete();
	/*afx_msg void OnAddNew();*/
	afx_msg void OnAddserverDoc();
	afx_msg void Onaddserverdoc();
};

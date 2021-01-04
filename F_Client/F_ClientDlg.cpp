
// F_ClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "F_Client.h"
#include "F_ClientDlg.h"
#include "afxdialogex.h"
#include"client.h"
#include "AddNew.h"
#include "AddServer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



char szDrives[MAX_PATH];
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFClientDlg 对话框



CFClientDlg::CFClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_F_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, locallist);
	DDX_Control(pDX, IDC_LIST2, serverlist);
	DDX_Control(pDX, IDC_COMBO1, m_localpath);
	DDX_Control(pDX, IDC_COMBO2, serverpath);
}

BEGIN_MESSAGE_MAP(CFClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFClientDlg::OnBnClickedFlashLocalBtn)
	ON_BN_CLICKED(IDC_BUTTON4, &CFClientDlg::OnBnClickedLocalBackBtn)
	ON_BN_CLICKED(IDC_BUTTON5, &CFClientDlg::OnBnClickedLocalViewBtn)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFClientDlg::OnCbnSelchangeSelectdir)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFClientDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON3, &CFClientDlg::OnBnClickedLoginBtn)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CFClientDlg::OnNMRClickList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CFClientDlg::OnNMRClickList1)
	ON_COMMAND(ID_Download, &CFClientDlg::OnDownloadMenu)
	ON_COMMAND(ID_Delete, &CFClientDlg::OnServerDelete)
	ON_COMMAND(ID_32775, &CFClientDlg::OnLocalUp)
	//ON_COMMAND(ID_32776, &CFClientDlg::OnLocalDelete)
	ON_BN_CLICKED(IDC_BUTTON2, &CFClientDlg::OnBnClickedCloseBtn)
	ON_BN_CLICKED(IDC_BUTTON6, &CFClientDlg::OnBnClickedServerBackBtn)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CFClientDlg::OnCbnSelchangeServerDir)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CFClientDlg::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_BUTTON7, &CFClientDlg::OnBnClickedServerFlush)
	ON_COMMAND(ID_32776, &CFClientDlg::Onlocaldelete)
	/*ON_COMMAND(ID_32778, &CFClientDlg::OnAddNew)*/
	ON_COMMAND(ID_32777, &CFClientDlg::OnAddserverDoc)
	ON_COMMAND(ID_32778, &CFClientDlg::Onaddserverdoc)
END_MESSAGE_MAP()


// CFClientDlg 消息处理程序

BOOL CFClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	init();




	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//函数
//初始化界面
void CFClientDlg::init()
{
	SetDlgItemText(IDC_EDIT2, _T("21"));
	SetDlgItemText(IDC_EDIT1, _T("s1ngle"));
	SetDlgItemText(IDC_EDIT3, _T("s1ngle"));
	//扩展风格，网格线
	DWORD dwstyle = locallist.GetExtendedStyle();
	DWORD dwstyle1 = serverlist.GetExtendedStyle();
	//dwstyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//设置列表样式风格，使子项也能插入图标
	dwstyle |= LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT;
	locallist.SetExtendedStyle(dwstyle);
	serverlist.SetExtendedStyle(dwstyle);

	/*UserList.*/

	locallist.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, 150);
	locallist.InsertColumn(1, _T("大小"), LVCFMT_CENTER, 100);
	locallist.InsertColumn(2, _T("时间"), LVCFMT_CENTER, 100);

	serverlist.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, 150);
	serverlist.InsertColumn(1, _T("大小"), LVCFMT_CENTER, 100);
	serverlist.InsertColumn(2, _T("时间"), LVCFMT_CENTER, 100);


	HIMAGELIST hImageList;
	//定义文件信息变量
	SHFILEINFO shFi;
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);
	hImageList = (HIMAGELIST)SHGetFileInfo(DEFAULT_HOME_DIR, 0, &shFi, sizeof(shFi),
		SHGFI_ICON | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	m_ImageList.m_hImageList = hImageList;
	locallist.SetImageList(&m_ImageList, LVSIL_SMALL);
	serverlist.SetImageList(&m_ImageList, LVSIL_SMALL);

	



	GetDiskInfo();
	m_localpath.AddString(DEFAULT_HOME_DIR);

	//int index = m_localpath.FindStringExact(0, _T("DEFAULT_HOME_DIR"));
	int nIndex = m_localpath.FindStringExact(0, DEFAULT_HOME_DIR);
	m_localpath.SetCurSel(nIndex);

	localpath = DEFAULT_HOME_DIR;
	ShowFileList(DEFAULT_HOME_DIR);
	//ShutDown = FALSE;


}

//获取磁盘信息
void CFClientDlg::GetDiskInfo()
{

	char* pDrive;
	if (!GetLogicalDriveStrings(sizeof(szDrives), szDrives))
	{
		AfxMessageBox(_T("获得驱动器失败"));
	}
	pDrive = szDrives;
	DWORD dwType;
	int index = 0;
	while (*pDrive)
	{
		dwType = ::GetDriveType(pDrive);
		CString str;
		str.Format(_T("%s"), pDrive);
		if (dwType != DRIVE_CDROM)
		{
			if (str != "E:\\")
			{
				m_localpath.AddString(str);
				index++;
			}

		}
		pDrive += strlen(pDrive) + 1;
	}
}

//获取文件图标
void CFClientDlg::GetFileIcon(CString& fileName, int* iIcon, int* iIconSel)
{

	CString str = fileName;
	if (str.Right(1) != "\\")
		str += "\\";
	SHFILEINFO shFi;
	if (!SHGetFileInfo(str, 0, &shFi, sizeof(shFi), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_DISPLAYNAME))
		//	if(!SHGetFileInfo("stggt.bmp",0,&shFi,sizeof(shFi),SHGFI_ICON|SHGFI_USEFILEATTRIBUTES))
	{
		AfxMessageBox("111");
		return;
	}
	*iIcon = shFi.iIcon;
	DestroyIcon(shFi.hIcon);
	if (iIconSel)
	{
		if (!SHGetFileInfo(str, 0, &shFi, sizeof(shFi), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OPENICON | SHGFI_DISPLAYNAME))
		{
			AfxMessageBox("222");
			return;
		}
		*iIconSel = shFi.iIcon;
		DestroyIcon(shFi.hIcon);
	}
	return;
}

//查找文件
void CFClientDlg::Findfile(CString csDirPath/*, vector<CString>& vctPath*/)
{
	/*FILE_FTP_INFO st[10];*/

	FILE_FTP_INFO* pInfo = new FILE_FTP_INFO;
	CFileFind fileFind;
	CFile f;
	int h = 0;
	int i = 0;
	CTime time;
	BOOL bContinue;
	::SetCurrentDirectory(csDirPath);

	bContinue = fileFind.FindFile("*.*");
	CString str;
	while (bContinue)
	{
		bContinue = fileFind.FindNextFile();
		csDirPath = fileFind.GetFilePath();

		if (fileFind.IsHidden())continue;
		str = fileFind.GetFileName();
		/*if (str == ".." || str == ".")
			continue;*/
		if (fileFind.IsDots())
			continue;

		strcpy(pInfo->szFileName, str);
		fileFind.GetLastWriteTime(time);

		str.Format("%d/%d/%d", time.GetYear(), time.GetMonth(), time.GetDay());
		strcpy(pInfo->szFileDate, str);

		if (fileFind.IsDirectory() && !fileFind.IsDots())
		{
			/*	h = GetDirSize(csDirPath);*/
			h = 0;
			pInfo->nType = 1;

		}
		else
		{
			h = fileFind.GetLength();
			pInfo->nType = 0;
		}
		if (h > (1024 * 1024 * 1024))
		{
			str.Format("%.2f", h * 1.0 / (1024 * 1024 * 1024));
			str += "G";
		}

		if (h > (1024 * 1024))
		{
			str.Format("%.2f", h * 1.0 / (1024 * 1024));
			str += "MB";
		}
		else if (h > 1024)
		{
			str.Format("%d", h / 1024);
			str += "KB";
		}
		else if (h == 0)
		{
			str.Format("%d", " ");
		}
		else {
			str.Format("%d", h);
			str += "B";
		}
		if (pInfo->nType == 1)
			strcpy(pInfo->szFileSize, "");
		else
			strcpy(pInfo->szFileSize, str);
		OnInsertFile(&locallist, pInfo);


	}
	delete pInfo;
	fileFind.Close();
}

//插入本地文件列表
void CFClientDlg::OnInsertFile(CListCtrl* pListCtrl, FILE_FTP_INFO* pIn)
{
	FILE_FTP_INFO* pInfo = (FILE_FTP_INFO*)pIn;
	LVITEM	lvIt;
	int iIcon, nItem;
	nItem = pListCtrl->GetItemCount();

	CString fileName = pInfo->szFileName;
	GetFileIcon(fileName, &iIcon, NULL);

	if (pInfo->nType == 1)dicon = iIcon;
	lvIt.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvIt.iImage = iIcon;
	lvIt.lParam = pInfo->nType;
	lvIt.pszText = pInfo->szFileName;
	lvIt.iSubItem = 0;
	lvIt.iItem = nItem;
	int iPos = pListCtrl->InsertItem(&lvIt);


	lvIt.mask = LVIF_TEXT;
	lvIt.iItem = iPos;
	lvIt.pszText = pInfo->szFileSize;
	lvIt.iSubItem = 1;
	pListCtrl->SetItem(&lvIt);


	lvIt.mask = LVIF_TEXT;
	lvIt.pszText = pInfo->szFileDate;
	lvIt.iSubItem = 2;
	pListCtrl->SetItem(&lvIt);

}

//显示本地数据列表
void CFClientDlg::ShowFileList(CString csDirPath)
{
	locallist.DeleteAllItems();
	Findfile(csDirPath);

}

//文件夹大小/不使用 层层递进太慢了
double CFClientDlg::GetDirSize(CString strFullPath)
{
	double dwDirSize, dwSubDirSize;
	CFileFind finder;

	dwDirSize = dwSubDirSize = 0;
	strFullPath += _T("\\*.*");
	if (finder.FindFile(strFullPath))
	{
		while (1)
		{
			BOOL   bFound;
			bFound = finder.FindNextFile();
			if (finder.IsDirectory())
			{
				if (!finder.IsDots())
				{
					dwSubDirSize = GetDirSize(finder.GetFilePath());
					dwDirSize += dwSubDirSize;
				}
			}
			else
			{
				CFile file;
				if (file.Open(finder.GetFilePath(), CFile::modeRead))
				{
					dwDirSize += file.GetLength();
					file.Close();
				}
			}
			if (bFound == FALSE)
				break;
		}
	}
	finder.Close();
	return dwDirSize;
}

//路径combox
void CFClientDlg::showdir()
{

	int item = m_localpath.GetCurSel();
	CString str1;
	TCHAR szText[100] = { 0 };
	m_localpath.GetLBText(item, str1);

	/*AfxMessageBox(szText);*/
	m_localpath.DeleteString(item);
	m_localpath.InsertString(item, str1.Left(3));

	/*TCHAR szText[100] = { 0 };
	m_localpath.GetLBText(item, szText);*/
	item = m_localpath.FindString(0, localpath.Left(3));
	m_localpath.SetCurSel(item);
	m_localpath.DeleteString(item);
	m_localpath.InsertString(item, localpath);
	m_localpath.SetCurSel(item);


}

//获取服务端图标
void CFClientDlg::GetFileIcon2(CString& fileName, int* iIcon, int* iIconSel)
{
	CString str = fileName;
	if (str.Right(1) != "\\")
		str += "\\";
	SHFILEINFO shFi;
	if (!SHGetFileInfo(fileName, 0, &shFi, sizeof(shFi), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
	{
		AfxMessageBox("111");
		return;
	}
	*iIcon = shFi.iIcon;
	DestroyIcon(shFi.hIcon);
	if (iIconSel)
	{
		if (!SHGetFileInfo(str, 0, &shFi, sizeof(shFi), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_OPENICON | SHGFI_DISPLAYNAME))
		{
			AfxMessageBox("222");
			return;
		}
		*iIconSel = shFi.iIcon;
		DestroyIcon(shFi.hIcon);
	}
	return;
}

//插入服务端数据
bool CFClientDlg::InsertServerFile(FILE_FTP_INFO temp)
{

	LV_ITEM lvi;
	int ndx = serverlist.GetItemCount();
	int iIcon;
	CString str;
	if (temp.nType)
		iIcon = dicon;
	else
	{
		str.Format(_T("%s"), temp.szFileName);
		GetFileIcon2(str, &iIcon, NULL);
	}





	CString s;
	s.Format("%d", iIcon);
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iImage = iIcon;
	lvi.lParam = 0;
	lvi.pszText = temp.szFileName/*.GetBuffer(0)*/;
	//temp.szFileName.ReleaseBuffer();
	lvi.iSubItem = 0;
	lvi.iItem = ndx;
	ndx = serverlist.InsertItem(&lvi);

	lvi.mask = LVIF_TEXT;
	if (temp.nType)
		lvi.pszText = " ";
	else
		lvi.pszText = temp.szFileSize/*.GetBuffer(0)*/;
	//temp.m_strLen.ReleaseBuffer();
	lvi.iItem = ndx;
	lvi.iSubItem = 1;
	serverlist.SetItem(&lvi);

	lvi.pszText = temp.szFileDate/*.GetBuffer(0)*/;
	//temp.m_strDate.ReleaseBuffer();
	lvi.iItem = ndx;
	lvi.iSubItem = 2;
	serverlist.SetItem(&lvi);

	//lvi.pszText = temp.szFileDate/*.GetBuffer(0)*/;
	////temp.m_strSec.ReleaseBuffer();
	//lvi.iItem = ndx;
	//lvi.iSubItem = 3;
	//serverlist.SetItem(&lvi);

	/*serverlist.SetItemText(1, 0, " ");*/

	return true;
}

//接收所有
bool CFClientDlg::RecvAll(SOCKET& sock, char* buffer, int size)
{

	while (size > 0)//剩余部分大于0
	{
		CString strMsg;
		int RecvSize = recv(sock, buffer, 1024, 0);
		strMsg.Format(_T("%s"), buffer);
		if (SOCKET_ERROR == RecvSize)
			return false;
		size = size - RecvSize;
		buffer += RecvSize;

	}


	return true;
}

//发送所有数据
UINT CFClientDlg::SendAll(SOCKET& sock, char* buffer, int size)
{
	/*for (int index = 0; index < ClientArry.GetCount(); index++)
	{*/
	int SendSize;
	while (size > 0)
	{
		if (size > 1024)
		{
			SendSize = send(sock, buffer, 1024, 0);
			if (SOCKET_ERROR == SendSize)
				return false;
			size = size - SendSize;//用于循环发送且退出功能
			buffer += SendSize;//用于计算已发buffer的偏移量
		}
		else
		{
			SendSize = send(sock, buffer, size, 0);
			if (SOCKET_ERROR == SendSize)
				return false;
			size = size - SendSize;//用于循环发送且退出功能
			buffer += SendSize;//用于计算已发buffer的偏移量
		}





	}
	//}
	return SendSize;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//本地
//刷新本地
void CFClientDlg::OnBnClickedFlashLocalBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csDirPath = DEFAULT_HOME_DIR;
	ShowFileList(localpath);
	
}

//上一级
void CFClientDlg::OnBnClickedLocalBackBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (localpath.GetLength() < 4)return;
	int j;
	for (j = localpath.GetLength() - 2; j >= 0; j--)
		if (localpath.GetAt(j) == '\\')break;
	localpath = localpath.Left(j);
	localpath += "\\";
	::SetCurrentDirectory(localpath);
	locallist.DeleteAllItems();
	Findfile(localpath);
	UpdateData(FALSE);
	showdir();
}

//浏览
void CFClientDlg::OnBnClickedLocalViewBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	CString str;
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = name;
	bi.lpszTitle = "Select folder";
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl == NULL)
		return;
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	localpath = str;//为对话框中与一编辑框对应的CString型变量，保存并显示选中的路径。
	if (str.GetAt(str.GetLength() - 1) != '\\')
		localpath += "\\";
	/*::SetCurrentDirectory("D:\\");*/
	locallist.DeleteAllItems();
	Findfile(localpath);
	UpdateData(FALSE);
	showdir();
}

//路径改变
void CFClientDlg::OnCbnSelchangeSelectdir()
{
	// TODO: 在此添加控件通知处理程序代码
	int item = m_localpath.GetCurSel();
	TCHAR szText[100] = { 0 };
	m_localpath.GetLBText(item, szText);
	localpath = szText;
	ShowFileList(szText);
}

//双击本地
void CFClientDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (locallist.GetSelectionMark() == -1)return;
	POSITION pos = locallist.GetFirstSelectedItemPosition();
	int row = (int)locallist.GetNextSelectedItem(pos);
	CString str;
	str = locallist.GetItemText(row, 0);
	if (GetFileAttributes(str) == FILE_ATTRIBUTE_DIRECTORY)
	{

		localpath=localpath+ str + "\\";
		locallist.DeleteAllItems();
		Findfile(localpath);
		UpdateData(FALSE);
		showdir();
	}


	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//服务端
//双击服务端
void CFClientDlg::OnNMDblclkList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (serverlist.GetSelectionMark() == -1)return;
	POSITION pos = serverlist.GetFirstSelectedItemPosition();
	int row = (int)serverlist.GetNextSelectedItem(pos);
	CString str;
	str = serverlist.GetItemText(row, 0);
	
	if (str.GetLength() < 4)
	{
		Serverpath = Serverpath.TrimRight("\\");
		Serverpath = Serverpath + "\\" + str + "\\";
		if (GetFileAttributes(Serverpath) == FILE_ATTRIBUTE_DIRECTORY)
		{
			
			serverlist.DeleteAllItems();

			ChangeDir(Serverpath);
			////////////////////////////////////////////////////////////////////////


		}
	}


	else if(GetFileAttributes(str) == FILE_ATTRIBUTE_DIRECTORY)
	{
	/*	AfxMessageBox("11111");*/
		Serverpath = Serverpath.TrimRight("\\");
		Serverpath = Serverpath +"\\"+ str+"\\" ;
		//AfxMessageBox(Serverpath);
		serverlist.DeleteAllItems();
	
		ChangeDir(Serverpath);
////////////////////////////////////////////////////////////////////////

		
	}
}


//服务端上一级
void CFClientDlg::OnBnClickedServerBackBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//BackThread = CreateThread(NULL, 0, BackThreadFunc, this, 0, NULL);
	int item = serverpath.GetCurSel();
	TCHAR szText[100] = { 0 };
	serverpath.GetLBText(item, szText);
	Currentpath = szText;
	
	if (Currentpath.GetLength() < 4)
	{
		//AfxMessageBox("11111");
		return;
	}
	
	serverlist.DeleteAllItems();
	Back(Currentpath);

}

//combox选择路径
void CFClientDlg::OnCbnSelchangeServerDir()
{
	// TODO: 在此添加控件通知处理程序代码

	int item = serverpath.GetCurSel();
	TCHAR szText[100] = { 0 };
	serverpath.GetLBText(item, szText);
	Serverpath = szText;
	serverlist.DeleteAllItems();
	//Serverpath = Serverpath + "\\";

	//AfxMessageBox(Serverpath);
	ChangeDir(Serverpath);



}

//服务端刷新
void CFClientDlg::OnBnClickedServerFlush()
{
	// TODO: 在此添加控件通知处理程序代码
	int item = serverpath.GetCurSel();
	TCHAR szText[100] = { 0 };
	serverpath.GetLBText(item, szText);
	Currentpath = szText;
	serverlist.DeleteAllItems();
	//Serverpath = Serverpath + "\\";

	//AfxMessageBox(Serverpath);
	CurrentDir(Currentpath);
}


//登陆
void CFClientDlg::OnBnClickedLoginBtn()
{
	// TODO: 在此添加控件通知处理程序代码

	ConnectThread = CreateThread(NULL, 0, ConnectThreadFunc, this, 0, NULL);
	
	
}


//关闭
void CFClientDlg::OnBnClickedCloseBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str = "QUIT \r\n";
	int i = send(ConnectSocket, str, str.GetLength(), 0);
	closesocket(ConnectSocket);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//其他
//右击菜单
void CFClientDlg::OnNMRClickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	

	DWORD dwPos = GetMessagePos();
	CPoint Point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU1); //要弹出的菜单ID
	CMenu* Popup = Menu.GetSubMenu(0); 
	Popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);




	
}

//右击本地菜单
void CFClientDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	DWORD dwPos = GetMessagePos();
	CPoint Point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU2); //要弹出的菜单ID
	CMenu* Popup = Menu.GetSubMenu(0);
	Popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);
}

//服务端下载
void CFClientDlg::OnDownloadMenu()
{
	// TODO: 在此添加命令处理程序代码
	DownloadThread = CreateThread(NULL, 0, DownloadThreadFunc, this, 0, NULL);
	
}

//服务端删除
void CFClientDlg::OnServerDelete()
{
	// TODO: 在此添加命令处理程序代码
	
}

//本地上传
void CFClientDlg::OnLocalUp()
{
	// TODO: 在此添加命令处理程序代码
	UpThread = CreateThread(NULL, 0, UpThreadFunc, this, 0, NULL);
}

//删除文件夹/文件
void CFClientDlg::DeleteDirectory(CString strPath)
{

	if (!strPath.IsEmpty())
	{

		CFileFind fileFinder;
		BOOL bFind = fileFinder.FindFile(strPath + _T("\\*.*"));
		while (bFind)
		{
			bFind = fileFinder.FindNextFile();
			if (fileFinder.IsDirectory()) //这里可以使用递归进行子文件夹的删除
				continue;

			CString strFilePath = fileFinder.GetFilePath();
			DeleteFile(strFilePath);
		}
		RemoveDirectory(strPath);
	}
	else
		RemoveDirectory(strPath);
}

//删除本地文件/文件夹
void CFClientDlg::Onlocaldelete()
{
	// TODO: 在此添加命令处理程序代码
	if (locallist.GetSelectionMark() == -1)return;
	POSITION pos = locallist.GetFirstSelectedItemPosition();
	int row = (int)locallist.GetNextSelectedItem(pos);
	CString str;
	str = locallist.GetItemText(row, 0);

	if (GetFileAttributes(str) == FILE_ATTRIBUTE_DIRECTORY)
	{
		CString path = localpath + str + "\\";
		DeleteDirectory(path);
	}

	else
	{
		DeleteFile(str);
	}

	ShowFileList(localpath);


}

//本地添加文件夹
void CFClientDlg::OnAddserverDoc()
{
	// TODO: 在此添加命令处理程序代码
	newwindow = new AddNew();
	newwindow->Create(IDD_DIALOG1);
	newwindow->ShowWindow(true);
	newwindow->Adddoc(this);
}


//服务端添加
void CFClientDlg::Onaddserverdoc()
{
	// TODO: 在此添加命令处理程序代码
	newdocwindow = new AddServer();
	newdocwindow->Create(IDD_DIALOG1);
	newdocwindow->ShowWindow(true);
	newdocwindow->Adddoc(this);
}

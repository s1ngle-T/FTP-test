
// F_ServerDlg.cpp: 实现文件
//


#include "pch.h"
#include "framework.h"
#include"F_Server.h"
#include "F_ServerDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "Ser.h"
#include "AddNew.h"
using namespace std;
#include<vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


////////////////////////////////////////////////////////
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


//
//char szDrives[MAX_PATH];

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


// CFServerDlg 对话框



CFServerDlg::CFServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_F_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, File_List);
	DDX_Control(pDX, IDC_LIST2, User_List);

	DDX_Control(pDX, IDC_COMBO1, m_localpath);
}

BEGIN_MESSAGE_MAP(CFServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CFServerDlg::OnBnClickedStartBtn)
	ON_BN_CLICKED(IDC_BUTTON4, &CFServerDlg::OnBnClickedAddBtn)
	ON_BN_CLICKED(IDC_BUTTON3, &CFServerDlg::OnBnClickedFlashBtn)
	ON_BN_CLICKED(IDC_BUTTON5, &CFServerDlg::OnBnClickedBackBtn)
	ON_BN_CLICKED(IDC_BUTTON6, &CFServerDlg::OnBnClickedViewBtn)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CFServerDlg::OnCbnSelchangeSelectdir)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CFServerDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CFServerDlg::OnNMRClickList1)
	ON_COMMAND(ID_32771, &CFServerDlg::Ondelete)
	ON_COMMAND(ID_32772, &CFServerDlg::OnAddNew)
END_MESSAGE_MAP()


// CFServerDlg 消息处理程序

BOOL CFServerDlg::OnInitDialog()
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
	Init();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFServerDlg::OnPaint()
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
HCURSOR CFServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//初始化
void CFServerDlg::Init()
{
	SetDlgItemText(IDC_EDIT1, _T("21"));
	//扩展风格，网格线
	DWORD dwstyle = User_List.GetExtendedStyle();
	DWORD dwstyle1 = File_List.GetExtendedStyle();
	//dwstyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	//设置列表样式风格，使子项也能插入图标
	dwstyle |= LVS_EX_GRIDLINES | LVS_EDITLABELS | LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT;
	User_List.SetExtendedStyle(dwstyle);
	File_List.SetExtendedStyle(dwstyle);

	/*UserList.*/

	User_List.InsertColumn(0, _T("用户名"), LVCFMT_CENTER,100);
	User_List.InsertColumn(1, _T("用户地址"), LVCFMT_CENTER, 200);
	

	File_List.InsertColumn(0, _T("文件名"), LVCFMT_CENTER, 220);
	File_List.InsertColumn(1, _T("大小"), LVCFMT_CENTER, 70);
	File_List.InsertColumn(2, _T("时间"), LVCFMT_CENTER, 100);

	HIMAGELIST hImageList;
	//定义文件信息变量
	SHFILEINFO shFi;
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);
	hImageList = (HIMAGELIST)SHGetFileInfo(DEFAULT_HOME_DIR, 0, &shFi, sizeof(shFi),
		SHGFI_ICON | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	m_ImageList.m_hImageList = hImageList;
	File_List.SetImageList(&m_ImageList, LVSIL_SMALL);

	GetDiskInfo();
	m_localpath.AddString(DEFAULT_HOME_DIR);

	//int index = m_localpath.FindStringExact(0, _T("DEFAULT_HOME_DIR"));
	int nIndex = m_localpath.FindStringExact(0, DEFAULT_HOME_DIR);
	m_localpath.SetCurSel(nIndex);

	localpath = DEFAULT_HOME_DIR;
	ShowFileList(DEFAULT_HOME_DIR);
	ShutDown = FALSE;
	
}

//获取文件图标
void CFServerDlg::GetFileIcon(CString& fileName, int* iIcon, int* iIconSel)
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
void CFServerDlg::Findfile(CString csDirPath/*, vector<CString>& vctPath*/)
{
	/*FILE_FTP_INFO st[10];*/

	FILE_FTP_INFO* pInfo = new FILE_FTP_INFO;
	CFileFind fileFind;
	CFile f;
	int h=0;
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
		if (h > (1024 * 1024*1024))
		{
			str.Format("%.2f", h * 1.0 / (1024 * 1024*1024));
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
		else if (h==0)
		{
			str.Format("%d", " ");
		}
		else {
			str.Format("%d", h);
			str += "B";
		}
		if(pInfo->nType == 1)
		strcpy(pInfo->szFileSize, "");
		else
		strcpy(pInfo->szFileSize, str);
		OnInsertFile(&File_List, pInfo);

		
		
	}

	delete pInfo;
	fileFind.Close();
}

//文件列表显示
void CFServerDlg::ShowFileList(CString csDirPath)
{
	File_List.DeleteAllItems();
	Findfile(csDirPath);
	
}

//文件夹大小
double CFServerDlg::GetDirSize(CString strFullPath)
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

//插入文件内容
void CFServerDlg::OnInsertFile(CListCtrl* pListCtrl, FILE_FTP_INFO* pIn)
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

//获取磁盘信息
void CFServerDlg::GetDiskInfo()
{
	
	char* pDrive;
	if (!GetLogicalDriveStrings(sizeof(szDrives), szDrives)) 
	{
		AfxMessageBox("获得驱动器失败");
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
			if (str != "D:\\")
			{
				m_localpath.AddString(str);
				index++;
			}
			
		}
		pDrive += strlen(pDrive) + 1;
	}
}

//显示路径
void CFServerDlg::showdir()
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


///////////////////////////////////////////////////////////////////////////////////////////////////
//事件
//停止

//添加
void CFServerDlg::OnBnClickedAddBtn()
{
	// TODO: 在此添加控件通知处理程序代码
}

//开始
void CFServerDlg::OnBnClickedStartBtn()
{
	ListenThread = CreateThread(NULL, 0, ListenThreadFunc, this, 0, NULL);

}

//刷新
void CFServerDlg::OnBnClickedFlashBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csDirPath = DEFAULT_HOME_DIR;
	ShowFileList(localpath);
	::SetCurrentDirectory(WorkPath);
}


//上一级
void CFServerDlg::OnBnClickedBackBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//localpath = DEFAULT_HOME_DIR;
	if (localpath.GetLength() < 4)return;
	int j;
	for (j = localpath.GetLength() - 2; j >= 0; j--)
		if (localpath.GetAt(j) == '\\')break;
	localpath = localpath.Left(j);
	localpath += "\\";
	::SetCurrentDirectory(localpath);
	File_List.DeleteAllItems();
	Findfile(localpath);
	UpdateData(FALSE);
	showdir();
	::SetCurrentDirectory(WorkPath);
	//AfxMessageBox(WorkPath);
}

//浏览
void CFServerDlg::OnBnClickedViewBtn()
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
	File_List.DeleteAllItems();
	Findfile(localpath);
	UpdateData(FALSE);
	showdir();
	::SetCurrentDirectory(WorkPath);
}

//////////////////////////////////////////////////////////////////////////////////
//更改事件
//路径选择改变


void CFServerDlg::OnCbnSelchangeSelectdir()
{
	// TODO: 在此添加控件通知处理程序代码
	int item = m_localpath.GetCurSel();
	TCHAR szText[100] = { 0 };
	m_localpath.GetLBText(item, szText);
	localpath = szText;
	ShowFileList(szText);
	
	::SetCurrentDirectory(WorkPath);
}

//双击下一级改变
void CFServerDlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	::SetCurrentDirectory(localpath);
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (File_List.GetSelectionMark() == -1)return;
	POSITION pos = File_List.GetFirstSelectedItemPosition();
	int row = (int)File_List.GetNextSelectedItem(pos);
	CString str;
	str = File_List.GetItemText(row, 0);
	if (GetFileAttributes(str) == FILE_ATTRIBUTE_DIRECTORY)
	{
	
		localpath += str + "\\";
		File_List.DeleteAllItems();
		::SetCurrentDirectory(localpath);
		Findfile(localpath);
		UpdateData(FALSE);
		showdir();
		::SetCurrentDirectory(WorkPath);
		//AfxMessageBox(WorkPath);
	}

	
	*pResult = 0;
}

//右击处理事件
void CFServerDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
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

//右击删除
void CFServerDlg::Ondelete()
{
	// TODO: 在此添加命令处理程序代码
	if (File_List.GetSelectionMark() == -1)return;
	POSITION pos = File_List.GetFirstSelectedItemPosition();
	int row = (int)File_List.GetNextSelectedItem(pos);
	CString str;
	str = File_List.GetItemText(row, 0);

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

//删除文件夹
void CFServerDlg::DeleteDirectory(CString strPath)
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

CString name;

//新建文件夹
void CFServerDlg::OnAddNew()
{
	// TODO: 在此添加命令处理程序代码
	
	 newwindow = new AddNew();
	newwindow->Create(IDD_DIALOG1);
	newwindow->ShowWindow(true);
	newwindow->Add(this);
	
}




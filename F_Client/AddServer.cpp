// AddServer.cpp: 实现文件
//

#include "pch.h"
#include "F_Client.h"
#include "AddServer.h"
#include "afxdialogex.h"
#include "F_ClientDlg.h"

#include "client.h"
// AddServer 对话框

IMPLEMENT_DYNAMIC(AddServer, CDialogEx)

AddServer::AddServer(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

AddServer::~AddServer()
{
}

void AddServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddServer, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddServer::OnBnClickedOk)
END_MESSAGE_MAP()


// AddServer 消息处理程序



int Flag4 = 0;

CFClientDlg* Ptr1;

void  AddServer::Adddoc(CFClientDlg* pPTFTrans)
{

	Ptr1 = pPTFTrans;
}



void AddServer::OnBnClickedOk()
{
	int Flag3 = 1;
//	CDialogEx::OnOK();
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	GetDlgItemText(IDC_EDIT1, strName);

	if (strName.GetLength() == 0)
	{
		MessageBox("名字不能为空");
	}

	int pos = 0;
	int count = Ptr1->serverlist.GetItemCount();
	CString name;


	while (pos < count)
	{
		name = Ptr1->serverlist.GetItemText(pos, 0);
		if (name == strName)
		{
			MessageBox("名字不能重复");
			Flag3 = 0;
			break;
		}
		pos++;

	}

	char buf[256];
	CString path;
	if (Flag3 == 1)
	{

		//::GetCurrentDirectory(256, buf);

		int item = Ptr1->serverpath.GetCurSel();
		TCHAR szText[100] = { 0 };
		Ptr1->serverpath.GetLBText(item, szText);
		//Ptr1->Currentpath = szText;

		//strcat(szText, "\\");
		path = szText;
		strcat(szText, strName);

		int i=Ptr1->createDir(szText);

		/*char buffctrl[30];
		int i = recv(Ptr1->ConnectSocket, buffctrl, sizeof(buffctrl), 0);*/
	/*	AfxMessageBox(buffctrl);*/
		if (i > 0)
		{
			MessageBox("文件夹创建成功");
			Ptr1->Flag = 1;
			Ptr1->newdocwindow->DestroyWindow();
			delete Ptr1->newdocwindow;
			Ptr1->newdocwindow = NULL;
			Ptr1->CurrentDir(path);
			ShowList(Ptr1);
		}
			
		else
			return;

	}

	//Ptr1->ShowFileList(path);


}


//void AddNew::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString strName;
//	GetDlgItemText(IDC_EDIT1, strName);
//
//	if (strName.GetLength() == 0)
//	{
//		MessageBox("名字不能为空");
//	}
//
//	int pos = 0;
//	int count = Ptr->locallist.GetItemCount();
//	CString name;
//
//
//	while (pos < count)
//	{
//		name = Ptr->locallist.GetItemText(pos, 0);
//		if (name == strName)
//		{
//			MessageBox("名字不能重复");
//			break;
//		}
//		pos++;
//
//	}
//
//	char buf[256];
//	CString path;
//	if (Flag == 1)
//	{
//
//		::GetCurrentDirectory(256, buf);
//		strcat(buf, "\\");
//		path = buf;
//		strcat(buf, strName);
//		if (CreateDirectory(buf, NULL))   //创建目录
//		{
//			MessageBox("文件夹创建成功");
//			Ptr->Flag = 1;
//			Ptr->newwindow->DestroyWindow();
//			delete Ptr->newwindow;
//			Ptr->newwindow = NULL;
//		}
//	}
//
//	Ptr->ShowFileList(path);
//
//
//}



// AddNew.cpp: 实现文件
//

#include "pch.h"
#include "F_Server.h"
#include "AddNew.h"
#include "afxdialogex.h"
#include "F_ServerDlg.h"

// AddNew 对话框

IMPLEMENT_DYNAMIC(AddNew, CDialogEx)

AddNew::AddNew(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

AddNew::~AddNew()
{
}

void AddNew::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(AddNew, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON1, &AddNew::OnBnClickedButton1)
END_MESSAGE_MAP()


// AddNew 消息处理程序

int Flag = 1;
int Flag1 = 0;
//LPVOID pParam;
//
//CFServerDlg* pPTFTrans = (CFServerDlg*)pParam;



CFServerDlg* Ptr;
void  AddNew::Add(CFServerDlg* pPTFTrans)
{

	Ptr = pPTFTrans;
}





void AddNew::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strName;
	GetDlgItemText(IDC_EDIT1, strName);

	if (strName.GetLength() == 0)
	{
		MessageBox("名字不能为空");
	}
	
	int pos = 0;
	int count = Ptr->File_List.GetItemCount();
	CString name;


		while (pos < count)
		{
			name = Ptr->File_List.GetItemText(pos, 0);
			if (name == strName)
			{
				MessageBox("名字不能重复");
				break;
			}
			pos++;

		}
	
		char buf[256];
		CString path;
	if (Flag==1)
	{
		
		::GetCurrentDirectory(256, buf);
		strcat(buf, "\\");
		path = buf;
		strcat(buf, strName);
		if (CreateDirectory(buf, NULL))   //创建目录
		{
			MessageBox("文件夹创建成功");
			Ptr->Flag = 1;
			Ptr->newwindow->DestroyWindow();
			delete Ptr->newwindow;
			Ptr->newwindow = NULL;
		}
	}
		
	Ptr->ShowFileList(path);
	
	
}




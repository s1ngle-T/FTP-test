#include "pch.h"
#include "framework.h"
#include "F_Client.h"
#include "F_ClientDlg.h"
#include "afxdialogex.h"
#include "client.h"
#include <stdio.h>
#include <string>
#include <vector>
#include<iostream>
using namespace std;
#include<stdio.h>
#define MAX_REQ_LEN     256
#define DATA_BUFSIZE    1024
char buffctrl[30];
char DiskBuff[20];
char buff1[10240];
char buffdata[BUFFMax];
FILE_FTP_INFO pFI[MAX_FILE_NUM];
UINT localsock;
SOCKET ListenSrvr;			//监听ftpsocket，等待ftp的连接
SOCKET DataSrvr;			//数据传输socket
CHAR   buffRecv[DATA_BUFSIZE];
struct  sockaddr_in saSrc;
struct  sockaddr_in saDest;
int addr_size = sizeof(sockaddr_in);

BOOL TypeA = true;
BOOL ModleP = true;


vector<char*>v;
CString lhost;
UINT localsock1;
//long LocalFileLen = 0;
//BOOL LocalFileExist = 0;
sockaddr_in server;
FILE_FTP_INFO FileArry[MAX_FILE_NUM];


char  szCmd[MAX_REQ_LEN];
char  buffrecv[MAX_PATH];

DWORD WINAPI ConnectThreadFunc(LPVOID pParam)
{
	/*CClientItem PClientItem = *(CClientItem*)pParam;
	PClientItem.CName = "zzzz";*/
	CFClientDlg* pClient = (CFClientDlg*)pParam;
	ASSERT(pClient != NULL);
	
	
	//////连接控制通道
	pClient->ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (pClient->ConnectSocket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("新建socket失败"));
		return FALSE;
	}


	int uPort = pClient->GetDlgItemInt(IDC_EDIT2);
	if (uPort !=21)
	{
		AfxMessageBox(_T("请输入正确的端口号：21"));
		closesocket(pClient->ConnectSocket);
		return FALSE;
	}




	//sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(21);
	inet_pton(AF_INET, "172.20.10.5", (void*)&server.sin_addr.S_un.S_addr);
	if (connect(pClient->ConnectSocket, (struct sockaddr*) & server, sizeof(struct sockaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("连接失败，请重试"));
		closesocket(pClient->ConnectSocket);
		return false;
	}
	
	if (!Login(pClient))
	{
		closesocket(pClient->ConnectSocket);
		return false;
	}

	ShowList(pClient);
	//StarctData(pClient);
	
	return true;

}

//设置文件模式
void SetType(CFClientDlg* pClient, BOOL TypeA)
{

	char* p;
	////只做ASCII式
	/*Sleep(100);*/
	CString str;
	if (TypeA)
		//pClient->GetDlgItemText(IDC_EDIT3, str);
		str = "TYPE A\r\n";
	else
		str = "TYPE binary\r\n";
	int i = send(pClient->ConnectSocket, str, str.GetLength(), 0);
	if (i > 0)
	{
		int i = recv(pClient->ConnectSocket, buffRecv, sizeof(buffRecv), 0);
		//AfxMessageBox(buffRecv);
		if (strtok(buffRecv, " \r\n") != NULL)
		{
			//AfxMessageBox(buffRecv);
			if (!strstr(buffRecv, "200"))
			{
				closesocket(pClient->ConnectSocket);
				return;
			}
		}
	}

}

//设置传输模式
void SetModele(CFClientDlg* pClient, BOOL ModleP)
{
	CString Port;
	if (ModleP)
		Port = "PORT " + lhost + "\r\n";
	else
		Port = "PASV " + lhost + "\r\n";
	int i = send(pClient->ConnectSocket, Port, Port.GetLength(), 0);
	if (i > 0)
	{
		int i = recv(pClient->ConnectSocket, buffRecv, sizeof(buffRecv), 0);
		//AfxMessageBox(buffRecv);
		if (strtok(buffRecv, " \r\n") != NULL)
		{
			//AfxMessageBox(buffRecv);
			if (!strstr(buffRecv, "200"))
			{
				closesocket(pClient->ConnectSocket);
				return;
			}
		}
	}
}

//登陆
bool Login(CFClientDlg* pClient)
{
	//pClient->RecvAll(pClient->ConnectSocket, buffctrl, sizeof(buffctrl));
	int i = recv(pClient->ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	//AfxMessageBox(buffRecv);

	if (strtok(buffRecv, " \r\n") != NULL)
	{
		if (strstr(buffRecv, "220"))
		{
			CString str;
			pClient->GetDlgItemText(IDC_EDIT1, str);
			str = "USER " + str + "\r\n";
			send(pClient->ConnectSocket, str.GetBuffer(str.GetLength()),
				str.GetLength(), 0);

		}

	}

	/*pClient->RecvAll(pClient->ConnectSocket, buffctrl, sizeof(buffctrl));*/
	recv(pClient->ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	//AfxMessageBox(buffRecv);
	/*AfxMessageBox(buff);*/

	if (strtok(buffRecv, " \r\n") != NULL)
	{
		if (strstr(buffRecv, "331"))
		{
			CString str;
			pClient->GetDlgItemText(IDC_EDIT3, str);
			str = "PASS " + str + "\r\n";
			send(pClient->ConnectSocket, str.GetBuffer(str.GetLength()),
				str.GetLength(), 0);
		}
		else
		{
			AfxMessageBox("输入错误.....");
			closesocket(pClient->ConnectSocket);
			return false;
		}

		recv(pClient->ConnectSocket, buffRecv, sizeof(buffRecv), 0);
		//AfxMessageBox(buffRecv);
		if (strtok(buffRecv, " \r\n") != NULL)
		{
			/*AfxMessageBox(buffRecv);*/
			if (!strstr(buffRecv, "230"))
			{
				closesocket(pClient->ConnectSocket);
				return false;
			}
			/*else
			{
				AfxMessageBox("11111");
				closesocket(pClient->ConnectSocket);
				return false;
			}*/
		}
		//AfxMessageBox(buffctrl);
	}
	//memset(buffRecv, 0, sizeof(buffRecv));
	return true;
}
//地址转换
char* ConvertCommaAddress(char* szAddress, WORD wPort)
{
	char szPort[10];
	sprintf(szPort, "%d,%d", wPort / 256, wPort % 256);
	char szIpAddr[20];
	sprintf(szIpAddr, "%s,", szAddress);
	int idx = 0;
	while (szIpAddr[idx])
	{
		if (szIpAddr[idx] == '.')
			szIpAddr[idx] = ',';
		idx++;
	}
	sprintf(szAddress, "%s%s", szIpAddr, szPort);
	return szAddress;
}

//数据连接
int	DataConn(/*DWORD dwIp, WORD wPort, int nMode,*/CFClientDlg* pClient,BOOL Type,BOOL ModleP)
{
	if (ModleP)
	{
		CString  temp, strOut, rhost;
		UINT localsock = 0, i;
		//CFile datafile;
		//SOCKET ListenSrvr;			//监听ftpsocket，等待ftp的连接
		//SOCKET DataSrvr;			//数据传输socket
		int num = 0, sum, nRet;
		const int BUFSIZE = 4096;
		DWORD lpArgument = 0;

		
		SetType(pClient, Type);
		//AfxMessageBox("1111");
		//struct  sockaddr_in saSrc;
		//struct  sockaddr_in saDest;
		//int addr_size;
		//addr_size = sizeof(sockaddr_in);

		////////////////////////////////////////////////////////////////////////////////
		//创建监听socket
		ListenSrvr = socket(AF_INET, SOCK_STREAM, 0);
		if (ListenSrvr == INVALID_SOCKET)
		{
			return FALSE;
		}


		//绑定socket
		memset(&saSrc, 0, sizeof(sockaddr_in));
		saSrc.sin_family = AF_INET;
		saSrc.sin_port = htons(0);
		nRet = bind(ListenSrvr, (LPSOCKADDR)&saSrc, addr_size);
		if (nRet == SOCKET_ERROR)
		{
			closesocket(ListenSrvr);
			return FALSE;
		}


		//监听
		nRet = listen(ListenSrvr, 2);
		if (nRet == SOCKET_ERROR)
		{
			closesocket(ListenSrvr);
			return FALSE;
		}
		memset(&saSrc, 0, sizeof(sockaddr_in));
		nRet = getsockname(ListenSrvr, (LPSOCKADDR)&saSrc, &addr_size);
		if (nRet == SOCKET_ERROR) {
			closesocket(ListenSrvr);
			return FALSE;
		}
		localsock = ntohs(saSrc.sin_port);
		lhost = ConvertCommaAddress(inet_ntoa(server.sin_addr), localsock);

		SetModele(pClient, ModleP);
	}
	return true;

}

//接收所有数据
int RecvAlldata(SOCKET& sock, char* buffer, int size)
{
	int RecvSize;
	CString strMsg;
	while (size > 0)//剩余部分大于0
	{
		
		RecvSize = recv(sock, buffer, 1024, 0);
		if (RecvSize == 0)
			break;
		strMsg.Format(_T("%s"), buffer);
		if (SOCKET_ERROR == RecvSize)
			return false;
		size = size - RecvSize;
		buffer += RecvSize;

		//AfxMessageBox(buffer);
	}

	

	return RecvSize;
}

//显示服务端数据裂变
 void  ShowList(CFClientDlg* pClient)
{

	pClient->serverlist.DeleteAllItems();
	DataConn(/*DWORD dwIp, WORD wPort, int nMode,*/ pClient, true, true);
	CString str = "LIST \r\n";
	int i = send(pClient->ConnectSocket, str, str.GetLength(), 0);

	recv(pClient->ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	//AfxMessageBox(buffRecv);
	if (strtok(buffRecv, " \r\n") != NULL)
	{
		//AfxMessageBox(buffRecv);
		if (!strstr(buffRecv, "150"))
		{
			closesocket(pClient->ConnectSocket);
			return;
		}
	}

	memset(&saDest, 0, sizeof(sockaddr_in));
	DataSrvr = accept(ListenSrvr, (LPSOCKADDR)&saDest, &addr_size);



	if (DataSrvr == INVALID_SOCKET)
	{
		AfxMessageBox(_T("错误：连接错误 %d"), WSAGetLastError());
		/*return false;*/
	}
	int ret = RecvAlldata(DataSrvr, buffdata, 1);
	//AfxMessageBox(buffdata);
	if (ret > 0)
	{	//AfxMessageBox(buffdata);

		strtokchar();
		ShowDisk(pClient);
		/*DiskIn = true;*/
		//AfxMessageBox("ttttttttttt");
		memset(buffdata, 0, sizeof(buffdata));
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
		
	ret = RecvAlldata(DataSrvr, buffdata, 1);
	
	i = recv(pClient->ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	//AfxMessageBox(buffRecv);

	closesocket(DataSrvr);
	closesocket(ListenSrvr);
	if (ret > 0)
	{
		strtokchar();
		StarctData(pClient);
		memset(buffdata, 0, sizeof(buffdata));
	}
	else
		return;
	// }
}

//显示服务端磁盘信息
void ShowDisk(CFClientDlg* pClient)
{
	pClient->serverpath.ResetContent();
	int index = 0;
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		pClient->serverpath.AddString(*it);
		if (it == v.end() - 1)
		{
			int nIndex = pClient->serverpath.FindStringExact(0, *it);
			pClient->serverpath.SetCurSel(nIndex);
			pClient->Serverpath = *it;
		}
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//抽取出数据
void StarctData(CFClientDlg* pClient)
{
	int index = 0;
	int i = 0;


	for (auto it = v.begin(); it != v.end(); ++it)
	{
		if (i == 0)
		{
			strcpy(pFI[index].szFileName, *it);
			//memcpy(pFI[index].szFileName, szType, sizeof(pFI[index].szFileName));
			//AfxMessageBox(pFI[index].szFileName);
			i++;
			continue;
		}
		if (i == 2)
		{
			strcpy(pFI[index].szFileSize, *it);
			//memcpy(pFI[index].szFileSize, szType, sizeof(pFI[index].szFileSize));
			i++;
			//AfxMessageBox(pFI[index].szFileSize);
			continue;
		}	
		if (i == 1)
		{
			pFI[index].nType=atoi(*it);
			i++;
			continue;
		}		
		if (i == 3)
		{
			//strcpy(pFI[index].szFileDate, szType);
			strcpy(pFI[index].szFileDate, *it);
			//AfxMessageBox(pFI[index].szFileDate);
			/*i++;
			if (i == 4)
			{*/
		/*	if (pFI[index].szFileName != "." && pFI[index].szFileName != "..")*/
				pClient->InsertServerFile(pFI[index]);

			i = 0;
				index++;
				
			//}
			continue;
		}
	}
	//int count = 0;
	//while (count <= index)
	//{
	//	pClient->InsertServerFile(pFI[count]);

	//}






}
	
//分割数据
void strtokchar()
{
		//方法一：
	v.clear();
		char* p;
		p = strtok(buffdata, "\r\n");
		
	/*	p = strtok(NULL, "\r\n");*/
		while (p)
		{
			v.push_back(p);
			p = strtok(NULL, "\r\n");
		};
	
}

//下载文件
void CFClientDlg::DownloadFile(CFClientDlg* pClient,BOOL ModleP)
{
	//获取点击的文件名
	if (serverlist.GetSelectionMark() == -1)return;
	POSITION pos = serverlist.GetFirstSelectedItemPosition();
	int row = (int)serverlist.GetNextSelectedItem(pos);
	CString Downloadfile = serverlist.GetItemText(row, 0);
	//AfxMessageBox(str);

	//获取当前路径名
	int item = m_localpath.GetCurSel();
	CString strname;
	m_localpath.GetLBText(item, strname);
	if (strname.Right(1) != "\\")
		strname += "\\" + Downloadfile;
	else
		strname += Downloadfile;
	CString localfile = strname;

	CFile datafile;
	if (!datafile.Open(localfile, (CFile::modeWrite | CFile::modeCreate))) {
		AfxMessageBox("不能打开文件");
		return ;
	}

	//Sleep(1000);
	DataConn(pClient, false, true);

//	AfxMessageBox("不能打开文件");
	CString str = "RETR "+ Downloadfile +"\r\n";
	int i = send(pClient->ConnectSocket, str, str.GetLength(), 0);
	//////////////////////////////////////////////////////////////////////////
	i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	AfxMessageBox(buffRecv);
	if (strtok(buffRecv, " \r\n") != NULL)
	{
		AfxMessageBox(buffRecv);
		if (!strstr(buffRecv, "150"))
		{
			closesocket(ConnectSocket);
			return;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	if (ModleP) //主动模式下
	{
		memset(&saDest, 0, sizeof(sockaddr_in));
		DataSrvr = accept(ListenSrvr, (LPSOCKADDR)&saDest, &addr_size);
		if (DataSrvr == INVALID_SOCKET)
		{
			AfxMessageBox(_T("错误：连接错误 %d"), WSAGetLastError());
			/*return false;*/
		}


	}

	while (1)
	{
		int ret = RecvAlldata(DataSrvr, buffdata, 1);

		datafile.Write(buffdata, ret);
		if (ret < BUFFSEND)
		{
			break;
		}
	}
	/*memset(buffdata, 0, sizeof(buffdata));*/
	closesocket(DataSrvr);
	datafile.Close();
	//////////////////////////////////////////////////////////////////////////
	i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	AfxMessageBox(buffRecv);
	if (strtok(buffRecv, " \r\n") != NULL)
	{
		AfxMessageBox(buffRecv);
		if (!strstr(buffRecv, "226"))
		{
			closesocket(ConnectSocket);
			return;
		}
	}


}

//下载线程
DWORD WINAPI DownloadThreadFunc(LPVOID pParam)
{
	CFClientDlg* pClient = (CFClientDlg*)pParam;
	pClient->DownloadFile(pClient,true);
	return true;
}

//上传文件
void CFClientDlg::UpFile(CFClientDlg* pClient, BOOL ModleP)
{
	//获取点击的文件名
	if (locallist.GetSelectionMark() == -1)return;
	POSITION pos = locallist.GetFirstSelectedItemPosition();
	int row = (int)locallist.GetNextSelectedItem(pos);
	CString upfile = locallist.GetItemText(row, 0);
	//CString Serverfile = serverlist.GetItemText(row, 0);

	//AfxMessageBox(upfile);

	int item = m_localpath.GetCurSel();
	CString strname;
	m_localpath.GetLBText(item, strname);
	if (strname.Right(1) != "\\")
		strname += "\\" + upfile;
	else
		strname += upfile;
	CString localfile = strname;

	CFile dataUPfile;
	if (!dataUPfile.Open(localfile, CFile::modeRead | CFile::typeBinary, 0)) {
		AfxMessageBox("不能打开文件");
		return;
	}
	DWORD dwFileLen;
	dwFileLen = dataUPfile.GetLength();
	char *pBuff = new char[dwFileLen + 1];
	//Sleep(1000);
	DataConn(pClient, false, true);


	CString str = "STOR " + upfile + "\r\n";
	int i = send(pClient->ConnectSocket, str, str.GetLength(), 0);

	i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	AfxMessageBox(buffRecv);
	if (strtok(buffRecv, " \r\n") != NULL)
	{
		AfxMessageBox(buffRecv);
		if (!strstr(buffRecv, "150"))
		{
			closesocket(ConnectSocket);
			return;
		}
	}



	if (ModleP) //主动模式下
	{
		memset(&saDest, 0, sizeof(sockaddr_in));
		DataSrvr = accept(ListenSrvr, (LPSOCKADDR)&saDest, &addr_size);
		if (DataSrvr == INVALID_SOCKET)
		{
			AfxMessageBox(_T("错误：连接错误 %d"), WSAGetLastError());
			/*return false;*/
		}
	}

	dataUPfile.Read(pBuff, dwFileLen);

	int ret = SendAll(DataSrvr, buffdata, dwFileLen);
	

	closesocket(DataSrvr);
	dataUPfile.Close();

	i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
	AfxMessageBox(buffRecv);
	if (strtok(buffRecv, " \r\n") != NULL)
	{
		AfxMessageBox(buffRecv);
		if (!strstr(buffRecv, "226"))
		{
			closesocket(ConnectSocket);
			return;
		}
	}


}

//上传线程
DWORD WINAPI UpThreadFunc(LPVOID pParam)
{
	CFClientDlg* pClient = (CFClientDlg*)pParam;
	pClient->UpFile(pClient, true);
	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//返回上一级线程
//DWORD WINAPI BackThreadFunc(LPVOID pParam)
//{
//	/*CFClientDlg* pClient = (CFClientDlg*)pParam;
//	pClient->ChangeDir(pClient, true);
//	return true;*/
//	return true;
//}

//返回上一级
void  CFClientDlg::Back(CString ServerPath)
{
	ServerPath = "CDUP " + ServerPath + "\r\n";
	//AfxMessageBox(ServerPath);
	int i = send(ConnectSocket, ServerPath, ServerPath.GetLength(), 0);
	if (i > 0)
	{
		int i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
		//AfxMessageBox(buffRecv);
		if (strtok(buffRecv, " \r\n") != NULL)
		{
			//AfxMessageBox(buffRecv);
			if (!strstr(buffRecv, "250"))
			{
				closesocket(ConnectSocket);
				return;
			}
		}
	}

	ShowList(this);
}


//双击下一级
void  CFClientDlg::ChangeDir(CString ServerPath)
{
	//AfxMessageBox("11111");
	ServerPath = "CWD " + ServerPath + "\r\n";
	//AfxMessageBox(ServerPath);
	int i = send(ConnectSocket, ServerPath, ServerPath.GetLength(), 0);
	if (i > 0)
	{
		int i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
		//AfxMessageBox(buffRecv);
		if (strtok(buffRecv, " \r\n") != NULL)
		{
			//AfxMessageBox(buffRecv);
			if (!strstr(buffRecv, "250"))
			{
				closesocket(ConnectSocket);
				return;
			}
		}
	}

	ShowList(this);

}



//显示当前
void  CFClientDlg::CurrentDir(CString ServerPath)
{
	ServerPath = "PWD " + ServerPath + "\r\n";
	//AfxMessageBox(ServerPath);
	int i = send(ConnectSocket, ServerPath, ServerPath.GetLength(), 0);
	if (i > 0)
	{
		int i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
		//AfxMessageBox(buffRecv);
		if (strtok(buffRecv, " \r\n") != NULL)
		{
			//AfxMessageBox(buffRecv);
			if (!strstr(buffRecv, "257"))
			{
				closesocket(ConnectSocket);
				return;
			}
		}
	}

	ShowList(this);
}



//添加服务端文件夹
BOOL  CFClientDlg::createDir(CString ServerPath)
{
	ServerPath = "MKD " + ServerPath + "\r\n";
	//AfxMessageBox(ServerPath);
	int i = send(ConnectSocket, ServerPath, ServerPath.GetLength(), 0);
	if (i > 0)
	{
		int i = recv(ConnectSocket, buffRecv, sizeof(buffRecv), 0);
		AfxMessageBox(buffRecv);
		if (strtok(buffRecv, " \r\n") != NULL)
		{
			AfxMessageBox(buffRecv);
			if (!strstr(buffRecv, "200"))
			{
				closesocket(ConnectSocket);
				return false;
			}
		}
	}
	return true;
	//ShowList(this);
}




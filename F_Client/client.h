#pragma once
#include "pch.h"
#include "framework.h"
#include "F_Client.h"
#include "F_ClientDlg.h"
#include "afxdialogex.h"

#define MAX_ADDR_LEN    80
#define DATA_FTP_PORT   20     // FTP Êý¾Ý¶Ë¿Ú
#define PORT_BIND   1821
#define MAX_FILE_NUM        1024
#define BUFFSEND     1024
#define BUFFMax     1024*1024*1024
static BOOL DiskIn = false;


class CClientItem {
public:
	CString CIP;
	CString CName;
	CString Pwd;
	INT CPort;
	SOCKET Soket;
	HANDLE hThread;
	HANDLE hThread1;
	CFClientDlg* pMainWnd;
	CClientItem()
	{
		pMainWnd = NULL;
		Soket = INVALID_SOCKET;
		hThread = NULL;
		CIP = "172.20.10.5";
		Pwd = "toldo";
		CName = "toldo";

	}
};


//////////////////////////////////////////////////////////////////////
DWORD WINAPI DownloadThreadFunc(LPVOID pParam);
DWORD WINAPI ConnectThreadFunc(LPVOID pParam);
DWORD WINAPI UpThreadFunc(LPVOID pParam);


///////////////////////////////////////////////////////////////////
bool Login(CFClientDlg* pClient);
void SetModele(CFClientDlg* pClient, BOOL ModleP);
void SetType(CFClientDlg* pClient, BOOL TypeA);
char* ConvertCommaAddress(char* szAddress, WORD wPort);
int	DataConn(CFClientDlg* pClient, BOOL Type, BOOL ModleP);
int RecvAlldata(SOCKET& sock, char* buffer, int size);
void  ShowList(CFClientDlg* pClient);
void StarctData(CFClientDlg* pClient);
void strtokchar();
void ShowDisk(CFClientDlg* pClient);



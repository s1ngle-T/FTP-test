#pragma once
#include "pch.h"
//#include "framework.h"
//#include "F_Server.h"
//#include "F_ServerDlg.h"












class CFServerDlg;

//class CClientItem {
//public:
//	CString CIP;
//	CString CID;
//	CString CName;
//	INT CPort;
//	SOCKET Soket;
//	SOCKET SoketFile;
//	HANDLE hThread;
//	HANDLE hThread1;
//	CFServerDlg* pMainWnd;
//	CHAR   buffRecv[DATA_BUFSIZE];
//	CHAR   buffSend[DATA_BUFSIZE];
//	WSABUF wsaBuf;
//	SOCKET s;
//	WSAOVERLAPPED o;
//	DWORD dwBytesSend;
//	DWORD dwBytesRecv;
//	int   nStatus;
//	CClientItem()
//	{
//		pMainWnd = NULL;
//		Soket = INVALID_SOCKET;
//		hThread = NULL;
//	}
//};


//class CClientFileItem {
//public:
//	TCHAR    szFileName[MAX_PATH];
//	DWORD    dwFileAttributes;
//	FILETIME ftCreationTime;
//	FILETIME ftLastAccessTime;
//	FILETIME ftLastWriteTime;
//	DWORD    nFileSizeHigh;
//	DWORD    nFileSizeLow;
//} ;




















////////////////////////////////////

DWORD WINAPI ListenThreadFunc(LPVOID pParam);

BOOL Socket_Select(SOCKET hSocket, int nTimeOut, BOOL bRead);

BOOL WelcomeInfo(SOCKET s);

DWORD WINAPI ProcessTreadIO(LPVOID lpParameter);
DWORD WINAPI ClientPro(LPVOID pParam);

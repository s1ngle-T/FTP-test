#pragma once
#include "pch.h"
#include "framework.h"
#include "F_Server.h"
#include "F_ServerDlg.h"

#define DEFAULT_HOME_DIR    "D:\\ftp\\"   //设置默认路径
#define DATA_BUFSIZE    1024
#define WSA_RECV         0 //接收
#define WSA_SEND         1 //发送
#define LOGGED_IN       230
#define MAX_NAME_LEN    128
#define MAX_PWD_LEN     128
#define USER_OK         331
#define FTP_USER		"s1ngle"
#define FTP_PASS		"s1ngle"
#define LOGIN_FAILED    530
#define FTP_QUIT        221
#define MAX_REQ_LEN     256
#define MAX_ADDR_LEN    80
#define CMD_OK          200
#define PORT_BIND   1821
#define REPLY_MARKER    504
#define MODE_PASV       1
#define MODE_PORT       0

#define FTP_PORT        21     // FTP 控制端口
#define DATA_FTP_PORT   20     // FTP 数据端口
#define TRANS_COMPLETE  226
#define MAX_FILE_NUM        1024
#define OS_TYPE         215
#define CANNOT_FIND     550
#define BUFFSEND     1024
#define BUFFMax     1024
#define DIR_CHANGED     250


//const char* szOpeningAMode = "150 Opening ASCII mode data connection for ";
//char szDrives[MAX_PATH];

extern CString WorkPath ;


//class CFServerDlg;
class SOCKET_INF
{
public:
	CHAR   buffRecv[DATA_BUFSIZE];
	CHAR   buffSend[DATA_BUFSIZE];
	DWORD BytesSend;
	DWORD BytesRecv;
	WSABUF wsaBuf;  //操作winsocket函数所使用的数据缓冲器--->len,缓冲区的指针
	SOCKET socket;
	WSAOVERLAPPED overlap;
	CFServerDlg* pMainWnd;
	CString CIP;
	int   nStatus;
};


DWORD  WINAPI ProcessTreadIO(LPVOID pParam);
DWORD WINAPI ListenThreadFunc(LPVOID pParam);
BOOL WelcomeInfo(SOCKET socket);

int LoginIn(SOCKET_INF* pSocketInfo);
int SendRes(SOCKET_INF* pSI);

int RecvReq(SOCKET_INF *pSI);
int DealCommand(SOCKET_INF *pSI);
int ConvertDotAddress(char* szAddress, LPDWORD IpAddr, LPWORD Port);


int	DataConn(SOCKET& s, DWORD dwIp, WORD wPort, int nMode);

int RecvAlldata(SOCKET& sock, char* buffer, int size);

char* GetLocalAddress();


UINT FileListToStr(char* buff, UINT nBuffSize, BOOL bDetails, CString path = DEFAULT_HOME_DIR);

UINT DiskinfoToCs(char* buff, UINT nBuffSize, BOOL bDetails ,CString path = DEFAULT_HOME_DIR);
int GetFileList(FILE_FTP_INFO* pFI, UINT nArraySize, CString csDirPath);

DWORD TransFileToBuffer(CFile &hFile, char* buff, const char* szFileName, int len);
void TransBufferToFlie( SOCKET s,char* szFileName);
UINT SendAll(SOCKET& sock, char* buffer, int size);
UINT Senddisk(SOCKET& sock, char* buffer, int size);
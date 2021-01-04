#include "pch.h"
#include "framework.h"
//#include "F_Server.h"
//#include "F_ServerDlg.h"
#include "afxdialogex.h"
#include "Ser.h"
#include<WS2tcpip.h>
#pragma comment(lib,"Ws2_32.lib")
//#include <vector>

SOCKET_INF*ClientArry[WSA_MAXIMUM_WAIT_EVENTS];
//char szDrives[MAX_PATH];

WSAEVENT Event[WSA_MAXIMUM_WAIT_EVENTS];
DWORD EventTotal = 0;
BOOL  LoggedIn; //��½״̬��Ϣ
DWORD g_index;
CRITICAL_SECTION g_cs;
static char User[MAX_NAME_LEN], Pwd[MAX_PWD_LEN];
static DWORD dwSendBytes = 0;
HANDLE Thread1;
int static rowId = 0;
CString csDirPath = DEFAULT_HOME_DIR;
char buff[DATA_BUFSIZE];

CString WorkPath = DEFAULT_HOME_DIR;

static BOOL DiskIn = false;

//
//CFServerDlg  pPTFTrans1 =new CFServerDlg();

DWORD WINAPI ListenThreadFunc(LPVOID pParam)
{
	DWORD ThreadId;//�߳�ID
	DWORD dwFlags=0;
	DWORD dwRecvBytes=0;
	InitializeCriticalSection(&g_cs);
	CFServerDlg* pPTFTrans = (CFServerDlg*)pParam;
	ASSERT(pPTFTrans != NULL);
	//pPTFTrans1 = pPTFTrans;
	pPTFTrans->SlistenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0,
		WSA_FLAG_OVERLAPPED);
	
	//�׽���
	if (pPTFTrans->SlistenSocket == INVALID_SOCKET)
	{
		AfxMessageBox(_T("�½�socketʧ��"));
		return FALSE;
	}
	//�˿�
	int uPort = pPTFTrans->GetDlgItemInt(IDC_EDIT1);
	if (uPort != 21)
	{
		AfxMessageBox(_T("��������ȷ�Ķ˿ںţ�21"));
		goto _Error_End;

	}

	//��ʼ��
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.S_un.S_addr = INADDR_ANY;
	service.sin_port = htons(uPort);
	//��
	if (bind(pPTFTrans->SlistenSocket, (sockaddr*)&service, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("��ʧ��"));
		goto _Error_End;
	}
	else
		AfxMessageBox(_T("������������"));

	//����
	if (listen(pPTFTrans->SlistenSocket, 10) == SOCKET_ERROR /*|| listen(pChatRoom->sev_file, 10) == SOCKET_ERROR*/)
	{
		AfxMessageBox(_T("����ʧ��"));
		goto _Error_End;
	}

	///�ֶ�  ��һ���¼��Ǹ������¼��ģ���Ȼ��һֱ�յ�������Ϣ���ǲ���������
	if ((Event[0] = WSACreateEvent()) == WSA_INVALID_EVENT)
	{
		return false;
	}

	//////���߳�
	Thread1 =CreateThread(NULL, 0, ProcessTreadIO,NULL, 0, &ThreadId);
	if (Thread1 == NULL)
	{
		AfxMessageBox(_T("����CreateThread failed with error %d"), WSAGetLastError());
		return false;
	}

	EventTotal = 1;//��������

	sockaddr_in clientAddr;
	sockaddr_in addr_client_file;
	int len = sizeof(sockaddr_in);

	while (TRUE)
	{
		SOCKET accSock = accept(pPTFTrans->SlistenSocket, (struct sockaddr*) & clientAddr, &len);
		if (accSock == INVALID_SOCKET)
		{
			AfxMessageBox(_T("����accept failed with error %d"),WSAGetLastError());
			return false;
		}
		else {
				AfxMessageBox(_T("һ�����ӳɹ�"));

		}
	/*	AfxMessageBox(_T("һ�����ӳɹ�"));*/

		if (!WelcomeInfo(accSock)) break;

		if (!SetCurrentDirectory(DEFAULT_HOME_DIR)) break;   //���õ�ǰ·��ΪĬ��·��

		EnterCriticalSection(&g_cs);
		
			//����һ���µ�SOCKET_INF�ṹ������ܵ�����socket.
			ClientArry [EventTotal] = new SOCKET_INF();
			//��ʼ���µ�SOCKET_INF�ṹ
			char buff[DATA_BUFSIZE]; memset(buff, 0, DATA_BUFSIZE);
			ClientArry[EventTotal]->wsaBuf.buf = buff;
			ClientArry[EventTotal]->wsaBuf.len = DATA_BUFSIZE;
			ClientArry[EventTotal]->socket = accSock;
		
			memset(&(ClientArry[EventTotal]->overlap), 0, sizeof(OVERLAPPED));
			ClientArry[EventTotal]->BytesSend = 0;
			ClientArry[EventTotal]->BytesRecv = 0;
			ClientArry[EventTotal]->nStatus = WSA_RECV;    // ����
			ClientArry[EventTotal]->pMainWnd = pPTFTrans;
			char sendBuf[20] = { '\0' };
			ClientArry[EventTotal]->CIP = inet_ntop(AF_INET, (void*)&clientAddr.sin_addr, sendBuf, 16);

			//�����¼�
			if ((ClientArry[EventTotal]->overlap.hEvent = Event[EventTotal] =
				WSACreateEvent()) == WSA_INVALID_EVENT)
			{
				//printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
				return false;
			}
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////
			//������������
			dwFlags = 0;
			if (WSARecv(ClientArry[EventTotal]->socket,
				&(ClientArry[EventTotal]->wsaBuf), 1, &dwRecvBytes, &dwFlags,
				&(ClientArry[EventTotal]->overlap), NULL) == SOCKET_ERROR) //���һ����������н����õ��Ĳ���
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					//printf("����WSARecv() failed with error %d\n", WSAGetLastError());
					return false;
				}
			}
			EventTotal++;
		
			//�뿪�ٽ���
			LeaveCriticalSection(&g_cs);
		


		//ʹ��һ���¼����źš�ʹ�������̴߳����������¼�   ���ղ���overlapped������
		if (WSASetEvent(Event[0]) == FALSE)
		{
			AfxMessageBox(_T("����WSASetEvent failed with error %d"), WSAGetLastError());
			return false;
			/*WSAResetEvent(g_events[0]);*/
		}
		/*else
			AfxMessageBox(_T("11111111111111"), WSAGetLastError());*/
	}
_Error_End:
	closesocket(pPTFTrans->SlistenSocket);

	return true;
}

//�¼�����
 DWORD  WINAPI ProcessTreadIO(LPVOID pParam)
{
	DWORD dwFlags;
	DWORD dwBytesTransferred;
	CFServerDlg* pPTFTrans = (CFServerDlg*)pParam;
	DWORD i;
	SOCKET_INF *pSI;

	while (TRUE)
	{
		if ((g_index = WSAWaitForMultipleEvents(EventTotal, Event, FALSE,
			WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)//�¼��ĸ������ڶ�������Ϊ�¼����飬�ȴ����л��ǵ������ȴ���ʱ�䣬false
		{
			//printf("����WSAWaitForMultipleEvents failed %d\n", WSAGetLastError());
			return 0;
		}

		if ((g_index - WSA_WAIT_EVENT_0) == 0)  //
		{
			WSAResetEvent(Event[0]);
			continue;
		}


		pSI = ClientArry[g_index - WSA_WAIT_EVENT_0];
		WSAResetEvent(Event[g_index - WSA_WAIT_EVENT_0]);
////////////////////////////////////////////////////////////////////////////////////////

		if (WSAGetOverlappedResult(pSI->socket, &(pSI->overlap), &dwBytesTransferred,
			FALSE, &dwFlags) == FALSE || dwBytesTransferred == 0)  //���ͻ���ղ���ʵ�ʴ��͵��ֽ���,�Ƿ�ȴ�������ص���������,������״̬�ĸ��ӱ�־λ
		{
			if (closesocket(pSI->socket) == SOCKET_ERROR) //���<0,�ͻ��˴�������û�н��յ�����
			{
				AfxMessageBox(_T("���󣺿ͻ��˹ر�ʧ�� %d"), WSAGetLastError());
			}

			GlobalFree(pSI);
			/*free(pSI);*/
			WSACloseEvent(Event[g_index - WSA_WAIT_EVENT_0]); //����û�н��յ����ݵĻ��͹ر�
			EnterCriticalSection(&g_cs);
			if ((g_index - WSA_WAIT_EVENT_0) + 1 != EventTotal)
				for (i = g_index - WSA_WAIT_EVENT_0; i < EventTotal; i++)
				{
					Event[i] = Event[i + 1];
					ClientArry[i] = ClientArry[i + 1];
				}

			EventTotal--;
		/*	freeoverlapsocket(lpParameter, pSI);*/
			LeaveCriticalSection(&g_cs);
			continue;
		}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (pSI->nStatus == WSA_RECV)  //����ǽ�����Ϣ��״̬
			{
				memcpy(&pSI->buffRecv[pSI->BytesRecv], pSI->wsaBuf.buf, dwBytesTransferred); //�ѻ����������ݷŵ�����������
				pSI->BytesRecv += dwBytesTransferred;
				CString str;
				str.Format(_T("%s"), pSI->buffRecv);
			/*	AfxMessageBox(_T("����: str:"));*/
				if (pSI->buffRecv[pSI->BytesRecv - 2] == '\r' && pSI->buffRecv[pSI->BytesRecv - 1] == '\n' && pSI->BytesRecv > 2) // Ҫ��֤�����\r\n
				{
					if (!LoggedIn)
					{
						if (LoginIn(pSI) == LOGGED_IN)
							LoggedIn = TRUE;
					}

					else
					{
						if (DealCommand(pSI) == FTP_QUIT)
							continue;
					}
					// ���������
					memset(pSI->buffRecv, 0, sizeof(pSI->buffRecv));
					pSI->BytesRecv = 0;

				}

			}
			else
			{
				pSI->BytesSend += dwBytesTransferred;
			}

			// ���������Ժ���������
			if (RecvReq(pSI) == -1)
				return -1;

		}

		return 0;
	}

 //��ӭ��Ϣ
BOOL WelcomeInfo(SOCKET socket)
{
	char* WelcomeInfo = "220 ��ӭ����¼�� FtpServer...\r\n";

	if (send(socket, WelcomeInfo, strlen(WelcomeInfo), 0) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Ftp �ͻ�������:%d"), WSAGetLastError());
		return false;
	}
	LoggedIn = false;
	//AfxMessageBox(_T("22222222222222"));

	return TRUE;
}


//��½��Ϣ
int LoginIn(SOCKET_INF *pSI)
{
	int  nRetVal = 0;
	const char* UserOK = "331 User name okay, need password.\r\n";
	const char* LoggedIn = "230 User logged in, proceed.\r\n";
	if (strstr(pSI->buffRecv, "USER"))
	{
		sprintf(User, "%s", pSI->buffRecv + strlen("USER") + 1);
		strtok(User, "\r\n");
		sprintf(pSI->buffSend, "%s", UserOK);  //�ͻ������Ӻ������������Ҫ�������Ϣ�ÿͻ����ṩ����
		if (SendRes(pSI) == -1) return -1;
		return USER_OK;
	}

	if (strstr(pSI->buffRecv, "PASS"))
	{
		sprintf(Pwd, "%s", pSI->buffRecv + strlen("PASS") + 1);
		strtok(Pwd, "\r\n");
		// �ж��û�����������ȷ��
		if (stricmp(Pwd, FTP_PASS) || stricmp(User, FTP_USER))
		{
			sprintf(pSI->buffSend, "530 User %s cannot log in.\r\n", User);
			CString str;
			str.Format(_T("User %s cannot log in "), User);
			AfxMessageBox(str);

			nRetVal = LOGIN_FAILED;
		}
		else
		{
			sprintf(pSI->buffSend, "%s", LoggedIn);
			CString str;
			str.Format(_T("User %s logged in "), User);
		/*	AfxMessageBox(str);*/

			pSI->pMainWnd->User_List.InsertItem(pSI->pMainWnd->User_List.GetItemCount(), _T("xxx"));
			pSI->pMainWnd->User_List.SetItemText(rowId, 0, User);
			pSI->pMainWnd->User_List.SetItemText(rowId, 1, pSI->CIP);
			nRetVal = LOGGED_IN;
			rowId++;
		}
		if (SendRes(pSI) == -1)
			return -1;
	}
	return nRetVal;
}

//������Ϣ
int SendRes(SOCKET_INF *pSI)
{
	pSI->nStatus = WSA_SEND;
	//memset(&(pSI->overlap), 0, sizeof(WSAOVERLAPPED));
	pSI->overlap.hEvent = Event[g_index - WSA_WAIT_EVENT_0];
	pSI->wsaBuf.buf = pSI->buffSend + pSI->BytesSend;
	pSI->wsaBuf.len = strlen(pSI->buffSend) - pSI->BytesSend;
	if (WSASend(pSI->socket, &(pSI->wsaBuf), 1, &dwSendBytes,
		0, &(pSI->overlap), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			//printf("WSASend() failed with error %d\n", WSAGetLastError());
			AfxMessageBox(_T("WSASend() failed with error %d"), WSAGetLastError());
			return -1;
		}

	}
	return 0;
}
//������Ϣ
int RecvReq(SOCKET_INF * pSI)
{
	/*SOCKET_INF* pSI = pSocketInfo;*/
	static DWORD dwRecvBytes = 0;
	pSI->nStatus = WSA_RECV;
	DWORD dwFlags = 0;
	/*memset(&(pSI->overlap), 0, sizeof(WSAOVERLAPPED));*/
	pSI->overlap.hEvent = Event[g_index - WSA_WAIT_EVENT_0];
	pSI->wsaBuf.len = DATA_BUFSIZE;
	if (WSARecv(pSI->socket, &(pSI->wsaBuf), 1, &dwRecvBytes,
		&dwFlags, &(pSI->overlap), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			//printf("WSARecv() failed with error %d\n", WSAGetLastError());
			return -1;
		}
	////AfxMessageBox(_T("����:%s"), *(pSI->wsaBuf));
	/*AfxMessageBox(_T("����: % s"), *(pSI->buffRecv));*/
	}
	return 0;
}

///����ģʽ�½����˿�
int ConvertDotAddress(char* szAddress, LPDWORD pdwIpAddr, LPWORD pwPort)
{
	int  idx = 0, i = 0, iCount = 0;
	char szIpAddr[MAX_ADDR_LEN]; memset(szIpAddr, 0, sizeof(szIpAddr));
	char szPort[MAX_ADDR_LEN];   memset(szPort, 0, sizeof(szPort));

	*pdwIpAddr = 0; *pwPort = 0;

	while (szAddress[idx])
	{
		if (szAddress[idx] == ',')
		{
			iCount++;
			szAddress[idx] = '.';
		}
		if (iCount < 4)
			szIpAddr[idx] = szAddress[idx];
		else
			szPort[i++] = szAddress[idx];
		idx++;
	}

	if (iCount != 5) return -1;
	*pdwIpAddr = inet_addr(szIpAddr);
	if (*pdwIpAddr == INADDR_NONE) return -1;
	char* pToken = strtok(szPort + 1, ".");
	if (pToken == NULL) return -1;
	*pwPort = (WORD)(atoi(pToken) * 256);
	pToken = strtok(NULL, ".");
	if (pToken == NULL) return -1;
	*pwPort += (WORD)atoi(pToken);

	return 0;
}

//��ȡ���ص�ַ
char* GetLocalAddress()
{
	struct in_addr* pinAddr;
	LPHOSTENT	lpHostEnt;
	int			nRet;
	int			nLen;
	char        szLocalAddr[80];
	memset(szLocalAddr, 0, sizeof(szLocalAddr));
	// Get our local name
	nRet = gethostname(szLocalAddr, sizeof(szLocalAddr));
	if (nRet == SOCKET_ERROR)
	{
		return NULL;
	}
	// "Lookup" the local name
	lpHostEnt = gethostbyname(szLocalAddr);
	if (NULL == lpHostEnt)
	{
		return NULL;
	}
	// Format first address in the list
	pinAddr = ((LPIN_ADDR)lpHostEnt->h_addr);
	nLen = strlen(inet_ntoa(*pinAddr));
	if ((DWORD)nLen > sizeof(szLocalAddr))
	{
		WSASetLastError(WSAEINVAL);
		return NULL;
	}
	return inet_ntoa(*pinAddr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//������Ϣ
int DealCommand(SOCKET_INF* pSI)
{
	int nRetVal = 0;
	static SOCKET sAccept = INVALID_SOCKET;
	static SOCKET s = INVALID_SOCKET;
	static BOOL   bPasv = FALSE;

	char  szCmd[MAX_REQ_LEN];
	char  szCurrDir[MAX_PATH];
	strcpy(szCmd, pSI->buffRecv);
	if (strtok(szCmd, " \r\n") == NULL) return -1;
	strupr(szCmd);

	const char* szOpeningAMode = "150 Opening ASCII mode data connection for ";
	//const char* szOpeningAMode = "150 Opening ASCII mode  ";
	static DWORD  dwIpAddr = 0;
	static WORD   wPort = 0;
	// PORT n1,n2,n3,n4,n5,n6


	if (strstr(szCmd, "PORT"))
	{
		if (ConvertDotAddress(pSI->buffRecv + strlen("PORT") + 1, &dwIpAddr, &wPort) == -1)
			return -1;
		const char* szPortCmdOK = "200 PORT Command successful.\r\n";
		sprintf(pSI->buffSend, "%s", szPortCmdOK);
		if (SendRes(pSI) == -1) return -1;
		bPasv = FALSE;
		/*	AfxMessageBox(_T("PORT........"));*/
		return CMD_OK;
	}


	if (strstr(szCmd, "PASV"))
	{

	}


	if (strstr(szCmd, "LIST"))
	{
		/*if (bPasv) sAccept = DataAccept(s);*/
		if (!bPasv)
			sprintf(pSI->buffSend, "%s/bin/ls.\r\n", szOpeningAMode);
		/*	else
				strcpy(pSI->buffSend, "125 Data connection already open; Transfer starting.\r\n");*/

		if (SendRes(pSI) == -1)
			return -1;
		// ȡ���ļ��б���Ϣ����ת�����ַ���
		BOOL bDetails = strstr(szCmd, "LIST") ? TRUE : FALSE;
		char buff[DATA_BUFSIZE];





		////////////////////////////////////////////////////////////////////////////////////////
			/*	UINT nStrLen = FileListToStr(buff, sizeof(buff), bDetails);*/
		if (!bPasv)
		{

			/*AfxMessageBox(_T("111111......."));*/
			if (DataConn(s, dwIpAddr, wPort, MODE_PORT) == -1)
			{
				/*	AfxMessageBox(_T("222222222......."));*/
				return -1;
			}
			///////////////////////////////////////////

			/*if (!DiskIn)
			{*/
			UINT nStrLen = DiskinfoToCs(buff, sizeof(buff), bDetails, csDirPath);
			Senddisk(s, buff, nStrLen);
			//}


			////////////////////////////////////////////////////

			/*UINT */nStrLen = FileListToStr(buff, sizeof(buff), bDetails, WorkPath);
			if (SendAll(s, buff, nStrLen) == -1)
				//if (SendAll(s, (char*)buff, 1024) !=1)
				return -1;
			closesocket(s);
		}

		/*	else
			{
				DataSend(sAccept, buff, nStrLen);
				closesocket(sAccept);
			}*/
		sprintf(pSI->buffSend, "%s", "226 Transfer complete.\r\n");
		if (SendRes(pSI) == -1)
			return -1;

		return TRANS_COMPLETE;
	}


	if (strstr(szCmd, "RETR"))
	{
		CFile Transfile;
		//if (bPasv) sAccept = DataAccept(s);
		char szFileNS[MAX_PATH];
		char* szFile = strtok(NULL, " \r\n");
		//////////////////////////////////////////////////////////////

		/*Transfile.Open(szFile, CFile::modeRead | CFile::typeBinary, 0);*/
		if (!Transfile.Open(szFile,CFile::modeRead | CFile::typeBinary, 0)) {
			AfxMessageBox("���ܴ��ļ�");
			return false;
		}
		int nFileSize = Transfile.GetLength();
		//CArchive ar(&Transfile, CArchive::load);//���ݴ򿪵��ļ��������ļ����л�����
		//Serialize(ar); //���ļ�����




		//////////////////////////////////////////////////////
		//int nFileSize = CombindFileNameSize(szFile, szFileNS);
		if (nFileSize == -1)
		{
			Transfile.Close();
			sprintf(pSI->buffSend, "550 %s: ϵͳ�Ҳ���ָ�����ļ�.\r\n", szFile);
			if (SendRes(pSI) == -1)
				return -1;
			if (!bPasv) closesocket(sAccept);
			else closesocket(s);

			return CANNOT_FIND;
		}
		else
			sprintf(pSI->buffSend, "%s%s.\r\n", szOpeningAMode, szFileNS);

		if (SendRes(pSI) == -1)
			return -1;

		char* buff = new char[nFileSize + 1];



		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (NULL == buff)
		{
			printf("Not enough memory error!\n");
			return -1;
		}


		/*if (ReadFileToBuffer(szFile, buff, nFileSize) == (DWORD)nFileSize)
		{*/
		//DWORD num = TransFileToBuffer(Transfile, buff, szFile, nFileSize);
		if (TransFileToBuffer(Transfile, buff, szFile, nFileSize))
		{
			// ����Data FTP����
			/*Sleep(1000);*/
			if (bPasv)
			{
				SendAll(sAccept, buff, nFileSize);
				closesocket(sAccept);
			}
			else
			{
				if (DataConn(s, dwIpAddr, wPort, MODE_PORT) == -1)
					return -1;
				SendAll(s, buff, nFileSize);
				/*Transfile.Close();*/
				closesocket(s);
			}
		}
		if (buff != NULL)
			delete[] buff;

		sprintf(pSI->buffSend, "%s", "226 Transfer complete.\r\n");
		if (SendRes(pSI) == -1)
			return -1;


		return TRANS_COMPLETE;
	}


	if (strstr(szCmd, "STOR"))
	{
		//if (bPasv) sAccept = DataAccept(s);
		char* szFile = strtok(NULL, " \r\n");
		if (NULL == szFile) return -1;
		sprintf(pSI->buffSend, "%s%s.\r\n", szOpeningAMode, szFile);
		if (SendRes(pSI) == -1)
			return -1;
		//	// ����Data FTP����
		/*	if (bPasv)
				DataRecv(sAccept, szFile);*/
		else
		{
			if (DataConn(s, dwIpAddr, wPort, MODE_PORT) == -1)
				return -1;

			TransBufferToFlie(s, szFile);

			//DataRecv(s, szFile);
		}

		sprintf(pSI->buffSend, "%s", "226 Transfer complete.\r\n");
		if (SendRes(pSI) == -1)
			return -1;

		return TRANS_COMPLETE;
	}


	if (strstr(szCmd, "QUIT"))
	{
		sprintf(pSI->buffSend, "%s", "221 Good bye,��ӭ�´�����.\r\n");
		if (SendRes(pSI) == -1)
			return -1;
		return FTP_QUIT;
	}


	if (strstr(szCmd, "PWD"))
	{
		GetCurrentDirectory(MAX_PATH, szCurrDir);
		sprintf(pSI->buffSend, "257 \"%s\" is current directory.\r\n", szCurrDir);
		if (SendRes(pSI) == -1) return -1;
		//return CURR_DIR;
		return true;
	}



	if (strstr(szCmd, "CDUP"))
	{
		char* szDir = strtok(NULL, "\r\n");
		/*if (szDir == NULL) szDir = "\\";*/
		char szSetDir[MAX_PATH];
		strcpy(szSetDir, szDir);
		//AfxMessageBox(szSetDir);
		CString Currentpath = szSetDir;
		int j;
		for (j = Currentpath.GetLength() - 2; j >= 0; j--)
			if (Currentpath.GetAt(j) == '\\')
				break;
		Currentpath = Currentpath.Left(j);
		Currentpath += "\\";
	/*	AfxMessageBox(Currentpath);*/

		WorkPath = Currentpath;
		csDirPath = Currentpath;
		if (!SetCurrentDirectory(Currentpath))
		{
			sprintf(szCurrDir, "\\%s", WorkPath);
			sprintf(pSI->buffSend, "550 %s No such file or Directory.\r\n", WorkPath);
			nRetVal = CANNOT_FIND;
		}
		else
		{
			sprintf(pSI->buffSend, "250 Directory changed to /%s.\r\n", WorkPath);
			nRetVal = DIR_CHANGED;
		}
		if (SendRes(pSI) == -1) return -1;
		return nRetVal;

	}

	if (strstr(szCmd, "CWD"))
	{
		char* szDir = strtok(NULL, "\r\n");
		if (szDir == NULL) szDir = "\\";
		/*	char szSetDir[MAX_PATH];*/
		WorkPath = szDir;
		//WorkPath += "\\";
		csDirPath = szDir;
		//AfxMessageBox(WorkPath);
		if (!SetCurrentDirectory(WorkPath))
		{
			sprintf(szCurrDir, "\\%s", szDir);
			sprintf(pSI->buffSend, "550 %s No such file or Directory.\r\n", szCurrDir);
			nRetVal = CANNOT_FIND;
		}
		else
		{
			sprintf(pSI->buffSend, "250 Directory changed to /%s.\r\n", WorkPath);
			nRetVal = DIR_CHANGED;
		}
		if (SendRes(pSI) == -1) return -1;
		return nRetVal;

	}


	if (strstr(szCmd, "TYPE"))
	{
		char* szType = strtok(NULL, "\r\n");
		if (szType == NULL) szType = "A";
		sprintf(pSI->buffSend, "200 Type set to %s.\r\n", szType);
		//AfxMessageBox(pSI->buffSend);
		if (SendRes(pSI) == -1)
			return -1;
		return CMD_OK;
	}

	if (strstr(szCmd, "MKD"))
	{
		char* szDir = strtok(NULL, "\r\n");
		if (CreateDirectory(szDir, NULL))   //����Ŀ¼
		{
			const char* szPortCmdOK = "200 create dir successful.\r\n";
			sprintf(pSI->buffSend, "%s", szPortCmdOK);
			if (SendRes(pSI) == -1) return -1;
			return true;
		}
	}


	else
	{
		sprintf(pSI->buffSend, "500 '%s' command not understand.\r\n", szCmd);
		if (SendRes(pSI) == -1) return -1;
		return nRetVal;
	}
}

//��������
int	DataConn(SOCKET& s, DWORD dwIp, WORD wPort, int nMode)
{

// ����һ��socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		//printf("Failed to get a socket %d\n", WSAGetLastError());
		return -1;
	}

	struct sockaddr_in inetAddr;
	inetAddr.sin_family = AF_INET;
	if (nMode == MODE_PASV)
	{
		inetAddr.sin_port = htons(wPort);
		inetAddr.sin_addr.s_addr = dwIp;
	}
	else
	{
		inetAddr.sin_port = htons(DATA_FTP_PORT);
		inetAddr.sin_addr.s_addr = inet_addr(GetLocalAddress());
	}

	BOOL optval = TRUE;
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval)) == SOCKET_ERROR)
	{
		//printf("Failed to setsockopt %d.\n", WSAGetLastError());
		closesocket(s);
		return -1;
	}

	if (bind(s, (struct sockaddr*) & inetAddr, sizeof(inetAddr)) == SOCKET_ERROR)
	{
		//printf("Failed to bind a socket %d.\n", WSAGetLastError());
		closesocket(s);
		return -1;
	}

	if (MODE_PASV == nMode)
	{
		if (listen(s, SOMAXCONN) == SOCKET_ERROR)
		{
			//printf("Failed to listen a socket %d.\n", WSAGetLastError());
			closesocket(s);
			return -1;
		}
	}

	else if (MODE_PORT == nMode)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(wPort);
		addr.sin_addr.s_addr = dwIp;

		/*	printf("111111111111111");*/

		if (connect(s, (const sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			//printf("Failed to connect a socket %d\n", WSAGetLastError());
			closesocket(s);
			return -1;
		}
		///*else {
		//	AfxMessageBox("�������ӳɹ�......");
		//}*/
	
	}

	/*char buff[20];
	recv(s,  buff, sizeof(buff), 0);
	AfxMessageBox(buff);*/

	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//���ݷ���
UINT SendAll(SOCKET& sock, char* buffer, int size)
{
	/*for (int index = 0; index < ClientArry.GetCount(); index++)
	{*/
	int SendSize=0;
	while (size > 0)
	{
		if (size >=1024)
		{
			SendSize = send(sock, buffer, 1024, 0);
			if (SOCKET_ERROR == SendSize)
				return false;
			size = size - SendSize;//����ѭ���������˳�����
			buffer += SendSize;//���ڼ����ѷ�buffer��ƫ����
		}
		else
		{
			SendSize = send(sock, buffer, size, 0);
			if (SOCKET_ERROR == SendSize)
				return false;
			size = size - SendSize;//����ѭ���������˳�����
			buffer += SendSize;//���ڼ����ѷ�buffer��ƫ����
			break;
		}


	}
	return SendSize;
}


//���̷���
UINT Senddisk(SOCKET& sock, char* buffer, int size)
{
	/*for (int index = 0; index < ClientArry.GetCount(); index++)
	{*/
	int SendSize = 0;
	while (size > 0)
	{
		/*if (size > 1024)
		{*/
		SendSize = send(sock, buffer, 1024, 0);
		if (SOCKET_ERROR == SendSize)
			return false;
		size = size - SendSize;//����ѭ���������˳�����
		buffer += SendSize;//���ڼ����ѷ�buffer��ƫ����
	//}
	//else
	//{
	//	SendSize = send(sock, buffer, size, 0);
	//	if (SOCKET_ERROR == SendSize)
	//		return false;
	//	size = size - SendSize;//����ѭ���������˳�����
	//	buffer += SendSize;//���ڼ����ѷ�buffer��ƫ����
	//}


	}
	return SendSize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//����˵��ͻ��˴���
//��ȡ�б��ļ�
int GetFileList(FILE_FTP_INFO* pFI, UINT nArraySize,CString csDirPath)
{
	int index = 0;
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

		strcpy(pFI[index].szFileName, str);

		/*if (str == ".." || str == ".")
			continue;*/
		if (fileFind.IsDots())
			continue;

	


		fileFind.GetLastWriteTime(time);
		str.Format("%d/%d/%d", time.GetYear(), time.GetMonth(), time.GetDay());
		
		strcpy(pFI[index].szFileDate, str);

		if (fileFind.IsDirectory() && !fileFind.IsDots())
		{
			/*	h = GetDirSize(csDirPath);*/
			h = 0;
			pFI[index].nType = 1;

		}
		else
		{
			h = fileFind.GetLength();
			pFI[index].nType = 0;
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
		if (pFI[index].nType == 1)
			strcpy(pFI[index].szFileSize, "|");
		else
			strcpy(pFI[index].szFileSize, str);
		//OnInsertFile(&File_List, pInfo);
		index++;

	}

	fileFind.Close();
	return index;
}

//�ļ��б�תbuff׼������
UINT FileListToStr(char* buff, UINT nBuffSize, BOOL bDetails,CString path)
{
	FILE_FTP_INFO FileArry[MAX_FILE_NUM];
	int nFiles = GetFileList(FileArry, MAX_FILE_NUM, path);
	char szTemp[128];
	sprintf(buff, "%s", "");
	//strcat(buff, DEFAULT_HOME_DIR);
	//strcat(buff, "\r\n");
	if (bDetails)
	{
		for (int i = 0; i < nFiles; i++)
		{
			if (strlen(buff) > nBuffSize - 128)   break;
			strcat(buff, FileArry[i].szFileName);
			strcat(buff, "\r\n");
			char   buf[20];
			strcat(buff, itoa(FileArry[i].nType, buf,10));
			strcat(buff, "\r\n");
			strcat(buff, FileArry[i].szFileSize);
			strcat(buff, "\r\n");
			strcat(buff, FileArry[i].szFileDate);
			strcat(buff, "\r\n");
		}
	}
	return strlen(buff);
}

//��ȡ��ǰ������Ϣ
UINT DiskinfoToCs(char* buff, UINT nBuffSize, BOOL bDetails,CString csDirPath)
{
	char Drives[MAX_PATH];
	char* pDrive;
	if (!GetLogicalDriveStrings(sizeof(Drives), Drives))
	{
		AfxMessageBox("���������ʧ��");
	}

	pDrive = Drives;
	//AfxMessageBox(Drives);
	strcpy(buff, Drives);
	strcat(buff, "\r\n");

	DWORD dwType;
	int index = 0;
	while (*pDrive)
	{
		dwType = ::GetDriveType(pDrive);
		CString str;
		str.Format(_T("%s"), pDrive);

		if (dwType != DRIVE_CDROM)
		{
			if (!strstr(csDirPath, str)&& str != Drives)
			{
			///*	if (strlen(buff) > nBuffSize - 128)   break;*/
				strcat(buff, pDrive);
				strcat(buff, "\r\n");
			//	//AfxMessageBox(str);
			//	index++;
			}

		}
		pDrive += strlen(pDrive) + 1;
		//AfxMessageBox(pDrive);
	}

	strcat(buff, csDirPath);
	strcat(buff, "\r\n");
	DiskIn = TRUE;
	return strlen(buff);
}


//��ȡ�ļ���buffer
DWORD TransFileToBuffer(CFile &hFile, char* buff,const char* szFileName,int len)
{
	CString CurrentPath;
	CHAR lpFileName[MAX_PATH];
	strcpy(lpFileName, szFileName);
	//::SetCurrentDirectory(WorkPath);
	GetCurrentDirectory(MAX_PATH, lpFileName);
	CurrentPath.Format(_T("%s"), lpFileName);
	CurrentPath = CurrentPath + "\\" + szFileName;

	hFile.Read(buff, len);

	//hFile.Read(buff, len);
	hFile.Close();
	return strlen(buff);
}

//д��buffer���ļ�
void TransBufferToFlie(SOCKET s, char* szFileName)
{
	CString CurrentPath;
	CHAR lpFileName[MAX_PATH];
	//::SetCurrentDirectory(WorkPath);
	GetCurrentDirectory(MAX_PATH, lpFileName);
	CurrentPath.Format(_T("%s"), lpFileName);
	CurrentPath = CurrentPath + "\\" + szFileName;

	CString Serverfile = CurrentPath;

	CFile datafile;
	if (!datafile.Open(Serverfile, (CFile::modeWrite | CFile::modeCreate))) {
		AfxMessageBox("���ܴ��ļ�");
		return ;
	}
	char buffdata[BUFFMax];

	while (1)
	{
		int ret = RecvAlldata(s, buffdata, 1);

		datafile.Write(buffdata, ret);
		if (ret < BUFFSEND)
		{
			break;
		}
	}
	/*memset(buffdata, 0, sizeof(buffdata));*/
	closesocket(s);
	datafile.Close();

	return ;
}


//�������е�����
int RecvAlldata(SOCKET& sock, char* buffer, int size)
{
	int RecvSize;
	CString strMsg;
	while (size > 0)//ʣ�ಿ�ִ���0
	{

		RecvSize = recv(sock, buffer, 1024, 0);
		strMsg.Format(_T("%s"), buffer);
		if (SOCKET_ERROR == RecvSize)
			return false;
		size = size - RecvSize;
		buffer += RecvSize;

		//AfxMessageBox(buffer);
	}



	return RecvSize;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

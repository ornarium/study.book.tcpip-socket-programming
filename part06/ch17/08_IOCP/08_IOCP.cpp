// 08_IOCP.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>
#include <winsock2.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

// ���� ����
HINSTANCE g_hInst;
HWND g_hWnd;
SOCKET g_hListen;
SOCKET g_hClient;

HANDLE g_hIocp;

//IOCP ���� ���ؽ�Ʈ
typedef struct _SOCKET_CONTEXT{
	SOCKET hSocket;
} SOCKET_CONTEXT;

//IOCP I/O ���ؽ�Ʈ
#define IO_TYPE_SEND 0
#define IO_TYPE_RECV 1
typedef struct _IO_CONTEXT {
	WSAOVERLAPPED ov;
	BOOL type;
	char buf[100];
	WSABUF wsabuf;
} IO_CONTEXT;

//WSAAsyncSelect�� ���� ��Ʈ��ũ �̺�Ʈ �߻� �� ����(Post)�� �޽���
#define WM_MYLISTEN_NOTIFY (WM_APP+0)
#define WM_MYSOCKET_NOTIFY (WM_APP+1)

//�ڽ� ��Ʈ�� ���̵�
#define IDC_LISTEN  0
#define IDC_SEND     1
#define IDC_CLOSE    2


//IP�� ��Ʈ�� �޾� ���ͳ� �ּ� ����ü�� �����ϴ� �Լ�
BOOL GetSockAddr(LPCTSTR lpszAddress, UINT nPort, SOCKADDR_IN * lpSa) {
	memset(lpSa, 0, sizeof(SOCKADDR_IN));

	//������ ���� sockAddr�� ä��
	LPSTR lpszAscii = (LPTSTR) lpszAddress;
	lpSa->sin_family = AF_INET;

	//�־��� �ּҰ� ���ٸ� � �ּҷ�(Net�����)������ �������.
	if(lpszAscii == NULL)
		lpSa->sin_addr.s_addr = htonl(INADDR_ANY);
	else
		lpSa->sin_addr.s_addr = inet_addr(lpszAscii);

	//������ �ּҸ� ������ ���ߴٸ�
	if(lpSa->sin_addr.s_addr == INADDR_NONE) {
		//IP�ּҰ� �ƴ϶� DNS���̴�
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if(lphost != NULL)
			lpSa->sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else {
			//���� ���� ���� �� ������ �߻�
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}
	//��Ʈ ����
	lpSa->sin_port = htons((u_short)nPort);
	return TRUE;
}

void IOCP_Recv(SOCKET_CONTEXT * pSocket) {
	int nError;
	DWORD dwRecv, dwFlags = 0;

	IO_CONTEXT * pIO = new IO_CONTEXT;
	ZeroMemory(pIO, sizeof(IO_CONTEXT));
	pIO->wsabuf.buf = pIO->buf;

	pIO->wsabuf.len = 100;
	pIO->type = IO_TYPE_RECV;

	nError = WSARecv(pSocket->hSocket, &pIO->wsabuf, 1, &dwRecv, &dwFlags, &pIO->ov, NULL);
	if(nError == 0) {
		//�ѹ��� �ޱ� ����
	} else if(nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING) {
		//IOCP I/O�ޱ� ��û ����
	} else {
		// ����, ���� �ݱ�
		closesocket(pSocket->hSocket);
		//��ü �ı�
		delete pSocket;
		delete pIO;
	}
}

void IOCP_Send(SOCKET_CONTEXT * pSocket, char * pBuf) {
	int nError;
	DWORD dwSend;

	IO_CONTEXT * pIO = new IO_CONTEXT;
	ZeroMemory(pIO, sizeof(IO_CONTEXT));
	pIO->wsabuf.buf = pIO->buf;
	strcpy(pIO->wsabuf.buf, pBuf);
	pIO->wsabuf.len = sizeof(pBuf)+1;
	pIO->type = IO_TYPE_SEND;

	nError = WSASend(pSocket->hSocket, &pIO->wsabuf, 1, &dwSend, 0, &pIO->ov, NULL);
	if(nError == 0) {
		//�ѹ��� ������ ����
	} else if(nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING) {
		//IOCP I/O������ ��û ����
	} else {
		// ����, ���� �ݱ�
		closesocket(pSocket->hSocket);
		//��ü �ı�
		delete pSocket;
		delete pIO;
	}
}

void _IOCPThread(void * p) {
  SOCKET_CONTEXT * pSocket = NULL;
  IO_CONTEXT * pIO = NULL;
  BOOL bRet;
  DWORD dwTransfer;
 
  while(1) {
    bRet = GetQueuedCompletionStatus(g_hIocp, &dwTransfer, (LPDWORD)&pSocket, (LPOVERLAPPED *)&pIO, INFINITE);
    //���� ������ ����
	if(dwTransfer == 0) {
		if(pIO) {
			closesocket(pSocket->hSocket);
			delete pSocket;
			delete pIO;
		}
		continue;
   }

	//IO�� NULL�̸� I/O�Ϸ� ��Ŷ�� �ƴ϶� ����� �޽�����
	if(pIO) {
		//���� �����Ͱ� �ִ�
		if(pIO->type == IO_TYPE_RECV) {
			MessageBox(g_hWnd, pIO->buf, "����", MB_OK);

			//�ٽ� ���� �� �ְ� I/O�� ��û�Ѵ�
			IOCP_Recv(pSocket);
			//�޾����Ƿ� ���� �����͸� ó���� �� �˸��� ����� �����ش�.
			IOCP_Send(pSocket, "world");
		} else if(pIO->type == IO_TYPE_SEND) {
			//���� �����Ͱ� �ִ�.
			//ó���� ���� ������ ó����.
		}

		//IO�� �����Ѵ�
		delete pIO;
	}
  }
}


void OnBtListen() {
	//���ε� �ּ� ����ü
	SOCKADDR_IN bind_addr;

	//TCP���� ����
	//g_hListen = socket(AF_INET, SOCK_STREAM, 0);
	g_hListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(g_hListen == INVALID_SOCKET) {
		MessageBox(g_hWnd, "Listen ���� ���� ����", "�˸�", MB_OK);
		return;
	}

	//�����ϰ��� �ϴ� �ּҿ� ���� �ּ� ����ü�� ����
	if(!GetSockAddr("localhost", 6565, &bind_addr)) {
		MessageBox(g_hWnd, "�ּ� ���� ����", "�˸�", MB_OK);
		return;
	}

	if(bind(g_hListen, (PSOCKADDR)&bind_addr, sizeof(bind_addr)) ==
		SOCKET_ERROR) {
			MessageBox(g_hWnd, "bind ����", "�˸�", MB_OK);
			closesocket(g_hListen);
			return;
	}

	if(listen(g_hListen, 5) == SOCKET_ERROR) {
			MessageBox(g_hWnd, "listen ����", "�˸�", MB_OK);
			closesocket(g_hListen);
			return;
	}

	//�޽��� ����� ����ŷ ����� �������� ����
	WSAAsyncSelect(g_hListen, g_hWnd, WM_MYLISTEN_NOTIFY, FD_ACCEPT);

	/////////////////////////////////////////////////////////////////
	//// IOCP ����
	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if(g_hIocp == NULL) {
		MessageBox(g_hWnd, "IOCP ���� ����", "�˸�", MB_OK);
		closesocket(g_hListen);
		return ;
	}

	//IOCPó�� �����带 ����
	SYSTEM_INFO si;
	DWORD i;

	GetSystemInfo(&si);

	for(i = 0; i < si.dwNumberOfProcessors * 2; ++i)
		_beginthread(_IOCPThread, 0, 0);
		

	/////////////////////////////////////////////////////////////////
}
/*
void OnBtSend() {
	send(g_hClient, "hello", 6, 0);
}
*/

void OnBtClose() {
	shutdown(g_hClient, 1);
}


void OnReceive() {
	char buf[100];
	ZeroMemory(buf, 100);

	if(recv(g_hClient, buf, 100, 0) == SOCKET_ERROR) {
		if(WSAGetLastError() == WSAEWOULDBLOCK)
			return;

		closesocket(g_hClient);
		MessageBox(g_hWnd, "���� ������", "�˸�", MB_OK);
		return;
	}

	MessageBox(g_hWnd, buf, "����", MB_OK);
}

void OnMyListenNotify(WPARAM wParam, LPARAM lParam) {
	SOCKADDR_IN saRemote;
	int nEvent = WSAGETSELECTEVENT(lParam);
	int nError = WSAGETSELECTERROR(lParam);

	if(nEvent == FD_ACCEPT && nError == 0) {
		int nSockLen = sizeof(saRemote);
		//���ο� Ŭ���̾�Ʈ�� �޾Ƶ���
		g_hClient = accept(g_hListen, (PSOCKADDR) &saRemote, &nSockLen);

		//Ŭ���̾�Ʈ�� ������ ��Ʈ��ũ �̺�Ʈ ����
		//WSAAsyncSelect(g_hClient, g_hWnd, WM_MYSOCKET_NOTIFY,
		//	FD_WRITE|FD_READ|FD_CLOSE);
		//MessageBox(g_hWnd, "���Ʈ��", "�˸�", MB_OK);
		SOCKET_CONTEXT * pSocket = new SOCKET_CONTEXT;
		pSocket->hSocket = g_hClient;

		//���Ʈ�� ������ IOCP�� ������Ų��
		CreateIoCompletionPort((HANDLE)g_hClient, g_hIocp, (DWORD)pSocket, 0);

		//���ʷ� WSARecv�Լ��� WSASend�Լ��� ���.
		//�ڽ��� ���� �������� ��Ĵ�� �ϸ� ��.
		IOCP_Recv(pSocket);
	}
}

//��Ʈ��ũ �̺�Ʈ �߻��� ȣ��Ǵ� �Լ�
void OnMySocketNotify(WPARAM wParam, LPARAM lParam) {
	int nEvent = WSAGETSELECTEVENT(lParam);
	int nError = WSAGETSELECTERROR(lParam);

	switch(nEvent) {
	case FD_WRITE:
		//���۸��� ���� �����Ⱑ �����Ǿ�� ��
		break;
	case FD_READ:
		OnReceive();
		break;

	case FD_CLOSE:
		closesocket(g_hClient);
		MessageBox(g_hWnd, "���� ����!", "�˸�", MB_OK);
		break;
	}
}

 //  ������ ���ν���
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_CREATE:
		CreateWindow("BUTTON", "����", WS_CHILD|WS_VISIBLE, 10, 10, 80, 20,
			hwnd, (HMENU) IDC_LISTEN, g_hInst, NULL);
		CreateWindow("BUTTON", "������", WS_CHILD|WS_VISIBLE, 10, 40, 80, 20,
			hwnd, (HMENU) IDC_SEND, g_hInst, NULL);
		CreateWindow("BUTTON", "����", WS_CHILD|WS_VISIBLE, 10, 70, 80, 20,
			hwnd, (HMENU) IDC_CLOSE, g_hInst, NULL);
		break;
	case WM_COMMAND:
		{
		//�� ��ư�� ������ �� �߻��ϴ� �̺�Ʈ �ڵ鸵
			WORD code = HIWORD(wParam);
			WORD control_id = LOWORD(wParam);
			if(code == BN_CLICKED) {
				switch(control_id) {
					case IDC_LISTEN:
					OnBtListen();
					break;
				
				case IDC_SEND:
					//OnBtSend();
					break;
				case IDC_CLOSE:
					OnBtClose();
					break;
				
				}
			}
		}
		break;
	case WM_MYSOCKET_NOTIFY:
		OnMySocketNotify(wParam, lParam);
		break;

	case WM_MYLISTEN_NOTIFY:
		OnMyListenNotify(wParam, lParam);
		break;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	MSG msg;
	WNDCLASS wc;

	g_hInst = hInstance;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "IOCP Server";

	RegisterClass(&wc);

	g_hWnd = CreateWindow("IOCP Server",
		"IOCP Server",
		WS_OVERLAPPEDWINDOW | WS_BORDER,
		100,
		50,
		300,
		200,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	//������ �ʱ�ȭ �ؾ߸� ����� �� ����
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		MessageBox(g_hWnd, "���� �ʱ�ȭ ����!", "�˸�", MB_OK);
		return 0;
	}

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//������ �� ������ cleanup�Լ���ȣ���ؾ� ��
	closesocket(g_hListen);
	closesocket(g_hClient);
	WSACleanup();
	return 0;
}



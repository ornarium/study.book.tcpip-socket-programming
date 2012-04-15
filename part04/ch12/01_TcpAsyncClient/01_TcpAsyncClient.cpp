// 01_TcpAsyncClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

// ���� ����
HINSTANCE g_hInst;
HWND g_hWnd;
SOCKET g_hSocket;

//WSAAsyncSelect�� ���� ��Ʈ��ũ �̺�Ʈ �߻� �� ����(Post)�� �޽���
#define WM_MYSOCKET_NOTIFY (WM_APP+0)

//�ڽ� ��Ʈ�� ���̵�
#define IDC_CONNECT  0
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

void OnBtConnect() {
	//������ �ּ� ����ü
	SOCKADDR_IN des_addr;

	//TCP���� ����
	g_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(g_hSocket == INVALID_SOCKET) {
		MessageBox(g_hWnd, "���� ���� ����", "�˸�", MB_OK);
		return;
	}

	//�����ϰ��� �ϴ� �ּҿ� ���� �ּ� ����ü�� ����
	if(!GetSockAddr("localhost", 6565, &des_addr)) {
		MessageBox(g_hWnd, "�ּ� ���� ����", "�˸�", MB_OK);
		return;
	}

	//�޽��� ����� ����ŷ ����� �������� ����
	//g_hSocket���� ��Ʈ��ũ �̺�Ʈ
	//(FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE)��
	//�߻��ϸ� g_hWnd������� WM_MYSOCKET_NOTIFY�޽��� ����
	WSAAsyncSelect(g_hSocket, g_hWnd, WM_MYSOCKET_NOTIFY, FD_CONNECT|
		FD_READ|FD_WRITE|FD_CLOSE);

	//���� ����
	if(connect(g_hSocket, (PSOCKADDR)&des_addr,
		sizeof(des_addr)) == SOCKET_ERROR) {
		if(WSAGetLastError() != WSAEWOULDBLOCK) {
			MessageBox(g_hWnd, "���� ����", "�˸�", MB_OK);
			closesocket(g_hSocket);
		}
	}
}

void OnBtSend() {
	send(g_hSocket, "hello", 6, 0);
}

void OnBtClose() {
	shutdown(g_hSocket, 1);
}

void OnReceive() {
	char buf[100];
	ZeroMemory(buf, 100);

	if(recv(g_hSocket, buf, 100, 0) == SOCKET_ERROR) {
		if(WSAGetLastError() == WSAEWOULDBLOCK)
			return;

		closesocket(g_hSocket);
		MessageBox(g_hWnd, "���� ������", "�˸�", MB_OK);
		return;
	}

	MessageBox(g_hWnd, buf, "����", MB_OK);
}

//��Ʈ��ũ �̺�Ʈ �߻��� ȣ��Ǵ� �Լ�
void OnMySocketNotify(WPARAM wParam, LPARAM lParam) {
	int nEvent = WSAGETSELECTEVENT(lParam);
	int nError = WSAGETSELECTERROR(lParam);

	switch(nEvent) {
	case FD_CONNECT:
		if(nError == 0)
			MessageBox(g_hWnd, "���� ����!", "�˸�", MB_OK);
		else {
			closesocket(g_hSocket);
			MessageBox(g_hWnd, "���� ����!", "�˸�", MB_OK);
		}
		break;

	case FD_READ:
		OnReceive();
		break;

	case FD_CLOSE:
		closesocket(g_hSocket);
		MessageBox(g_hWnd, "���� ����!", "�˸�", MB_OK);
		break;
	}
}

 //  ������ ���ν���
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_CREATE:
		CreateWindow("BUTTON", "����", WS_CHILD|WS_VISIBLE, 10, 10, 80, 20,
			hwnd, (HMENU) IDC_CONNECT, g_hInst, NULL);
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
				case IDC_CONNECT:
					OnBtConnect();
					break;
				case IDC_SEND:
					OnBtSend();
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
	wc.lpszClassName = "WSAAsyncSelect Client";

	RegisterClass(&wc);

	g_hWnd = CreateWindow("WSAAsyncSelect Client",
		"WSAAsyncSelect Client",
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
	closesocket(g_hSocket);
	WSACleanup();
	return 0;
}




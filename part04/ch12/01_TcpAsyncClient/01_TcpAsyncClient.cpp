// 01_TcpAsyncClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

// 전역 변수
HINSTANCE g_hInst;
HWND g_hWnd;
SOCKET g_hSocket;

//WSAAsyncSelect에 쓰일 네트워크 이벤트 발생 시 전달(Post)될 메시지
#define WM_MYSOCKET_NOTIFY (WM_APP+0)

//자식 컨트롤 아이디
#define IDC_CONNECT  0
#define IDC_SEND     1
#define IDC_CLOSE    2


//IP와 포트를 받아 인터넷 주소 구조체를 구성하는 함수
BOOL GetSockAddr(LPCTSTR lpszAddress, UINT nPort, SOCKADDR_IN * lpSa) {
	memset(lpSa, 0, sizeof(SOCKADDR_IN));

	//접속을 윟나 sockAddr을 채움
	LPSTR lpszAscii = (LPTSTR) lpszAddress;
	lpSa->sin_family = AF_INET;

	//주어진 주소가 없다면 어떤 주소로(Net어댑터)들어오든 상관없다.
	if(lpszAscii == NULL)
		lpSa->sin_addr.s_addr = htonl(INADDR_ANY);
	else
		lpSa->sin_addr.s_addr = inet_addr(lpszAscii);

	//접속할 주소를 만들지 못했다면
	if(lpSa->sin_addr.s_addr == INADDR_NONE) {
		//IP주소가 아니라 DNS명이다
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszAscii);
		if(lphost != NULL)
			lpSa->sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else {
			//옳지 못한 인자 값 오류를 발생
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}
	//포트 설정
	lpSa->sin_port = htons((u_short)nPort);
	return TRUE;
}

void OnBtConnect() {
	//목적지 주소 구조체
	SOCKADDR_IN des_addr;

	//TCP소켓 생성
	g_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(g_hSocket == INVALID_SOCKET) {
		MessageBox(g_hWnd, "소켓 생성 실패", "알림", MB_OK);
		return;
	}

	//접속하고자 하는 주소에 대한 주소 구조체를 얻어옴
	if(!GetSockAddr("localhost", 6565, &des_addr)) {
		MessageBox(g_hWnd, "주소 설정 실패", "알림", MB_OK);
		return;
	}

	//메시지 방식의 비블로킹 모드의 소켓으로 변경
	//g_hSocket으로 네트워크 이벤트
	//(FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE)가
	//발생하면 g_hWnd윈도우로 WM_MYSOCKET_NOTIFY메시지 전달
	WSAAsyncSelect(g_hSocket, g_hWnd, WM_MYSOCKET_NOTIFY, FD_CONNECT|
		FD_READ|FD_WRITE|FD_CLOSE);

	//서버 접속
	if(connect(g_hSocket, (PSOCKADDR)&des_addr,
		sizeof(des_addr)) == SOCKET_ERROR) {
		if(WSAGetLastError() != WSAEWOULDBLOCK) {
			MessageBox(g_hWnd, "접속 실패", "알림", MB_OK);
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
		MessageBox(g_hWnd, "접속 끊어짐", "알림", MB_OK);
		return;
	}

	MessageBox(g_hWnd, buf, "받음", MB_OK);
}

//네트워크 이벤트 발생시 호출되는 함수
void OnMySocketNotify(WPARAM wParam, LPARAM lParam) {
	int nEvent = WSAGETSELECTEVENT(lParam);
	int nError = WSAGETSELECTERROR(lParam);

	switch(nEvent) {
	case FD_CONNECT:
		if(nError == 0)
			MessageBox(g_hWnd, "접속 성송!", "알림", MB_OK);
		else {
			closesocket(g_hSocket);
			MessageBox(g_hWnd, "접속 실패!", "알림", MB_OK);
		}
		break;

	case FD_READ:
		OnReceive();
		break;

	case FD_CLOSE:
		closesocket(g_hSocket);
		MessageBox(g_hWnd, "접속 종료!", "알림", MB_OK);
		break;
	}
}

 //  윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
	case WM_CREATE:
		CreateWindow("BUTTON", "접속", WS_CHILD|WS_VISIBLE, 10, 10, 80, 20,
			hwnd, (HMENU) IDC_CONNECT, g_hInst, NULL);
		CreateWindow("BUTTON", "보내기", WS_CHILD|WS_VISIBLE, 10, 40, 80, 20,
			hwnd, (HMENU) IDC_SEND, g_hInst, NULL);
		CreateWindow("BUTTON", "끊기", WS_CHILD|WS_VISIBLE, 10, 70, 80, 20,
			hwnd, (HMENU) IDC_CLOSE, g_hInst, NULL);
		break;
	case WM_COMMAND:
		{
		//각 버튼이 눌렸을 때 발생하는 이벤트 핸들링
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

	//윈속을 초기화 해야만 사용하 수 있음
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		MessageBox(g_hWnd, "윈속 초기화 실패!", "알림", MB_OK);
		return 0;
	}

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//윈속을 다 썼으면 cleanup함수를호출해야 함
	closesocket(g_hSocket);
	WSACleanup();
	return 0;
}




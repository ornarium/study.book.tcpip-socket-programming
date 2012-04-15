// MyEventSocket.cpp: implementation of the CMyEventSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "05_TcpEventClient.h"
#include "MyEventSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//윈속 라이브러리를 자동으로 초기화
WINSOCK2 _startup();
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyEventSocket::CMyEventSocket()
{

}

CMyEventSocket::~CMyEventSocket()
{

}

void CMyEventSocket::OnConnect(int nErrorCode) {
	if(nErrorCode == 0) {
		AfxMessageBox("접속 성공");
	} else {
		int err = GetLastError();
		AfxMessageBox("접속 실패");
	}
}

void CMyEventSocket::OnReceive(int nErrorCode) {
	char buf[4096];
	ZeroMemory(buf, 4096);
	int nRead = Receive(buf, 4096);

	if(nRead == 0) //연결이 귾어졌다.
		Close();
	else if(nRead == SOCKET_ERROR) {
		int nErr = GetLastError();
		//네트워크 리소스를 잠시사용못하는 경우가 아닌 모든 오류시
		//연결을 끊는다
		if(nErr != WSAEWOULDBLOCK)
			Close();
	} else {
		//데이터를 받았다
		AfxMessageBox(buf);
	}
}

void CMyEventSocket::OnClose(int nErrorCode) {
	Close();
	AfxMessageBox("접속 종료");
}
// MyAsyncSocket.cpp : implementation file
//

#include "stdafx.h"
#include "TcpAsyncServer.h"
#include "MyAsyncSocket.h"
#include "TcpAsyncServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyAsyncSocket

CMyAsyncSocket::CMyAsyncSocket()
{
}

CMyAsyncSocket::~CMyAsyncSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMyAsyncSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMyAsyncSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMyAsyncSocket member functions

void CMyAsyncSocket::OnConnect(int nErrorCode) 
{
	if(nErrorCode == 0) {
		AfxMessageBox("접속 성공");
	} else {
		int err = GetLastError();
		AfxMessageBox("접속 실패");
	}

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMyAsyncSocket::OnReceive(int nErrorCode) 
{
	char buf[2096];
	ZeroMemory(buf, 4096);
	int nRead = Receive(buf, 4096);

	if(nRead == 0) //연결이 끊어짐
		Close();
	else if (nRead == SOCKET_ERROR) {
		int nErr = GetLastError();
		//네트워크 리소스를 잠시 사용못하는 경우가 아닌
		//모든 오류 시 연결을 끊는다.
		if(nErr != WSAEWOULDBLOCK)
			Close();
	} else {
		//데이터를 받았다.
		AfxMessageBox(buf);
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL CMyAsyncSocket::AsyncSend(char * pBuf, int nLen) {
	//현재 이 버전은 보낼 데이터를 버퍼 큐로 쌓아 놓지 않는다
	if(m_nSendLeft != 0) {
		AfxMessageBox("아직 보내야 할 데이터를 다 보내지 못했습니다");
		return FALSE;
	}

	//현재 버전은 고정 크기의 버퍼에 담아 보내기 때문에 버퍼 크기보다
	//큰 데이터를 한 번에 보낼 수 없다.
	if(nLen > 4096) {
		AfxMessageBox("버퍼 크기보다 큰 데이터를 보낼 수 없습니다");
		return FALSE;
	}

	//보낼 데이터를 보내기 버퍼에 복사
	memcpy(m_SendBuf, pBuf, nLen);
	m_pSendCurBuf = m_SendBuf;
	m_nSendLeft = nLen;

	//버퍼 보내기를 시도
	DoSendBuf();

	return TRUE;
}

void CMyAsyncSocket::DoSendBuf() {
	while(m_nSendLeft > 0) {
		int nSend;

		nSend = Send(m_pSendCurBuf, m_nSendLeft);

		if(nSend == SOCKET_ERROR) {
			int nError = GetLastError();
			if(nError == WSAEWOULDBLOCK)
				break;
			else {
				Close();
				m_nSendLeft =0;
				AfxMessageBox("보내기 실패, 연결을 끊습니다");
				return ;
			}
		} else {
			//보내기 성공으로 현재 보내야 할 위치와 남은 양을 변경한다
			m_pSendCurBuf += nSend;
			m_nSendLeft -= nSend;
		}
	}
}

void CMyAsyncSocket::OnSend(int nErrorCode) 
{
	DoSendBuf();
	
	CAsyncSocket::OnSend(nErrorCode);
}

void CMyAsyncSocket::OnClose(int nErrorCode) 
{
	Close();
	AfxMessageBox("접속 종료");

	CAsyncSocket::OnClose(nErrorCode);
}

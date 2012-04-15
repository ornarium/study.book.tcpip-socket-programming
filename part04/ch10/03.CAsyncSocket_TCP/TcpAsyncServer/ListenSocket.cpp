// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "TcpAsyncServer.h"
#include "ListenSocket.h"

#include "TcpAsyncServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	CTcpAsyncServerDlg * pDlg = (CTcpAsyncServerDlg *)AfxGetMainWnd();

	if(pDlg->m_pClient) {
		//단일 클라이언트만 접속 가능한 버전
		//따라서 다른 접속은 Accept 만 임시로 하고,
		//바로 Close시켜서 쓰지 못하게 함.
		CAsyncSocket TempSocket;
		Accept(TempSocket);
		TempSocket.Close();
		AfxMessageBox("이미 접속되어 있음");

		return ;
	}
	pDlg->m_pClient = new CMyAsyncSocket();
	Accept(*pDlg->m_pClient);
	AfxMessageBox("억셉트 성공");
	CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::OnClose(int nErrorCode) 
{
	CTcpAsyncServerDlg * pDlg = (CTcpAsyncServerDlg *)AfxGetMainWnd();
	
	Close();
	
	if(pDlg->m_pClient) {
		delete pDlg->m_pClient;
		pDlg->m_pClient = NULL;
	}
	AfxMessageBox("접속 종료");
	CAsyncSocket::OnClose(nErrorCode);
}

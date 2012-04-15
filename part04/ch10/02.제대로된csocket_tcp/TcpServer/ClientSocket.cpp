// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "TcpServer.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	char buf[100];
	ZeroMemory(buf, 100);
	Receive(buf, 100);
	AfxMessageBox(buf);

	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	Close();
	AfxMessageBox("접속이 종료되었음");

	CSocket::OnClose(nErrorCode);
}

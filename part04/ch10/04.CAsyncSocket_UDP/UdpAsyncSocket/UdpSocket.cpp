// UdpSocket.cpp : implementation file
//

#include "stdafx.h"
#include "UdpAsyncSocket.h"
#include "UdpSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUdpSocket

CUdpSocket::CUdpSocket()
{
}

CUdpSocket::~CUdpSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUdpSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CUdpSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUdpSocket member functions

void CUdpSocket::OnReceive(int nErrorCode) 
{
	CString sIP, sMsg;
	UINT nPort;
	char buf[100];
	ZeroMemory(buf, 100);
	ReceiveFrom(buf, 100, sIP, nPort);
	sMsg.Format(_T("%s (%d):%s"), sIP, nPort, buf);
	AfxMessageBox(sMsg);

	CAsyncSocket::OnReceive(nErrorCode);
}

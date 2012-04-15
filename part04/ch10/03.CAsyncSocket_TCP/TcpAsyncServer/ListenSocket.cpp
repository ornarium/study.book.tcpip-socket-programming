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
		//���� Ŭ���̾�Ʈ�� ���� ������ ����
		//���� �ٸ� ������ Accept �� �ӽ÷� �ϰ�,
		//�ٷ� Close���Ѽ� ���� ���ϰ� ��.
		CAsyncSocket TempSocket;
		Accept(TempSocket);
		TempSocket.Close();
		AfxMessageBox("�̹� ���ӵǾ� ����");

		return ;
	}
	pDlg->m_pClient = new CMyAsyncSocket();
	Accept(*pDlg->m_pClient);
	AfxMessageBox("���Ʈ ����");
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
	AfxMessageBox("���� ����");
	CAsyncSocket::OnClose(nErrorCode);
}

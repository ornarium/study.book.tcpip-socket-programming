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
		AfxMessageBox("���� ����");
	} else {
		int err = GetLastError();
		AfxMessageBox("���� ����");
	}

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMyAsyncSocket::OnReceive(int nErrorCode) 
{
	char buf[2096];
	ZeroMemory(buf, 4096);
	int nRead = Receive(buf, 4096);

	if(nRead == 0) //������ ������
		Close();
	else if (nRead == SOCKET_ERROR) {
		int nErr = GetLastError();
		//��Ʈ��ũ ���ҽ��� ��� �����ϴ� ��찡 �ƴ�
		//��� ���� �� ������ ���´�.
		if(nErr != WSAEWOULDBLOCK)
			Close();
	} else {
		//�����͸� �޾Ҵ�.
		AfxMessageBox(buf);
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL CMyAsyncSocket::AsyncSend(char * pBuf, int nLen) {
	//���� �� ������ ���� �����͸� ���� ť�� �׾� ���� �ʴ´�
	if(m_nSendLeft != 0) {
		AfxMessageBox("���� ������ �� �����͸� �� ������ ���߽��ϴ�");
		return FALSE;
	}

	//���� ������ ���� ũ���� ���ۿ� ��� ������ ������ ���� ũ�⺸��
	//ū �����͸� �� ���� ���� �� ����.
	if(nLen > 4096) {
		AfxMessageBox("���� ũ�⺸�� ū �����͸� ���� �� �����ϴ�");
		return FALSE;
	}

	//���� �����͸� ������ ���ۿ� ����
	memcpy(m_SendBuf, pBuf, nLen);
	m_pSendCurBuf = m_SendBuf;
	m_nSendLeft = nLen;

	//���� �����⸦ �õ�
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
				AfxMessageBox("������ ����, ������ �����ϴ�");
				return ;
			}
		} else {
			//������ �������� ���� ������ �� ��ġ�� ���� ���� �����Ѵ�
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
	AfxMessageBox("���� ����");

	CAsyncSocket::OnClose(nErrorCode);
}

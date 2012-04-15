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

//���� ���̺귯���� �ڵ����� �ʱ�ȭ
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
		AfxMessageBox("���� ����");
	} else {
		int err = GetLastError();
		AfxMessageBox("���� ����");
	}
}

void CMyEventSocket::OnReceive(int nErrorCode) {
	char buf[4096];
	ZeroMemory(buf, 4096);
	int nRead = Receive(buf, 4096);

	if(nRead == 0) //������ �D������.
		Close();
	else if(nRead == SOCKET_ERROR) {
		int nErr = GetLastError();
		//��Ʈ��ũ ���ҽ��� ��û����ϴ� ��찡 �ƴ� ��� ������
		//������ ���´�
		if(nErr != WSAEWOULDBLOCK)
			Close();
	} else {
		//�����͸� �޾Ҵ�
		AfxMessageBox(buf);
	}
}

void CMyEventSocket::OnClose(int nErrorCode) {
	Close();
	AfxMessageBox("���� ����");
}
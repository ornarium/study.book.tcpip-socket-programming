// EventSocket.cpp: implementation of the CEventSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "05_TcpEventClient.h"
#include "EventSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEventSocket::CEventSocket()
{
	m_hSocket = INVALID_SOCKET;
	m_hEvent = m_hClose = m_hThread = 0;
}

CEventSocket::~CEventSocket()
{
	Close();
}

void CEventSocket::Close() {
	if(m_hSocket != INVALID_SOCKET) {
		//�ϴ� ��Ʈ��ũ�� ������� ���ϰ� ����
		EventSelect(0);
		//������ ����
		WSASetEvent(m_hClose);
	}
}

//���� �����峻������ ȣ��Ǵ� ���� �Լ�
void CEventSocket::_CloseSocket() {
	if(m_hEvent) CloseHandle(m_hEvent);
	if(m_hClose) CloseHandle(m_hClose);
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}

//���� ������
void CEventSocket::_MonitorThread(LPVOID pParam) {
	CEventSocket * psk = (CEventSocket *)pParam;

	BOOL bContinue = TRUE;
	WSAEVENT EventToWaitFOr[2];
	EventToWaitFor[0] = psk->m_hClose; //���� ��� �̺�Ʈ
	EventToWaitFor[1] = psk->m_hEvent; //��Ʈ��ũ �̺�Ʈ�� �޴� �̺�Ʈ ��ü

	while(bContinue) {
		DWORD dwHandleSignaled = WSAWaitForMultipleEvents(2,
			EventToWaitFor, FALSE, INFINITE, TRUE);

		//� �̺�Ʈ�� �߻��ߴ°�?
		switch(dwHandleSignaled) {
		case WSA_WAIT_EVENT_0: //CloseEvent �߻�
			bContinue = FALSE;
			break;
		case WSA_WAIT_EVENT_0 + 1: // SocketEvent �߻�
			{
			WSANETWORKEVENTS NetEvents;
			//�� ���� �߻��� �̺�Ʈ���� �޾ƺ���
			int ce = WSAEnumNetworkEvents(psk->m_hSocket, psk->m_hEvent, &NetEvents);
			//�ϳ� �̻��� �̺�Ʈ�� ���ÿ� ���� �� �ִ�
			if(NetEvents.lNetworkEvents & FD_ACCEPT)
				psk->OnAccept(NetEvents.iErrorCode[FD_ACCEPT_BIT]);
			if(NetEvents.lNetworkEvents & FD_CONNECT)
				psk->OnConnect(NetEvents.iErrorCode[FD_CONNECT_BIT]);
			if(NetEvents.lNetworkEvents & FD_WRITE)
				psk->OnSend(NetEvents.iErrorCode[FD_WRITE_BIT]);
			if(NetEvents.lNetworkEvents & FD_READ)
				psk->OnReceive(NetEvents.iErrorCode[FD_READ_BIT]);
			if(NetEvents.lNetworkEvents & FD_CLOSE)
				psk->OnClose(NetEvents.iErrorCode[FD_CLOSE_BIT]);
			if(NetEvents.lNetworkEvents & FD_OOB)
				psk->OnOutOfBandData(NetEvents.iErrorCode[FD_OOB_BIT]);
			if(NetEvents.lNetworkEvents & FD_QOS)
				psk->OnQualityOfService(NetEvents.iErrorCode[FD_QOS_BIT]);

			if(NetEvents.lNetworkEvents & FD_GROUP_QOS)
				psk->OnGroupQualityOfService(NetEvents.iErrorCode[FD_GROUP_QOS_BIT]);
			}
			break;
		}
	}

	//Close�Լ� ȣ�� �� �Ʒ� �ڵ尡 ȣ��Ǹ� ������ �����.
	psk->_CloseSocket();
}

BOOL CEventSocket::GetSockAddr(LPCTSTR lpszAddress, UINT nPort, SOCKADDR_IN * lpSa) {
	memset(lpSa, 0, sizeof(SOCKADDR_IN);

	//������ ���� sockAddr�� ä���
	LPSTR lpszAscii = (LPTSTR)lpszAddress;
	lpSz->sin_family = AF_INET;

	//�־��� �ּҰ� ���ٸ� � �ּҷ�(Net�����) ������ ��� ����.
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

//���� ����
BOOL CEventSocket::Create(UINT nSocketPort /*=0*/, int nSocketType/*=SOCK_STREAM*/,
						  long lEvent /*=FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE */,
						  LPCTSTR lpszSocketAddress /*= NULL */) {
	m_hSocket = socket(AF_INET, nSocketType, 0);
	if(m_hSocket == SOCKET_ERROR)
		return FALSE;
	m_hEvent = WSACreateEvent();
	m_hClose = WSACreateEvent();

	if(EventSelect(lEvent)) {
		SOCKADDR_IN saLocal;
		if(GetSockAddr(lpszSocketAddress, nSOcketPort, &saLocal)) {
			if(bind(m_hSocket, (PSOCKADDR) &saLocal, sizeof(SOCKADDR))
				!= SOCKET_ERROR) {
				m_hThread = (HANDLE)_beginthread(_MonitorThread, 0, (void*) this);
				return TRUE;
			}
		}
	}

	Close();
	return FALSE;
}

//Ŭ���̾�Ʈ ���Ͽ� ���� �Լ�
BOOL CEventSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort) {
	SOCKADDR_IN saRemote;
	if(GetSockAddr(lpszHostAddress, nHostPort, &saRemote)) {
		if(connect(m_hSocket, (PSOCKADDR) &saRemote,
			sizeof(SOCKADDR)) == SOCKET_ERROR) {
			if(WSAGetLastError() == WSAEWOULDBLOCK)
				return TRUE;
		}
	}

	return FALSE;
}

//���� ���� ���Ͽ� Accept �Լ�
BOOL CEventSocket::Accept(CEventSocket& rConnectedSocket, SOCKADDR* lpSockAddr /*=NULL*/,
						  int * lpSockAddrLen /*=NULL */) {
	//���� Accept�� ó��
	rConnectedSocket.m_hSocket = accept(m_hSocket, lpSockAddr, lpSockAddrLen);

	if(rConnectedSocket.m_hSocket == INVALID_SOCKET)
		return FALSE;

	//������ ���ϰ� ����� ���� �ʱ�ȭ
	rConnectedSocket.m_hClose = WSACreateEvent();
	rConnectedSocket.m_hEvent = WSACreateEvent();

	// �⺻������ ��� ���� �̺�Ʈ ó���� �Ѵٰ� ����
	rConnectedSocket.EventSelect();
	//���� ���� �̺�Ʈ ó���� ���� ����� �����带 ����
	rConnectedSocket.m_hThread = (HANDLE) _beginthread(_MonitorThread,
		0, (void*)&rConnectedSocket);

	//���� ����
	return TRUE;
}
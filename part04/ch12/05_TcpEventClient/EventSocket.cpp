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
		//일단 네트워크를 사용하지 못하게 막음
		EventSelect(0);
		//스레드 종료
		WSASetEvent(m_hClose);
	}
}

//소켓 스레드내에서만 호출되는 종료 함수
void CEventSocket::_CloseSocket() {
	if(m_hEvent) CloseHandle(m_hEvent);
	if(m_hClose) CloseHandle(m_hClose);
	closesocket(m_hSocket);
	m_hSocket = INVALID_SOCKET;
}

//소켓 스레드
void CEventSocket::_MonitorThread(LPVOID pParam) {
	CEventSocket * psk = (CEventSocket *)pParam;

	BOOL bContinue = TRUE;
	WSAEVENT EventToWaitFOr[2];
	EventToWaitFor[0] = psk->m_hClose; //소켓 대기 이벤트
	EventToWaitFor[1] = psk->m_hEvent; //네트워크 이벤트를 받는 이벤트 객체

	while(bContinue) {
		DWORD dwHandleSignaled = WSAWaitForMultipleEvents(2,
			EventToWaitFor, FALSE, INFINITE, TRUE);

		//어떤 이벤트가 발생했는가?
		switch(dwHandleSignaled) {
		case WSA_WAIT_EVENT_0: //CloseEvent 발생
			bContinue = FALSE;
			break;
		case WSA_WAIT_EVENT_0 + 1: // SocketEvent 발생
			{
			WSANETWORKEVENTS NetEvents;
			//그 동안 발생됨 이벤트들을 받아본다
			int ce = WSAEnumNetworkEvents(psk->m_hSocket, psk->m_hEvent, &NetEvents);
			//하나 이상의 이벤트를 동시에 받을 수 있다
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

	//Close함수 호출 시 아래 코드가 호출되며 스레드 종료됨.
	psk->_CloseSocket();
}

BOOL CEventSocket::GetSockAddr(LPCTSTR lpszAddress, UINT nPort, SOCKADDR_IN * lpSa) {
	memset(lpSa, 0, sizeof(SOCKADDR_IN);

	//접속을 위한 sockAddr을 채운다
	LPSTR lpszAscii = (LPTSTR)lpszAddress;
	lpSz->sin_family = AF_INET;

	//주어진 주소가 없다면 어떤 주소로(Net어댑터) 들어오든 상관 없다.
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
			//옳지 못한 인자 값 오류가 발생
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	//포트 설정
	lpSa->sin_port = htons((u_short)nPort);
	return TRUE;
}

//소켓 생성
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

//클라이언트 소켓용 접속 함수
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

//서버 리슨 소켓용 Accept 함수
BOOL CEventSocket::Accept(CEventSocket& rConnectedSocket, SOCKADDR* lpSockAddr /*=NULL*/,
						  int * lpSockAddrLen /*=NULL */) {
	//실제 Accept를 처리
	rConnectedSocket.m_hSocket = accept(m_hSocket, lpSockAddr, lpSockAddrLen);

	if(rConnectedSocket.m_hSocket == INVALID_SOCKET)
		return FALSE;

	//접속한 소켓과 통신할 소켓 초기화
	rConnectedSocket.m_hClose = WSACreateEvent();
	rConnectedSocket.m_hEvent = WSACreateEvent();

	// 기본적으로 모든 소켓 이벤트 처리를 한다고 설정
	rConnectedSocket.EventSelect();
	//실제 소켓 이벤트 처리를 위한 모니터 스레드를 생성
	rConnectedSocket.m_hThread = (HANDLE) _beginthread(_MonitorThread,
		0, (void*)&rConnectedSocket);

	//성공 리턴
	return TRUE;
}
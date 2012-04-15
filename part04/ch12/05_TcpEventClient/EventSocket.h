// EventSocket.h: interface for the CEventSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENTSOCKET_H__DB14DED3_461D_4BE3_970F_67252CDB14F1__INCLUDED_)
#define AFX_EVENTSOCKET_H__DB14DED3_461D_4BE3_970F_67252CDB14F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//기본적인 소켓 라이브러리 설정
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>

struct WINSOCK2 {
	WINSOCK2() {
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2,2), &wsaData);
	}
	~WINSOCK2() {
		WSACleanup();
	}

class CEventSocket  
{
public:
	CEventSocket();
	virtual ~CEventSocket();

	SOCKET m_hSocket; //소켓 핸들
	WSAEVENT m_hEvent; //네트워크 이벤트 객체
	WSAEVENT m_hClose; //스레드 종료 이벤트 객체
	HANDLE m_hTHread; //스레드 핸들

	static void _MonitorThread(LPVOID pParam);
	static BOOL GetSockAddr(LPCTSTR lpszAddress, UINT nPort, SOCKADDR_IN* lpSa);

	BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM,
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_SCCEPT | FD_CONNECT | FD_CLOSE,
		LPCTSTR lpszSocketAddress = NULL);

	void Close();

	int GetLastError() {
		return WSAGetLastError();
	}

	BOOL Listen(int nConnectionBacklog = 5) {
		return (SOCKET_ERROR != listen(m_hSocket, nConnectionBackLog));
	}

	BOOL ShutDown(int nHow = 1) {
		return (SOCKET_ERROR != shutdown(m_hSocket, nHow));
	}

	BOOL EventSelect(long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_SCCEPT | FD_CONNECT | FD_CLOSE) {
		return (SOCKET_ERROR != WSAEventSelect( m_hSocket, m_hEvent, lEvent));
	}

	BOOL Connect(LPCTSTR lpszHostAddress, UINT nHostPort);

	BOOL Accept(CEventSocket& rConnectedSocket, SOCKADDR* lpSockAddr = NULL, int * lpSockAddrLen = NULL);

	virtual int Send(const char * lpBuf, int nBufLen) {
		return send(m_hSocket, lpBuf, nBufLen, 0);
	}

	virtual int Receive(const char * lpBuf, int nBufLen) {
		return recv(m_hSocket, lpBuf, nBufLen, 0);
	}

protected:
	virtual void OnConnect(int nErrorCode) {}
	virtual void OnAccept(int nErrorCode) {}
	virtual void OnReceive(int nErrorCode) {}
	virtual void OnSend(int nErrorCode) {}
	virtual void OnOutOfBandData(int nErrorCode) {}
	virtual void OnClose(int nErrorCode) {}
	virtual void OnQualityOfService(int nErrorCode) {}
	virtual void OnGroupQualityOfService(int nErrorCode) {}

	void _CloseSocket();
};

#endif // !defined(AFX_EVENTSOCKET_H__DB14DED3_461D_4BE3_970F_67252CDB14F1__INCLUDED_)

#if !defined(AFX_MYASYNCSOCKET_H__0CF0B520_00FE_4F04_9F18_455574A30086__INCLUDED_)
#define AFX_MYASYNCSOCKET_H__0CF0B520_00FE_4F04_9F18_455574A30086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyAsyncSocket.h : header file
//

#include "stdafx.h"
#include "TcpAsyncServer.h"

//AsyncClient에서 쓰였던 CMYAsyncSocket을 가져다 쓴것.
//OnClose를 추가로 구현하여 사용한다.

/////////////////////////////////////////////////////////////////////////////
// CMyAsyncSocket command target

class CMyAsyncSocket : public CAsyncSocket
{
// Attributes
public:

	char m_SendBuf[4096]; //보낼 데이터 버퍼
	char * m_pSendCurBuf; //보내야 할 버퍼의 현재 위치
	int m_nSendLeft; //보내기 위해 남은 길이
	
// Operations
public:
	CMyAsyncSocket();
	virtual ~CMyAsyncSocket();

	BOOL AsyncSend(char * pBuf, int nLen);
	void DoSendBuf();
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAsyncSocket)
	public:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMyAsyncSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYASYNCSOCKET_H__0CF0B520_00FE_4F04_9F18_455574A30086__INCLUDED_)

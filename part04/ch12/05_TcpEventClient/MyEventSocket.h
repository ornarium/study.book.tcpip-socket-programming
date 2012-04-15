// MyEventSocket.h: interface for the CMyEventSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEVENTSOCKET_H__40673F19_A72B_4502_86C2_409AC60C4FE4__INCLUDED_)
#define AFX_MYEVENTSOCKET_H__40673F19_A72B_4502_86C2_409AC60C4FE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EventSocket.h"

class CMyEventSocket : public CEventSocket  
{
public:
	CMyEventSocket();
	virtual ~CMyEventSocket();

protected:
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

#endif // !defined(AFX_MYEVENTSOCKET_H__40673F19_A72B_4502_86C2_409AC60C4FE4__INCLUDED_)

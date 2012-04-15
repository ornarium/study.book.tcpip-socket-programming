#if !defined(AFX_LISTENSOCKET_H__CCC765B4_CE40_4279_8DE8_DA34B1BEAF69__INCLUDED_)
#define AFX_LISTENSOCKET_H__CCC765B4_CE40_4279_8DE8_DA34B1BEAF69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//

#include "ClientSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target

class CListenSocket : public CSocket
{
// Attributes
public:
	CClientSocket m_Client;

// Operations
public:
	CListenSocket();
	virtual ~CListenSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__CCC765B4_CE40_4279_8DE8_DA34B1BEAF69__INCLUDED_)

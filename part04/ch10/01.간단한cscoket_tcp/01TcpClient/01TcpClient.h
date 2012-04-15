// 01TcpClient.h : main header file for the 01TCPCLIENT application
//

#if !defined(AFX_01TCPCLIENT_H__414F8ABA_EEC0_4943_90B6_362E60D8FD00__INCLUDED_)
#define AFX_01TCPCLIENT_H__414F8ABA_EEC0_4943_90B6_362E60D8FD00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy01TcpClientApp:
// See 01TcpClient.cpp for the implementation of this class
//

class CMy01TcpClientApp : public CWinApp
{
public:
	CMy01TcpClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy01TcpClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy01TcpClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_01TCPCLIENT_H__414F8ABA_EEC0_4943_90B6_362E60D8FD00__INCLUDED_)

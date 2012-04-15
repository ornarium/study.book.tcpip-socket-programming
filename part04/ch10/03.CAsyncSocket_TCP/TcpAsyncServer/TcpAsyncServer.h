// TcpAsyncServer.h : main header file for the TCPASYNCSERVER application
//

#if !defined(AFX_TCPASYNCSERVER_H__5D58065B_D9AE_45E5_8E8E_3F831C7CF139__INCLUDED_)
#define AFX_TCPASYNCSERVER_H__5D58065B_D9AE_45E5_8E8E_3F831C7CF139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncServerApp:
// See TcpAsyncServer.cpp for the implementation of this class
//

class CTcpAsyncServerApp : public CWinApp
{
public:
	CTcpAsyncServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpAsyncServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTcpAsyncServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPASYNCSERVER_H__5D58065B_D9AE_45E5_8E8E_3F831C7CF139__INCLUDED_)

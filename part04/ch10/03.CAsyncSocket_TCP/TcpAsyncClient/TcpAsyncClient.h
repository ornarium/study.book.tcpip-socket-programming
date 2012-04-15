// TcpAsyncClient.h : main header file for the TCPASYNCCLIENT application
//

#if !defined(AFX_TCPASYNCCLIENT_H__C6A423F1_668C_4B64_A670_BE2FB2706624__INCLUDED_)
#define AFX_TCPASYNCCLIENT_H__C6A423F1_668C_4B64_A670_BE2FB2706624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncClientApp:
// See TcpAsyncClient.cpp for the implementation of this class
//

class CTcpAsyncClientApp : public CWinApp
{
public:
	CTcpAsyncClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpAsyncClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTcpAsyncClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPASYNCCLIENT_H__C6A423F1_668C_4B64_A670_BE2FB2706624__INCLUDED_)

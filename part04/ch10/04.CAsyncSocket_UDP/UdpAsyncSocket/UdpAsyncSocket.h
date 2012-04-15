// UdpAsyncSocket.h : main header file for the UDPASYNCSOCKET application
//

#if !defined(AFX_UDPASYNCSOCKET_H__E3321F72_6BB1_4876_9F1C_112D028C9700__INCLUDED_)
#define AFX_UDPASYNCSOCKET_H__E3321F72_6BB1_4876_9F1C_112D028C9700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUdpAsyncSocketApp:
// See UdpAsyncSocket.cpp for the implementation of this class
//

class CUdpAsyncSocketApp : public CWinApp
{
public:
	CUdpAsyncSocketApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdpAsyncSocketApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUdpAsyncSocketApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPASYNCSOCKET_H__E3321F72_6BB1_4876_9F1C_112D028C9700__INCLUDED_)

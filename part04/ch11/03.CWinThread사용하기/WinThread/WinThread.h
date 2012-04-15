// WinThread.h : main header file for the WINTHREAD application
//

#if !defined(AFX_WINTHREAD_H__D4432800_39BA_4602_AD70_021C5A2C49CC__INCLUDED_)
#define AFX_WINTHREAD_H__D4432800_39BA_4602_AD70_021C5A2C49CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinThreadApp:
// See WinThread.cpp for the implementation of this class
//

class CWinThreadApp : public CWinApp
{
public:
	CWinThreadApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinThreadApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinThreadApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTHREAD_H__D4432800_39BA_4602_AD70_021C5A2C49CC__INCLUDED_)

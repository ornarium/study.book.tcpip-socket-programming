#if !defined(AFX_MYWINTHREAD_H__BCAD2461_50CB_4C28_828F_3CB36BA41B73__INCLUDED_)
#define AFX_MYWINTHREAD_H__BCAD2461_50CB_4C28_828F_3CB36BA41B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyWinThread.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CMyWinThread thread

#define TM_BLOCKING_CALL (WM_APP+0)

class CMyWinThread : public CWinThread
{
	DECLARE_DYNCREATE(CMyWinThread)
protected:
	CMyWinThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyWinThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyWinThread();

	// Generated message map functions
	//{{AFX_MSG(CMyWinThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnBlockingCall(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWINTHREAD_H__BCAD2461_50CB_4C28_828F_3CB36BA41B73__INCLUDED_)

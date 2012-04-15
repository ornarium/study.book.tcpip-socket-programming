// WinThreadDlg.h : header file
//

#if !defined(AFX_WINTHREADDLG_H__BCB210CD_1D4E_4D1A_9180_EB51B81218F3__INCLUDED_)
#define AFX_WINTHREADDLG_H__BCB210CD_1D4E_4D1A_9180_EB51B81218F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyWinThread.h"

/////////////////////////////////////////////////////////////////////////////
// CWinThreadDlg dialog

class CWinThreadDlg : public CDialog
{
// Construction
public:
	CWinThreadDlg(CWnd* pParent = NULL);	// standard constructor

	CWinThread * m_pThread;
// Dialog Data
	//{{AFX_DATA(CWinThreadDlg)
	enum { IDD = IDD_WINTHREAD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinThreadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinThreadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCreateMywinthread();
	afx_msg void OnTmBlockingCall();
	afx_msg void OnBlockingCall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINTHREADDLG_H__BCB210CD_1D4E_4D1A_9180_EB51B81218F3__INCLUDED_)

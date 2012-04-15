// UdpAsyncSocketDlg.h : header file
//

#if !defined(AFX_UDPASYNCSOCKETDLG_H__D552E03F_37B4_45DD_B762_AF9E838C8B14__INCLUDED_)
#define AFX_UDPASYNCSOCKETDLG_H__D552E03F_37B4_45DD_B762_AF9E838C8B14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UdpSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CUdpAsyncSocketDlg dialog

class CUdpAsyncSocketDlg : public CDialog
{
// Construction
public:
	CUdpAsyncSocketDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUdpAsyncSocketDlg)
	enum { IDD = IDD_UDPASYNCSOCKET_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdpAsyncSocketDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	CUdpSocket m_Udp;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUdpAsyncSocketDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCreate();
	afx_msg void OnSend();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPASYNCSOCKETDLG_H__D552E03F_37B4_45DD_B762_AF9E838C8B14__INCLUDED_)

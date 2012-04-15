// 05_TcpEventClientDlg.h : header file
//

#if !defined(AFX_05_TCPEVENTCLIENTDLG_H__3CD20AA1_BEE6_4B6A_809A_BA69BB7BF356__INCLUDED_)
#define AFX_05_TCPEVENTCLIENTDLG_H__3CD20AA1_BEE6_4B6A_809A_BA69BB7BF356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyEventSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CMy05_TcpEventClientDlg dialog

class CMy05_TcpEventClientDlg : public CDialog
{
// Construction
public:
	CMy05_TcpEventClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMy05_TcpEventClientDlg)
	enum { IDD = IDD_MY05_TCPEVENTCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy05_TcpEventClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL


	CMyEventSocket m_Client;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy05_TcpEventClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnSend();
	afx_msg void OnReceive();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_05_TCPEVENTCLIENTDLG_H__3CD20AA1_BEE6_4B6A_809A_BA69BB7BF356__INCLUDED_)

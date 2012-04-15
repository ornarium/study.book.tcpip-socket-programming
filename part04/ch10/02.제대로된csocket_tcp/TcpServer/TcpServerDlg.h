// TcpServerDlg.h : header file
//

#if !defined(AFX_TCPSERVERDLG_H__C72E2106_88E3_4F38_8184_9E405FCB0F62__INCLUDED_)
#define AFX_TCPSERVERDLG_H__C72E2106_88E3_4F38_8184_9E405FCB0F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListenSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CTcpServerDlg dialog

class CTcpServerDlg : public CDialog
{
// Construction
public:
	CTcpServerDlg(CWnd* pParent = NULL);	// standard constructor

	CListenSocket m_Listen;
// Dialog Data
	//{{AFX_DATA(CTcpServerDlg)
	enum { IDD = IDD_TCPSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTcpServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnListen();
	afx_msg void OnSend();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPSERVERDLG_H__C72E2106_88E3_4F38_8184_9E405FCB0F62__INCLUDED_)

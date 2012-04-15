// TcpClientDlg.h : header file
//

#if !defined(AFX_TCPCLIENTDLG_H__3FCC4CED_6E6A_4E64_B676_6F104EFCDB7C__INCLUDED_)
#define AFX_TCPCLIENTDLG_H__3FCC4CED_6E6A_4E64_B676_6F104EFCDB7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTcpClientDlg dialog

#include "ClientSocket.h"

class CTcpClientDlg : public CDialog
{
// Construction
public:
	CTcpClientDlg(CWnd* pParent = NULL);	// standard constructor

	CClientSocket m_Socket;

// Dialog Data
	//{{AFX_DATA(CTcpClientDlg)
	enum { IDD = IDD_TCPCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTcpClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnSend();
	afx_msg void OnClose();
	afx_msg void OnGetSource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPCLIENTDLG_H__3FCC4CED_6E6A_4E64_B676_6F104EFCDB7C__INCLUDED_)

// TcpAsyncServerDlg.h : header file
//

#if !defined(AFX_TCPASYNCSERVERDLG_H__1587CC13_879F_4C94_A861_58DC6722C0E4__INCLUDED_)
#define AFX_TCPASYNCSERVERDLG_H__1587CC13_879F_4C94_A861_58DC6722C0E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MyAsyncSocket.h"
#include "ListenSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncServerDlg dialog

class CTcpAsyncServerDlg : public CDialog
{
// Construction
public:
	CTcpAsyncServerDlg(CWnd* pParent = NULL);	// standard constructor
	
	CMyAsyncSocket * m_pClient;
	CListenSocket  m_Listen;
// Dialog Data
	//{{AFX_DATA(CTcpAsyncServerDlg)
	enum { IDD = IDD_TCPASYNCSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpAsyncServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTcpAsyncServerDlg)
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

#endif // !defined(AFX_TCPASYNCSERVERDLG_H__1587CC13_879F_4C94_A861_58DC6722C0E4__INCLUDED_)

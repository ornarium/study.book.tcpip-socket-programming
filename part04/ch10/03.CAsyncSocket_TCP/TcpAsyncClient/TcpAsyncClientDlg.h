// TcpAsyncClientDlg.h : header file
//

#if !defined(AFX_TCPASYNCCLIENTDLG_H__ADDC347D_ADD6_4261_8595_7731D43967B5__INCLUDED_)
#define AFX_TCPASYNCCLIENTDLG_H__ADDC347D_ADD6_4261_8595_7731D43967B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyAsyncSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncClientDlg dialog

class CTcpAsyncClientDlg : public CDialog
{
// Construction
public:
	CTcpAsyncClientDlg(CWnd* pParent = NULL);	// standard constructor

	CMyAsyncSocket m_Client;
// Dialog Data
	//{{AFX_DATA(CTcpAsyncClientDlg)
	enum { IDD = IDD_TCPASYNCCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTcpAsyncClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTcpAsyncClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnSend();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPASYNCCLIENTDLG_H__ADDC347D_ADD6_4261_8595_7731D43967B5__INCLUDED_)

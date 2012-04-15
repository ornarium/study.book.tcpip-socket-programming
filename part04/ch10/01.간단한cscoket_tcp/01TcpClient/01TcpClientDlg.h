// 01TcpClientDlg.h : header file
//

#if !defined(AFX_01TCPCLIENTDLG_H__7F9CFA3A_6A70_4490_AF17_FEEB225100DE__INCLUDED_)
#define AFX_01TCPCLIENTDLG_H__7F9CFA3A_6A70_4490_AF17_FEEB225100DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy01TcpClientDlg dialog

class CMy01TcpClientDlg : public CDialog
{
// Construction
public:
	CSocket m_Socket;

	CMy01TcpClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMy01TcpClientDlg)
	enum { IDD = IDD_MY01TCPCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy01TcpClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy01TcpClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnSend();
	afx_msg void OnRecv();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_01TCPCLIENTDLG_H__7F9CFA3A_6A70_4490_AF17_FEEB225100DE__INCLUDED_)

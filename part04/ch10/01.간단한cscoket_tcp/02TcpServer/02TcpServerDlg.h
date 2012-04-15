// 02TcpServerDlg.h : header file
//

#if !defined(AFX_02TCPSERVERDLG_H__76C8F7EF_70CB_4374_BAF5_59514D9DDAF8__INCLUDED_)
#define AFX_02TCPSERVERDLG_H__76C8F7EF_70CB_4374_BAF5_59514D9DDAF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy02TcpServerDlg dialog

class CMy02TcpServerDlg : public CDialog
{
// Construction
public:
	CMy02TcpServerDlg(CWnd* pParent = NULL);	// standard constructor

	CSocket m_Listen;
	CSocket m_Client;

// Dialog Data
	//{{AFX_DATA(CMy02TcpServerDlg)
	enum { IDD = IDD_MY02TCPSERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy02TcpServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy02TcpServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnListen();
	afx_msg void OnAccept();
	afx_msg void OnSend();
	afx_msg void OnRecv();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_02TCPSERVERDLG_H__76C8F7EF_70CB_4374_BAF5_59514D9DDAF8__INCLUDED_)

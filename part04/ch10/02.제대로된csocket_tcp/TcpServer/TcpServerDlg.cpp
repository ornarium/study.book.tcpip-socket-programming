// TcpServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TcpServer.h"
#include "TcpServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTcpServerDlg dialog

CTcpServerDlg::CTcpServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTcpServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTcpServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTcpServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTcpServerDlg, CDialog)
	//{{AFX_MSG_MAP(CTcpServerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LISTEN, OnListen)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcpServerDlg message handlers

BOOL CTcpServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTcpServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTcpServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTcpServerDlg::OnListen() 
{
	// TODO: Add your control notification handler code here
	if(!m_Listen.Create(6565)) {
		AfxMessageBox(_T("Listen Socket Create Fail"));
		return ;
	}

	if(!m_Listen.Listen()) {
		AfxMessageBox(_T("¸®½¼ ½ÇÆÐ"));
		return ;
	}
}

void CTcpServerDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	m_Listen.m_Client.Send("world", 6);
}

void CTcpServerDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	m_Listen.m_Client.Close();
}

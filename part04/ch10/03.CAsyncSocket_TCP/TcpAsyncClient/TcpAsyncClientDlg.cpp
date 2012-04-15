// TcpAsyncClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TcpAsyncClient.h"
#include "TcpAsyncClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncClientDlg dialog

CTcpAsyncClientDlg::CTcpAsyncClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpAsyncClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTcpAsyncClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTcpAsyncClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTcpAsyncClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTcpAsyncClientDlg, CDialog)
	//{{AFX_MSG_MAP(CTcpAsyncClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcpAsyncClientDlg message handlers

BOOL CTcpAsyncClientDlg::OnInitDialog()
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

void CTcpAsyncClientDlg::OnPaint() 
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
HCURSOR CTcpAsyncClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTcpAsyncClientDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here
	if(!m_Client.Create()) {
		AfxMessageBox("家南 积己 角菩");
		return ;
	}

	if(m_Client.Connect("localhost", 6565) == 0) {
		if(m_Client.GetLastError() != WSAEWOULDBLOCK) {
			AfxMessageBox("家南 积己 角菩");
			return ;
		}
	}
}

void CTcpAsyncClientDlg::OnSend() 
{
	char buf[1024];
	ZeroMemory(buf, 1024);
	strcpy(buf, "[client] CMyAsyncClient: AsyncSending...");
	m_Client.AsyncSend(buf, strlen(buf) + 1);
}

void CTcpAsyncClientDlg::OnClose() 
{
	m_Client.ShutDown();	
}

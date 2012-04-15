// 05_TcpEventClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "05_TcpEventClient.h"
#include "05_TcpEventClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy05_TcpEventClientDlg dialog

CMy05_TcpEventClientDlg::CMy05_TcpEventClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy05_TcpEventClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy05_TcpEventClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy05_TcpEventClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy05_TcpEventClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy05_TcpEventClientDlg, CDialog)
	//{{AFX_MSG_MAP(CMy05_TcpEventClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_RECEIVE, OnReceive)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy05_TcpEventClientDlg message handlers

BOOL CMy05_TcpEventClientDlg::OnInitDialog()
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

void CMy05_TcpEventClientDlg::OnPaint() 
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
HCURSOR CMy05_TcpEventClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy05_TcpEventClientDlg::OnConnect() 
{
	if(!m_Client.Create) {
		AfxMessageBox("소켓 생성 실패!");
		return;
	}

	if(m_Client.Connect("localhost", 6565) == 0) {
		if(m_Client.GetLastError() != WSAEWOULDBLOCK) {
			AfxMessageBox("소켓 접속 실패");
			return;
		}
	}
}

void CMy05_TcpEventClientDlg::OnSend() 
{
	m_Client.Send("hello", 5);	
}

void CMy05_TcpEventClientDlg::OnReceive() 
{
	m_Client.ShutDown();
	//완전히 닫히는것을 확인하기 위해서 Close함수를 또 호출할 수 있다
	m_Client.Close();
}

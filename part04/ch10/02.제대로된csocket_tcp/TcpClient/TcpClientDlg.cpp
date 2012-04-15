// TcpClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TcpClient.h"
#include "TcpClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTcpClientDlg dialog

CTcpClientDlg::CTcpClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTcpClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTcpClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTcpClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTcpClientDlg, CDialog)
	//{{AFX_MSG_MAP(CTcpClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_GET_SOURCE, OnGetSource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTcpClientDlg message handlers

BOOL CTcpClientDlg::OnInitDialog()
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

void CTcpClientDlg::OnPaint() 
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
HCURSOR CTcpClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTcpClientDlg::OnConnect() 
{
	// TODO: Add your control notification handler code here
	if(m_Socket.Create() == FALSE) {
		AfxMessageBox("소켓 생성 실패");
		return;
	}

	if(m_Socket.Connect("localhost", 6565) == FALSE)
		AfxMessageBox("접속 실패!");
	else
		AfxMessageBox("접속 성공!");
}

void CTcpClientDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	m_Socket.Send("hello", 6);
}

void CTcpClientDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	m_Socket.Close();	
}

void CTcpClientDlg::OnGetSource() 
{
	 CSocket s;
	 
	 if(!s.Create()) {
		 AfxMessageBox("소켓 생성 실패");
		 return;
	 }

	 if(!s.Connect("www.google.com", 80)) {
		 AfxMessageBox("접속 실패!");
		 return ;
	 }

	 //GET 명령으로 데이터를 얻어옴
	 //GET 명령 형식 : GET 대상URL HTTP/1.0 <enter><enter>
	 char * cmd = "GET / HTTP/1.0\r\n\r\n";

	 s.Send(cmd, strlen(cmd));

	 char buf[100];
	 ZeroMemory(buf, 100);
	 while(s.Receive(buf, 1000)) {
		 AfxMessageBox(buf);
		 ZeroMemory(buf, 1000);
	 }
}

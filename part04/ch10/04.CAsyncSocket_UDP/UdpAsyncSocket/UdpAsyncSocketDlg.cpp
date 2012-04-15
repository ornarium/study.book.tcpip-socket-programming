// UdpAsyncSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UdpAsyncSocket.h"
#include "UdpAsyncSocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUdpAsyncSocketDlg dialog

CUdpAsyncSocketDlg::CUdpAsyncSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUdpAsyncSocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUdpAsyncSocketDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUdpAsyncSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUdpAsyncSocketDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUdpAsyncSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CUdpAsyncSocketDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUdpAsyncSocketDlg message handlers

BOOL CUdpAsyncSocketDlg::OnInitDialog()
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

void CUdpAsyncSocketDlg::OnPaint() 
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
HCURSOR CUdpAsyncSocketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUdpAsyncSocketDlg::OnCreate() 
{
	CString sPort;
	GetDlgItemText(IDC_PORT, sPort);

	if(m_Udp.Create(_ttoi(sPort), SOCK_DGRAM))
		AfxMessageBox(_T("积己 己傍!"));
	else
		AfxMessageBox(_T("积己 角菩!"));
}

void CUdpAsyncSocketDlg::OnSend() 
{
	CString sRemoteIP, sRemotePort;
	GetDlgItemText(IDC_REMOTE_IP, sRemoteIP);
	GetDlgItemText(IDC_REMOTE_PORT, sRemotePort);

	m_Udp.SendTo("hello udp", 10, _ttoi(sRemotePort), sRemoteIP);
}

void CUdpAsyncSocketDlg::OnClose() 
{
	m_Udp.Close();	
}

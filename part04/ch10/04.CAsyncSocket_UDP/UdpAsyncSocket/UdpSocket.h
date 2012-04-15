#if !defined(AFX_UDPSOCKET_H__82AFC6C9_FDC2_4B39_B691_8EBA98C5C8B4__INCLUDED_)
#define AFX_UDPSOCKET_H__82AFC6C9_FDC2_4B39_B691_8EBA98C5C8B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UdpSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CUdpSocket command target

class CUdpSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CUdpSocket();
	virtual ~CUdpSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdpSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUdpSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPSOCKET_H__82AFC6C9_FDC2_4B39_B691_8EBA98C5C8B4__INCLUDED_)

; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTcpAsyncClientDlg
LastTemplate=CAsyncSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TcpAsyncClient.h"

ClassCount=3
Class1=CTcpAsyncClientApp
Class2=CTcpAsyncClientDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CMyAsyncSocket
Resource2=IDD_TCPASYNCCLIENT_DIALOG

[CLS:CTcpAsyncClientApp]
Type=0
HeaderFile=TcpAsyncClient.h
ImplementationFile=TcpAsyncClient.cpp
Filter=N

[CLS:CTcpAsyncClientDlg]
Type=0
HeaderFile=TcpAsyncClientDlg.h
ImplementationFile=TcpAsyncClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTcpAsyncClientDlg



[DLG:IDD_TCPASYNCCLIENT_DIALOG]
Type=1
Class=CTcpAsyncClientDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CONNECT,button,1342242816
Control5=IDC_SEND,button,1342242816
Control6=IDC_CLOSE,button,1342242816

[CLS:CMyAsyncSocket]
Type=0
HeaderFile=MyAsyncSocket.h
ImplementationFile=MyAsyncSocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q
LastObject=CMyAsyncSocket


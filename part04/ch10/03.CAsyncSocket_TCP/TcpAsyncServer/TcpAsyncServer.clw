; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTcpAsyncServerDlg
LastTemplate=CAsyncSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TcpAsyncServer.h"

ClassCount=3
Class1=CTcpAsyncServerApp
Class2=CTcpAsyncServerDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CListenSocket
Resource2=IDD_TCPASYNCSERVER_DIALOG

[CLS:CTcpAsyncServerApp]
Type=0
HeaderFile=TcpAsyncServer.h
ImplementationFile=TcpAsyncServer.cpp
Filter=N

[CLS:CTcpAsyncServerDlg]
Type=0
HeaderFile=TcpAsyncServerDlg.h
ImplementationFile=TcpAsyncServerDlg.cpp
Filter=D
LastObject=IDC_CLOSE
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_TCPASYNCSERVER_DIALOG]
Type=1
Class=CTcpAsyncServerDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LISTEN,button,1342242816
Control5=IDC_SEND,button,1342242816
Control6=IDC_CLOSE,button,1342242816

[CLS:CListenSocket]
Type=0
HeaderFile=ListenSocket.h
ImplementationFile=ListenSocket.cpp
BaseClass=CAsyncSocket
Filter=N
LastObject=CListenSocket
VirtualFilter=q


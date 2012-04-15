; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTcpServerDlg
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TcpServer.h"

ClassCount=4
Class1=CTcpServerApp
Class2=CTcpServerDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CListenSocket
Class4=CClientSocket
Resource2=IDD_TCPSERVER_DIALOG

[CLS:CTcpServerApp]
Type=0
HeaderFile=TcpServer.h
ImplementationFile=TcpServer.cpp
Filter=N

[CLS:CTcpServerDlg]
Type=0
HeaderFile=TcpServerDlg.h
ImplementationFile=TcpServerDlg.cpp
Filter=D
LastObject=CTcpServerDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_TCPSERVER_DIALOG]
Type=1
Class=CTcpServerDlg
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
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CListenSocket

[CLS:CClientSocket]
Type=0
HeaderFile=ClientSocket.h
ImplementationFile=ClientSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CClientSocket


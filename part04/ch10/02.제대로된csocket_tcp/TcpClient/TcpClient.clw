; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClientSocket
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TcpClient.h"

ClassCount=3
Class1=CTcpClientApp
Class2=CTcpClientDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CClientSocket
Resource2=IDD_TCPCLIENT_DIALOG

[CLS:CTcpClientApp]
Type=0
HeaderFile=TcpClient.h
ImplementationFile=TcpClient.cpp
Filter=N

[CLS:CTcpClientDlg]
Type=0
HeaderFile=TcpClientDlg.h
ImplementationFile=TcpClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_TCPCLIENT_DIALOG]
Type=1
Class=CTcpClientDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CONNECT,button,1342242816
Control5=IDC_SEND,button,1342242816
Control6=IDC_CLOSE,button,1342242816
Control7=IDC_GET_SOURCE,button,1342242816

[CLS:CClientSocket]
Type=0
HeaderFile=ClientSocket.h
ImplementationFile=ClientSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CClientSocket


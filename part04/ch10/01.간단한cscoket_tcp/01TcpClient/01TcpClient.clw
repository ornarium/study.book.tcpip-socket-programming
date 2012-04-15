; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMy01TcpClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "01TcpClient.h"

ClassCount=3
Class1=CMy01TcpClientApp
Class2=CMy01TcpClientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_MY01TCPCLIENT_DIALOG

[CLS:CMy01TcpClientApp]
Type=0
HeaderFile=01TcpClient.h
ImplementationFile=01TcpClient.cpp
Filter=N

[CLS:CMy01TcpClientDlg]
Type=0
HeaderFile=01TcpClientDlg.h
ImplementationFile=01TcpClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CONNECT

[CLS:CAboutDlg]
Type=0
HeaderFile=01TcpClientDlg.h
ImplementationFile=01TcpClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MY01TCPCLIENT_DIALOG]
Type=1
Class=CMy01TcpClientDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CONNECT,button,1342242816
Control5=IDC_SEND,button,1342242816
Control6=IDC_RECV,button,1342242816
Control7=IDC_CLOSE,button,1342242816


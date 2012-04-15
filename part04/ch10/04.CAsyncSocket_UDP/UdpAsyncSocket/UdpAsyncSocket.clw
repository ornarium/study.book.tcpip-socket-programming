; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUdpSocket
LastTemplate=CAsyncSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "UdpAsyncSocket.h"

ClassCount=3
Class1=CUdpAsyncSocketApp
Class2=CUdpAsyncSocketDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CUdpSocket
Resource2=IDD_UDPASYNCSOCKET_DIALOG

[CLS:CUdpAsyncSocketApp]
Type=0
HeaderFile=UdpAsyncSocket.h
ImplementationFile=UdpAsyncSocket.cpp
Filter=N

[CLS:CUdpAsyncSocketDlg]
Type=0
HeaderFile=UdpAsyncSocketDlg.h
ImplementationFile=UdpAsyncSocketDlg.cpp
Filter=D
LastObject=CUdpAsyncSocketDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_UDPASYNCSOCKET_DIALOG]
Type=1
Class=CUdpAsyncSocketDlg
ControlCount=11
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_REMOTE_IP,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_REMOTE_PORT,edit,1350631552
Control8=IDC_PORT,edit,1350631552
Control9=IDC_CLOSE,button,1342242816
Control10=IDC_CREATE,button,1342242816
Control11=IDC_SEND,button,1342242816

[CLS:CUdpSocket]
Type=0
HeaderFile=UdpSocket.h
ImplementationFile=UdpSocket.cpp
BaseClass=CAsyncSocket
Filter=N
VirtualFilter=q
LastObject=CUdpSocket


; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyWinThread
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WinThread.h"

ClassCount=5
Class1=CWinThreadApp
Class2=CWinThreadDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class5=CMyWinThread
Resource2=IDD_WINTHREAD_DIALOG

[CLS:CWinThreadApp]
Type=0
HeaderFile=WinThread.h
ImplementationFile=WinThread.cpp
Filter=N

[CLS:CWinThreadDlg]
Type=0
HeaderFile=WinThreadDlg.h
ImplementationFile=WinThreadDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_WINTHREAD_DIALOG]
Type=1
Class=CWinThreadDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CREATE_MYWINTHREAD,button,1342242816
Control4=IDC_TM_BLOCKING_CALL,button,1342242816
Control5=IDC_BLOCKING_CALL,button,1342242816

[CLS:CMyWinThread]
Type=0
HeaderFile=MyWinThread.h
ImplementationFile=MyWinThread.cpp
BaseClass=CWinThread
Filter=N


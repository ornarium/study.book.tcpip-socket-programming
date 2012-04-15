������ ������ ���α׷���

������� ���������� ó���ϴ� ������ �ۼ��Ҷ� ���μ����� �����ϴ� ���н��� �������� fork�Լ��� ������� �ʴ´�. �����쿡���� ���� ������ �����带 ����Ͽ� ���������� ó��. 

���� ���α׷��� �ۼ��Ҷ� ������/���н� �׸��� �����츦 ��� ���� �ҽ����� �ۼ��Ϸ��� ��Ŭ�� ���� �Լ� ������� �ۼ��ؾ� ��. ���� ����� �Լ� �Լ��� cǥ�� ����� ��Ÿ����(fopen, fprintf)���� ���.

������� ���н�/�������� ������ �ٸ��⶧�������н�/������ ���α׷��� ������� �����ϴ� ����� MSDN�� UNIX Application Migration Guide���� �ڼ��� �ٷ�� �ִ�. 

 * ������ ����ϱ�
DWORD�� unsigned long�� ���� 4����Ʈ �ǹ�. WORD�� unsigned short�� ���� 2����Ʈ.
LP�� Long Pointer�� �Ӹ������̸�, Long�� 16��Ʈ �ڵ� ȣȯ�� ���� ���� �Ǿ���(16��Ʈ ������ �깰)

HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes, //SecurityDescriptor
  SIZE_T dwStackSize,  //�ʱ� ���� ũ��
  LPTHREAD_START_ROUTINE lpStartAddress, //������ �Լ�
  LPVOID lpParameter, //������ ����
  DWORD dwCreationFlags, //���� �ɼ�
  LPDWORD lpThreadId //������ ���̵�
);

- lpThreadAttributes: ������ �����忡 ���� ������ ����. ���� NULL�̸� THREAD_ALL_ACCESS�� �ǹ�. � �Լ��� �� �������� �ڵ��� �̿��ؼ� �����带 ������ �� ����
- dwStackSize: �����尡 �Լ��� �����Ҷ� �߻��Ǵ� ���� ����, �Լ� ȣ�� ���� �ּҵ��� ����ϴµ� ���. ������ ����� ���μ��� �޸𸮿��� ����. 0�� �ָ� �⺻���� 1MB�� ���� ũ�� ����
- lpStartAddress: 
DWORD WINAPI ThreadFunc(LPVOID lpParam) ������ ������ �Լ�
- lpParameter: ������ ���� �ɼ� �ش�. ���ڿ� 0�� �ָ� ������ ������ �ٷ� ����. CREATE_SUSPENDED�� �ָ� Suspend ���·� ����. �����尡 Suspend �����̸� ResumeThread�� Ready���·� �ٲܼ� �ִ�. Ready���°� �Ǹ� ������ �����췯�� ���ؼ� ������� ������ �߿��� �켱������ ���� �����尡 ���õ��� ������°� ��.
- lpThreadId: ������ ������ ���̵� ���� ����. 2000�̻󿡼��� NULL�� �ָ� ������ ���̵� ���� �ʰڴٴ� �ǹ�����, 95/98/ME������ NULL�� �ָ� ������ �����.

 * �����췯
���μ����� �켱������ �켱���� Ŭ������� �θ���, �������� �켱������ �켱���� �����̶�� �θ�. �ΰ����� �켱������ �����Ͽ� ���� �����층�Ҷ� �ʿ��� �������� �������� �켱������ ���ϸ�, �̸� ���̽� �켱����(Base Priority)��� �θ���. �ڼ��� ������ MSDN->Process and Threads->Scheduling->Scheduling Priorities ������ ����
<���μ��� �켱���� ��>
- ABOVE_NORMAL_PRIORITY_CLASS: ������ 2000/XP������ ����� �� �ִ�. NORMAL_PRIORITY_CLASS���ٴ� �켱������ ������ HIGH_PRIORITY_CLASS���ٴ� �켱������ ����.
- BELOW_NORMAL_PRIORITY_CLASS: IDLE_PRIORITY_CLASS���ٴ� �켱 ������ ������ NORMAL_PRIORITY_CLASS���ٴ� �켱������ ����
- HIGH_PRIORITY_CLASS �ð��� �ΰ��ؼ� ������ ����Ǿ�� �ϴ� ���μ����� ���� �켱����.NORMAL�̳� IDLE���� HIGH�� ������ �켱���� �ִ�.
- IDLE_PRIORITY_CLASS �ý����� idle�ɶ��� ������ ����(ex. ȭ�麸ȣ��)
- NORMAL_PRIORITY_CLASS �Ϲ����� �켱����
- REALTIME_PRIORITY_CLASS: ������� �켱����. ��� �����庸�� �켱���� �������� ���´�. ��ĳ ĳ�ÿ� ������ ����ų� ���콺�� ������ ���� �� �־�, ���� ª�� �Ⱓ���� �����.


<������ �켱���� ��>
SetThreadPriority�� GetThreadPriority�� Thread�� ���õ�.
THREAD_PRIORITY_IDLE < THREAD_PRIORITY_LOWEST < THREAD_PRIORITY_BELOW_NORMAL < THREAD_PRIORITY_NORMAL < THREAD_PRIORITY_ABOVE_NORMAL < THREAD_PRIORITY_HIGHEST < THREAD_PRIORITY_TIME_CRITICAL

* ������ ����
ThreadID�� ��´�
HANDLE GetCurrentThread(VOID);

SuspendThread/Resume���� ���� ����.

* ������ ����
������ �Լ������� return�ϰų� ExitThread API�� ����ȣ���Ѵ�.

VOID ExitThread(DWORD dwExitCode);
�����带 �����ϸ� Ŀ�ο��� ������ ��ü�� �����ϰ� ������� �ڵ�� STILL_ACTIVE�̴�.
�����尡 ����Ǹ� ������ ��ü�� �ñ׳�(����)�� ���°� �ǰ� ������´� dwExitCode�� �ٲ�� �ȴ�.

* ������ ��ٸ���
�ڽ� �����尡 ����ɶ����� �θ𽺷��尡 ��ٸ����� WaitForSingleObject �Լ��� ����Ѵ�. WaitForSingleObject�Լ��� �ش� �ڵ� ��ü�� �ñ׳� ���°� �Ǹ� ���ϵǴµ�, �����嵵 ����� �ñ׳� ���°� �Ǳ� �����̴�. �����忡 ���� ��� �����ٸ� CloseHandle�� �ڵ鰴ü�� �����ؾ��Ѵ�.

hThread = CreateThread(...);
if(hThread == NULL) {
  AfxMessageBox("error");
} else {
  //�����尡 �ñ׳�(����)�� ������ ���� ��ٸ�.
  ::WaitForSingleObject(hThread, INFINITE);
  CloseHandle(hThread);
}

* _beginthread�� _beginthreadex, _endthread
CreateThread�� Win32 API�ε� ���� _beginthread�� _beginthreadex C��Ÿ�� ���̺귯���� ���ؼ� ���ȴ�(���������� CreateThread�� ���). C��Ÿ�� �Լ����� ��� �Ѵٸ� _beginthread�� _beginthreadex�� ��� �Ѵ�.

_beginthread �Լ��� ������ ���� �ɼ��̳� Thread ID, Security Descriptor�� �������� �ʴ´�. �׷� ���� ������ �����Ͽ� C��Ÿ�� �����带 �����Ϸ��� _beginthreadex�Լ��� ����Ѵ�. ����ÿ� _endthread �Ǵ� _endthreadex�� ����Ѵ�.

Platform SDK�� ���� C��Ÿ�� �Լ����� ��� �����Ǿ� �ִ��� Ȯ���� �� �ִ�.


section03. MFC CWinThread ����ϱ�

MFC�� �����带�����ϱ� ���ؼ� AfxBeginThread�Լ��� ����.
1. �۾��� ������(Worker Thread)  ���
CWinThread * AfxBeginThread(AFX_THREADPROC pfnTHreadProc, LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttr = NULL);
- AFX_THREADPROC�� �Լ� ����
UINT MyControllingFunction(LPVOID pParam);

��뿹
UINT _MyFunction(LPVOID pParam) {...}
void CTestThreadDlg::OnAfxbeginthreadFunction() {
  AfxBeginThread(_MyFunction, 0);
}

2. UI ������ ���
CWinThread * AfxBeginThread(CRuntimeClass * pThreadClass, LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttr = NULL);
- MFC�� ��Ÿ�� Ŭ���� ���� ��������� ����� ������ ���� ���
- UI������� CWinThread�� ��ӹ޾� ó���ؾ� ��.
- ������ �ȿ��� ����� �Է��� �޾� �޽���ó���� ����

UI������� Run�Լ� �ȿ��� �޽��� ������ while�� ȣ���Ѵ�. GetMessage, TranslateMessage, DIspatchMessageȣ���. ������ UI�������°� ���� �ִ°��� �ƴϱ� ������, �󸶵��� CreateThread�� �Ἥ UI������� ����� �� �ִ�.

DWORD WINAPI WOrkerThread(LPVOID lpParam) {
  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0)) {
    switch(msg.message) {
      case TM_BLOCKING_CALL:
        ::MessageBox(NULL, "Blocking...", "�۾��� ������", MB_OK);
        break;
    }

    //GUI�� ���� �޽��� ����
    TranslateMessage(&msg);
    DispatchMessage(msg);
    return 0;
}

HANDLE g_hThread;
DWORD g_dwTHreadID;
void CTestThreadDlg::OnCreateWorker() {
    //TODO: Add your control notification handler code here
    g_hThread = CreateThread(NULL, 0, WorkerThread, this, 0, &g_dwThreadID);

    if(g_hThread == NULL)
      AfxMessageBox("������ ���� ����");
}

void CTestThreadDlg::OnPostWorker() {
    //TODO: Add your control notification handler code here
    ::PostThreadMessage(g_dwThreadID, TM_BLOCKING_CALL, 0, 0);
}

 * ��Ƽ �����忡�� MFC��ü�� ����Ҷ� ������
MFC��ü�� �ϳ��� �����忡���� ����ؾ� �Ѵ�. ���ڷ� �Ѱ��ְų�, �ٸ������忡�� MFC��ü�� �Լ��� ȣ���ϸ� �������� ������ �߻��� �� �ִ�. MFC�� ������� �����帶�� CWnd�� ���� Ŭ�������� �ڵ���� ���� �ִ� MFC����ü Ŭ����(AFX_MODULE_THREAD_STATE)�� ���� �ִ�.
CHandleMap::LookupPermanent �Ǵ� CHandleMap::LookupTemporary���� �Լ��� MFC��ü�� ���� �������� �ڵ�ʿ� �����ϴ��� ã�ƺ��� �Լ�. ȣ��� �������� �ڵ�ʿ� ������ CWnd::AssertValid �Լ����� �����߻�

MSDN: CWnd Derived MFC Objects and Multi-threaded Applications ��� ����.

Section06. �����尣 ����ȭ
1. �Ӱ迵��
CRITICAL_SECTION cs;
InitializeCriticalSection(&cs);
EnterCriticalSection(&cs);
...//������ �ϳ��� ����
LeaveCriticalSection(&cs);
DeleteCriticalSection(&cs);

MFC�� CCriticalSection Ŭ������ ������ ������ Ŭ���� ������ �� �� �ִ�.
#include <afxmt.h>
CCriticalSection cs;

cs.Lock();
...//������ �ϳ��� ����
cc.Unlock();

2. �̺�Ʈ ��ü �̿�
�̺�Ʈ ��ü�� (�ñ׳�/��ñ׳�) �ΰ��� ���¸� ����. WaitForSingleObject�Լ��� �̺�Ʈ ��ü�� �ñ׳� �ɶ����� ��ٸ�.

�̺�Ʈ ��ü�� CreateEvent�Լ��� ����. ����� �ݵ�� CloseHandle�� ��ü ����.
HANDLE CreateEvent(
LPSECURITY_ATTRIBUTES lpEventAttributes, //Security Descriptor
BOOL bManualReset,
BOOL bInitialState,
LPCTSTR lpName
);
- lpEventAttributes: �� ��ü�� �����Ҽ� �ִ� ���ȼ���. ���� NULL���. SecurityDescriptor�� �����Ѵٴ°��� Ŀ�� ��ü����  �ǹ�. Ŀ�� ��ü�� ������ Ŀ�ο��� ����.
- bManualReset: TRUE�̸� �������� ���� ResetEvent�� ȣ���Ͽ� ��ñ׳λ��¸� ������ ��. FALSE�̸� �ñ׳� ���°� �Ǹ� �ڵ����� ��ñ׳� ���·� �����(WaitForSingleObject�� ������̴� ������� �ñ׳��� �Ǿ� ������ �簳�ǰ�, �ٽ� ��ñ׳� ���°� �ȴٴ� �ǹ�)
- bInitialState: �ʱ� �ñ׳� ������.
- lpName: �̺�Ʈ Ŀ�� ��ü���� �̸��� �ο�. NULL�̸� �̸����� ��ü. ���� NULL ���. Ŀ�� ��ü�� Ŀ�ο��� �����ǹǷ�, �̸� �ִ� Ŀ�� ��ü�� �ٸ� ���μ����� ������� ����ȭ�ϴµ� ��밡��.

HANDLE g_hEvent;
DWORD WINAPI _EventTest(LPVOID lpObj) {
  HANDLE hEvent = (HANDLE)lpObj;
  DWORD dwRet;
  while(1) {
    //g_hEvent�� �ñ׳� ���°� �� ������ ���� ���
    dwRet = ::WaitForSingleObject(g_hEvent, INFINITE);
    //setEvent�� �ñ׳� �Ǹ� Wait�� ��������
    //Auto Reset�̹Ƿ� �ٷ� ��ñ׳� ����
    ::MessageBox(NULL, "�ñ׳� �Ǿ���", "Event", MB_OK);
  }

  return 0;
}

void CTestThreadDlg::OnEvent() {
   g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
   DWORD dwTid;
   HANDLE hThread;
   hThread = CreateThread(NULL, 0, _EventTest, (LPVOID)g_hEvent, 0, &dwTid);
}

void CTestThreadDlg::OnSetEvent() {
  SetEvent(g_hEvent);
}

3. �������� �ñ׳� ��ٸ���
DWORD WaitForMultipleObjects(
   DWORD nCount,
   CONST HANDLE * lpHandles,
   BOOL bWaitAll,
   DWORD dwMilliseconds
);
- nCount: �˻��� ��ü�� ����
- lpHandles: ��ü�� �ڵ� �迭
- bWaitAll: TRUE�̸� ��� ��ü�� �ñ׳� �ɶ����� ��ٸ���, FALSE�̸� �ϳ��� ��ü�� �ٲ�� Wait���� ���. FALSE�϶�, ���� ������ �ñ׳� �� ��ü�� �迭 ��° ���� ����(�� ��° �ñ׳��̶��, WAIT_OBJECT_0 + 1)
- dwMilliseconds: Ÿ�Ӿƿ��� �־� ������ ��ٸ��� �ʵ��� ��. INFINITE�� ������ ��ٸ�.

HANDLE g_hEvent1, g_hEvent2;
DWORD WINAPI _EventTest(LPVOID lpObj) {
  HANDLE hEvents[2];
  hEvents[0] = g_hEvent1;
  hEvents[1] = g_hEvent2;

  DWORD dwRet;
  while(1) {
    dwRet = ::WaitForMultipleObject(2, hEvents, FALSE, INFINITE);
    switch(dwRet) {
    case WAIT_OBJECT_0 + 0:
      ::MessageBox(NULL, "1st �ñ׳� �Ǿ���", "Event", MB_OK);
      break;
    case WAIT_OBJECT_0 + 0:
      ::MessageBox(NULL, "2st �ñ׳� �Ǿ���", "Event", MB_OK);
      break;
    case WAIT_TIMEOUT:
      ::MessageBox(NULL, "timeout", "Event", MB_OK);
      break;
    }
  }

  return 0;
}

void CTestThreadDlg::OnEvents() {
   g_hEvent1 = CreateEvent(NULL, FALSE, FALSE, NULL);
   g_hEvent2 = CreateEvent(NULL, FALSE, FALSE, NULL);

   DWORD dwTid;
   HANDLE hThread;
   hThread = CreateThread(NULL, 0, _EventTest, NULL, 0, &dwTid);
}

void CTestThreadDlg::OnSetEvent1() {
  SetEvent(g_hEvent1);
}

void CTestThreadDlg::OnSetEvent2() {
  SetEvent(g_hEvent2);
}

07. ��Ƽ �����忡�� MFC Socket����ϱ�
CAsyncSocket�̳� CSocket���� ���� Ŭ������ ��� �ٸ� ������� ���� ��ü�� �ѱ涧�� (MS���� �ǰ����� ������), ���� Ŭ������ Detach�Լ��� ��� ���� ���� �ڵ��� ��� �ٸ� �������� �ѱ��, �ٸ� �����忡�� ���� �ڵ��� Attach��Ű�� ���(MSDN : MultiSoc:Illustrates Using Sockets in Multiple Thread ���� �ҽ� ����)

void CListenSoc::OnAccept(int nErrorCode) {
CSocket soc;
//�ӽü������� �ϴ� ���Ʈ
Accept(soc);
//������ ���� ó���� ����� CConnectThread�� Suspend���·� ����(derived CWinThread)
CConnectThread * pTHread = (CConnectThread*) AfxBeginThread(
RUNTIME_CLASS(CConnectThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
if(!pThread) {
  soc.Close();
  TRACE("���� ����!\n");
  return ;
}

//������ �ʱ�ȭ �۾�
CServerDlg * pDlg = (CServerDlg *) m_pWndDlg;
//set members of CConnectThread.m_socket
pThread->m_socket.m_pCriticalSection = &pDlg->m_criticalSection;
pThread->m_socket.m_pLastString = &pDlg->m_pLastString;
//������ �ڵ��� Detach�ؼ� �Ѱ���
pThread->m_hSocket = soc.Detach();
//������ ������ InitInstance�Լ� ȣ��
pTHread->ResumeThread();

CAsyncSocket::OnAccept(nErrorCode);
}

BOOL CConnectThread::InitInstance() {
//���� ������� �����尡 �����Ǿ����� ����.
//WM_THERADSTART�� define�ؼ� ���� �޽���.
::PostMessage(AfxGetApp->m_pMainWnd->m_hWnd, WM_THREADSTART, 0, 0);

//���� �ڵ�� Attach
m_socket.Attach(m_hSocket);
m_socket.m_pThread = this;
return TRUE;
}
}

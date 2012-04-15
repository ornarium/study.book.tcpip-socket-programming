윈도우 스레드 프로그래밍

윈도우는 다중접속을 처리하는 서버를 작성할때 프로세스를 생성하는 유닉스나 리눅스의 fork함수를 사용하지 않는다. 윈도우에서는 좀더 가벼운 스레드를 사용하여 다중접속을 처리. 

소켓 프로그램을 작성할때 리눅스/유닉스 그리고 윈도우를 모두 같은 소스에서 작성하려면 버클리 소켓 함수 방식으로 작성해야 함. 파일 입출력 함수 함수도 c표중 입출력 스타일인(fopen, fprintf)등을 사용.

윈도우는 유닉스/리눅스와 구조가 다르기때문에유닉스/리눅스 프로그램을 윈도우로 포팅하능 방법을 MSDN의 UNIX Application Migration Guide에서 자세히 다루고 있다. 

 * 스레드 사용하기
DWORD는 unsigned long와 같은 4바이트 의미. WORD는 unsigned short와 같은 2바이트.
LP는 Long Pointer의 머릿글자이며, Long은 16비트 코드 호환을 위해 남게 되었음(16비트 시절의 산물)

HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes, //SecurityDescriptor
  SIZE_T dwStackSize,  //초기 스택 크기
  LPTHREAD_START_ROUTINE lpStartAddress, //스레드 함수
  LPVOID lpParameter, //스레드 인자
  DWORD dwCreationFlags, //생성 옵션
  LPDWORD lpThreadId //스레드 아이디
);

- lpThreadAttributes: 생성될 스레드에 대한 사용권한 지정. 보통 NULL이며 THREAD_ALL_ACCESS를 의미. 어떤 함수든 이 스레드의 핸들을 이용해서 스레드를 제어할 수 있음
- dwStackSize: 스레드가 함수를 수행할때 발생되는 지역 변수, 함수 호출 복귀 주소등을 기억하는데 사용. 스레드 종료시 프로세스 메모리에서 해제. 0을 주면 기본으로 1MB로 스택 크기 생성
- lpStartAddress: 
DWORD WINAPI ThreadFunc(LPVOID lpParam) 형태의 스레드 함수
- lpParameter: 스레드 생성 옵션 해당. 인자에 0을 주면 스레드 생성후 바로 실행. CREATE_SUSPENDED를 주면 Suspend 상태로 생성. 스레드가 Suspend 상태이면 ResumeThread로 Ready상태로 바꿀수 있다. Ready상태가 되면 윈도우 스케쥴러에 의해서 대기중인 스레드 중에서 우선순위에 따라 스레드가 선택된후 실행상태가 됨.
- lpThreadId: 생성된 스레드 아이디 값을 받음. 2000이상에서는 NULL을 주면 스레드 아이디를 받지 않겠다는 의미지만, 95/98/ME에서는 NULL을 주면 문제가 생긴다.

 * 스케쥴러
프로세스의 우선순위는 우선순위 클래스라고 부르고, 스레드의 우선순위는 우선순위 레벨이라고 부름. 두가지의 우선순위를 조합하여 실제 스케쥴링할때 필요한 스레드의 최종적인 우선순위를 정하며, 이를 베이스 우선순위(Base Priority)라고 부른다. 자세한 내용은 MSDN->Process and Threads->Scheduling->Scheduling Priorities 페이지 참조
<프로세스 우선순위 값>
- ABOVE_NORMAL_PRIORITY_CLASS: 윈도우 2000/XP에서만 사용할 수 있다. NORMAL_PRIORITY_CLASS보다는 우선순위가 높지만 HIGH_PRIORITY_CLASS보다는 우선순위가 낮다.
- BELOW_NORMAL_PRIORITY_CLASS: IDLE_PRIORITY_CLASS보다는 우선 순위가 높지만 NORMAL_PRIORITY_CLASS보다는 우선순위가 낮음
- HIGH_PRIORITY_CLASS 시간에 민감해서 빠르게 실행되어야 하는 프로세스를 위한 우선순위.NORMAL이나 IDLE보다 HIGH에 스케쥴 우선권이 있다.
- IDLE_PRIORITY_CLASS 시스템이 idle될때만 스레드 동작(ex. 화면보호기)
- NORMAL_PRIORITY_CLASS 일반적인 우선순위
- REALTIME_PRIORITY_CLASS: 가장높은 우선순위. 모든 스레드보다 우선적인 선점권을 갖는다. 디스캐 캐시에 오류가 생기거나 마우스가 응답을 못할 수 있어, 아주 짧은 기간에만 써야함.


<스레드 우선순위 값>
SetThreadPriority와 GetThreadPriority가 Thread와 관련됨.
THREAD_PRIORITY_IDLE < THREAD_PRIORITY_LOWEST < THREAD_PRIORITY_BELOW_NORMAL < THREAD_PRIORITY_NORMAL < THREAD_PRIORITY_ABOVE_NORMAL < THREAD_PRIORITY_HIGHEST < THREAD_PRIORITY_TIME_CRITICAL

* 스레드 제어
ThreadID를 얻는다
HANDLE GetCurrentThread(VOID);

SuspendThread/Resume으로 동작 제어.

* 스레드 종료
스레드 함수내에서 return하거나 ExitThread API를 직접호출한다.

VOID ExitThread(DWORD dwExitCode);
스레드를 생성하면 커널에서 스레드 객체를 관리하고 종료상태 코드는 STILL_ACTIVE이다.
스레드가 종료되면 스레드 객체는 시그널(종료)된 상태가 되고 종료상태는 dwExitCode로 바뀌게 된다.

* 스레드 기다리기
자식 스레드가 종료될때까지 부모스레드가 기다리려면 WaitForSingleObject 함수를 사용한다. WaitForSingleObject함수는 해당 핸들 객체가 시그널 상태가 되면 리턴되는데, 스레드도 종료시 시그널 상태가 되기 때문이다. 스레드에 대한 제어가 끝났다면 CloseHandle로 핸들객체를 해제해야한다.

hThread = CreateThread(...);
if(hThread == NULL) {
  AfxMessageBox("error");
} else {
  //스레드가 시그널(종료)될 때까지 무한 기다림.
  ::WaitForSingleObject(hThread, INFINITE);
  CloseHandle(hThread);
}

* _beginthread와 _beginthreadex, _endthread
CreateThread는 Win32 API인데 반해 _beginthread와 _beginthreadex C런타임 라이브러리를 위해서 사용된다(내부적으로 CreateThread를 사용). C런타임 함수들을 써야 한다면 _beginthread와 _beginthreadex를 써야 한다.

_beginthread 함수는 스레드 생성 옵션이나 Thread ID, Security Descriptor를 포함하지 않는다. 그런 세부 설정을 포함하여 C런타임 스레드를 구동하려면 _beginthreadex함수를 사용한다. 종료시엔 _endthread 또는 _endthreadex를 사용한다.

Platform SDK를 보면 C런타임 함수들이 어떻게 구현되어 있는지 확인할 수 있다.


section03. MFC CWinThread 사용하기

MFC는 스레드를구동하기 위해서 AfxBeginThread함수를 제공.
1. 작업자 스레드(Worker Thread)  방식
CWinThread * AfxBeginThread(AFX_THREADPROC pfnTHreadProc, LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttr = NULL);
- AFX_THREADPROC의 함수 원형
UINT MyControllingFunction(LPVOID pParam);

사용예
UINT _MyFunction(LPVOID pParam) {...}
void CTestThreadDlg::OnAfxbeginthreadFunction() {
  AfxBeginThread(_MyFunction, 0);
}

2. UI 스레드 방식
CWinThread * AfxBeginThread(CRuntimeClass * pThreadClass, LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttr = NULL);
- MFC의 런타임 클래스 동적 생성방식을 사용한 스레드 구동 방식
- UI스레드는 CWinThread를 상속받아 처리해야 함.
- 스레드 안에서 사용자 입력을 받아 메시지처리가 가능

UI스레드는 Run함수 안에서 메시지 펌핑을 while로 호출한다. GetMessage, TranslateMessage, DIspatchMessage호출순. 실제로 UI스레드라는게 따로 있는것이 아니기 때문에, 얼마든지 CreateThread를 써서 UI스레드로 사용할 수 있다.

DWORD WINAPI WOrkerThread(LPVOID lpParam) {
  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0)) {
    switch(msg.message) {
      case TM_BLOCKING_CALL:
        ::MessageBox(NULL, "Blocking...", "작업자 스레드", MB_OK);
        break;
    }

    //GUI를 위한 메시지 펌프
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
      AfxMessageBox("스레드 생성 실패");
}

void CTestThreadDlg::OnPostWorker() {
    //TODO: Add your control notification handler code here
    ::PostThreadMessage(g_dwThreadID, TM_BLOCKING_CALL, 0, 0);
}

 * 멀티 스레드에서 MFC객체를 사용할때 주의점
MFC객체는 하나의 스레드에서만 사용해야 한다. 인자로 넘겨주거나, 다른스레드에서 MFC객체의 함수를 호출하면 잠재적인 문제가 발생할 수 있다. MFC의 스레드는 스레드마다 CWnd나 소켓 클래스들의 핸들맵을 갖고 있는 MFC구조체 클래스(AFX_MODULE_THREAD_STATE)를 갖고 있다.
CHandleMap::LookupPermanent 또는 CHandleMap::LookupTemporary같은 함수는 MFC객체가 실제 스레드의 핸들맵에 존재하는지 찾아보는 함수. 호출된 스레드의 핸들맵에 없으면 CWnd::AssertValid 함수에서 오류발생

MSDN: CWnd Derived MFC Objects and Multi-threaded Applications 기사 참조.

Section06. 스레드간 동기화
1. 임계영역
CRITICAL_SECTION cs;
InitializeCriticalSection(&cs);
EnterCriticalSection(&cs);
...//스레드 하나만 가능
LeaveCriticalSection(&cs);
DeleteCriticalSection(&cs);

MFC는 CCriticalSection 클래스로 간단히 래핑한 클래스 버전을 쓸 수 있다.
#include <afxmt.h>
CCriticalSection cs;

cs.Lock();
...//스레드 하나만 가능
cc.Unlock();

2. 이벤트 객체 이용
이벤트 객체는 (시그널/비시그널) 두가지 상태만 존재. WaitForSingleObject함수로 이벤트 객체가 시그널 될때까지 기다림.

이벤트 객체는 CreateEvent함수로 생성. 종료시 반드시 CloseHandle로 객체 제거.
HANDLE CreateEvent(
LPSECURITY_ATTRIBUTES lpEventAttributes, //Security Descriptor
BOOL bManualReset,
BOOL bInitialState,
LPCTSTR lpName
);
- lpEventAttributes: 이 객체를 접근할수 있는 보안수준. 보통 NULL사용. SecurityDescriptor가 존재한다는것은 커널 객체임을  의미. 커널 객체는 윈도우 커널에서 관리.
- bManualReset: TRUE이면 수동으로 직접 ResetEvent를 호출하여 비시그널상태를 만들어야 함. FALSE이면 시그널 상태가 되면 자동으로 비시그널 상태로 변경됨(WaitForSingleObject로 대기중이던 스레드는 시그널이 되어 동작이 재개되고, 다시 비시그널 상태가 된다는 의미)
- bInitialState: 초기 시그널 설정값.
- lpName: 이벤트 커널 객체에게 이름을 부여. NULL이면 이름없는 객체. 보통 NULL 사용. 커널 객체는 커널에서 관리되므로, 이름 있는 커널 객체는 다른 프로세스의 스레드와 동기화하는데 사용가능.

HANDLE g_hEvent;
DWORD WINAPI _EventTest(LPVOID lpObj) {
  HANDLE hEvent = (HANDLE)lpObj;
  DWORD dwRet;
  while(1) {
    //g_hEvent가 시그널 상태가 될 때까지 무한 대기
    dwRet = ::WaitForSingleObject(g_hEvent, INFINITE);
    //setEvent로 시그널 되면 Wait를 빠져나옴
    //Auto Reset이므로 바로 비시그널 상태
    ::MessageBox(NULL, "시그널 되었음", "Event", MB_OK);
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

3. 여러개의 시그널 기다리기
DWORD WaitForMultipleObjects(
   DWORD nCount,
   CONST HANDLE * lpHandles,
   BOOL bWaitAll,
   DWORD dwMilliseconds
);
- nCount: 검사할 객체의 갯수
- lpHandles: 객체의 핸들 배열
- bWaitAll: TRUE이면 모든 객체가 시그널 될때까지 기다리고, FALSE이면 하나의 객체가 바뀌면 Wait에서 벗어남. FALSE일때, 리턴 값으로 시그널 된 객체의 배열 번째 수가 리턴(두 번째 시그널이라면, WAIT_OBJECT_0 + 1)
- dwMilliseconds: 타임아웃을 주어 무한정 기다리지 않도록 함. INFINITE는 무한정 기다림.

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
      ::MessageBox(NULL, "1st 시그널 되었음", "Event", MB_OK);
      break;
    case WAIT_OBJECT_0 + 0:
      ::MessageBox(NULL, "2st 시그널 되었음", "Event", MB_OK);
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

07. 멀티 스레드에서 MFC Socket사용하기
CAsyncSocket이나 CSocket같은 소켓 클래스의 경우 다른 스레드로 소켓 객체를 넘길때는 (MS에서 권고하지 않지만), 소켓 클래스의 Detach함수를 사용 실제 소켓 핸들을 떼어내 다른 스레도로 넘긴뒤, 다른 스레드에서 소켓 핸들을 Attach시키는 방법(MSDN : MultiSoc:Illustrates Using Sockets in Multiple Thread 기사와 소스 참조)

void CListenSoc::OnAccept(int nErrorCode) {
CSocket soc;
//임시소켓으로 일단 억셉트
Accept(soc);
//소켓의 실제 처리를 담당할 CConnectThread를 Suspend상태로 생성(derived CWinThread)
CConnectThread * pTHread = (CConnectThread*) AfxBeginThread(
RUNTIME_CLASS(CConnectThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
if(!pThread) {
  soc.Close();
  TRACE("생성 실패!\n");
  return ;
}

//스레드 초기화 작업
CServerDlg * pDlg = (CServerDlg *) m_pWndDlg;
//set members of CConnectThread.m_socket
pThread->m_socket.m_pCriticalSection = &pDlg->m_criticalSection;
pThread->m_socket.m_pLastString = &pDlg->m_pLastString;
//소켓의 핸들을 Detach해서 넘겨줌
pThread->m_hSocket = soc.Detach();
//스레드 구동시 InitInstance함수 호출
pTHread->ResumeThread();

CAsyncSocket::OnAccept(nErrorCode);
}

BOOL CConnectThread::InitInstance() {
//메인 윈도우로 스레드가 생성되었음을 보냄.
//WM_THERADSTART는 define해서 만들어낸 메시지.
::PostMessage(AfxGetApp->m_pMainWnd->m_hWnd, WM_THREADSTART, 0, 0);

//소켓 핸들로 Attach
m_socket.Attach(m_hSocket);
m_socket.m_pThread = this;
return TRUE;
}
}

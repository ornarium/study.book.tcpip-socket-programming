ch17. 윈속 고성능 네트워크 프로그래밍
section. 오버랩드 I/O
IOCP를 이해하는데 도움.

오버랩드I/O는 입력과 출력이 중첩되어 처리된다는 것. 기존의 입출력 방식은 입력동작이 이루어지는 동안 그 동작의 결과가 나올 때 까지 기다리는 방식이지만, 입력과 여러 출력이 중첩되어 병렬적으로 처리됨.

오버 랩드 I/O의 첫번째 이점은 디바이스로부터 읽어온 데이터를 중간 버퍼에 복사하는 과정없이 사용자 버퍼로 데이터가 들어온다는 것.  그리고, 파일 비동기 I/O로 기존의  First Come First Serve 스케쥴 방식이 아닌, SCAN스케쥴 방식으로 헤드암 위치로부터 가까운 데이터 위치를 먼저 읽어낸다.

두 번째 이점은 여러 디스크 접근을 하게 될때, 비동기적으로 동시에 여러 디스크에 대한 접근처리가 일어나 동시에 I/O처리를 요청하므로 디바이스 레벨 I/O 병렬 처리라고 말한다

오버랩드 I/O는 이벤트 객체가 시그널 되거나 완료 함수가 호출되는 것을 통해서 디바이스 입출력 동작의 결과를 통보받을 수 있다.

파일 입출력에서도 파일 I/O확장 함숭ㄴ ReadFileEx와 WriteFileEx를 사용하면 APC(Asynchronous Procedure Call)기법을 통해 파일 I/O 완료 함수(Completion 루틴)도 결과를 통보받을 수 있다. 좀더 나아가 파일I/O에서도 파일 IOCP(I/O Completion Port)를 통해 고성능 I/O를 구현할 수 있다

기존 I/O방식의 TCP에서 적은양을 내보내면 매번 적은 양의 데이터를 보내게 되므로 네트워크 성능에 나쁜 영향을 주게 됨. 오버랩드 I/O인 경우에는 TCP상황에 적은 양의 WSASend함수가 여러번 호출된다고 하더라도 전송계층에서 하나의 큰 덩어리로 묶여 보내주게 되어 효츌적으로 보내기가 제공.

기존 I/O방식의 TCP스택 내부의 Send, Receive버퍼의 크기가 정해져 있기 때문에 내부 스택에 있는 버퍼가 꽉 차게 되면, 신뢰성 있는 데이터 전송을 처리하기 위해 추가적 네트워크 부하가 발생할 수있는데, 오버랩드 I/O에서는 여러개의 사용자 버퍼를 WSARecv호출로 여러번 받을 수 있고, 미리 요청해서 I/O를 예약할 수 있기 때문에, 내부 버퍼가 꽉 차는 상황을 피할 수 있다(UDP에서도 Receive 버퍼를 사용하지 않음으로 내부 버퍼가 꽉 차는 과부하를 줄일수 있는 이점이 있음)

section02. winsock2 오버랩드 I/O 프로그래밍
소켓에서 오버랩드 I/O를지원하게 하려면 WSASocket 함수 호출로  오버랩드 속성(WSA_FLAG_OVERLAPPED)을 준다. UDP오버랩드 I/O는 SOCK_DGRAM을 주고 WSASendTo/WSAReceiveFrom을 호출

g_hSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
if(g_hSocket == INVALID_SOCKET) {
  MessageBox(g_hWnd, "오버랩드 소켓 생성 실패", "알림", MB_OK);
  return ;
}

보내기를 할때  호출 즉시 리턴되고 버퍼와 함께 오버랩드 I/O요청(예약)이 이루어지며, 함수 호출시 해당 버퍼와 오버랩드 객체가 오버랩드 I/O마다 쌍으로 존재한다. 완료 통지는 호출 순서대로 받지 않고, 완료 통지가 올때까지 버퍼를 파괴시키면 안된다.

OVERLAPPED ov1, ov2, ov3;
WSASend(s, buf1, ov1);
WSASend(s, buf2, ov2);
WSASend(s, buf3, ov3);


int WSASend(
  Socket s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesSent,
  DWORD dwFlags,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

int WSARecv(
  Socket s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesRecvd,
  DWORD dwFlags,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);


WSASend 함수를 오버랩드 방식으로 호출하게 되면 Overlapped I/O가 발생되었다고 표현. WSASend 호출할 때마다 각각 다른 오버랩드 객체를 넣어야 하기 때문에 Overlapped구조체는 Overlapped IO Context정보로 유용히 사용.

typdef struct _WSAOVERLAPPED {
  DWORD Internal;
  DWORD InternalHigh;
  DWORD Offset;
  DWORD OffsetHigh;
  WSAEVENT hEvent;
} WSAOVERLAPPED, * LPWSAOVERLAPED;

hEvent는 오버랩드 I/O가 완료되는 것을 알리는데 사용되는 이벤트 객체. hEvent만 알면 되고 나머지는 예약된 멤버 자료들
이기 때문에 알 필요가 없다. 

오버랩드 I/O에서 입출력 결과를 통보받는 방법은 2가지, WSAOVERLAPPED 구조체의 시그널 사용하는것과 완료함수를 이용하는 것. lpCompletionRoutine을 NULL로 주면 hEvent를 통해서 완료를 통보하고 lpCOmpletionRoutine이 NULL이 아니면 hEvent를 쓰지 않고, 완료함수 호출.

WSAWaitForMultipleEvents/WSAGetOverLappedResult 함수를 이용해서 완료가 되었는지 Block/비Block 처리를 할 수 있다. 주로 얼마만큼 보내고 받았는지 알수 있기 때문에, WSAGetOverlappedResult 함수를 많이 사용. 

완료 함수를 이용하는 것은 WSASend함수의 마지막 인자인 lpCompletionRoutine에 Callback 함수를 넣어서 호출.

void CALLBACK CompletionROUTINE(
  IN DWORD dwError,
  IN DWORD cbTransferred,
  IN LPWSAOVERLAPPED lpOverlapped,
  IN DWORD dwFlags
);

section03. 이벤트 객체를 이용한 오버랩드 I/O 결과 통보받기

보내야할 데이터가 많으면 WSASend함수는 SOCKET_ERROR를 리턴하게 됨. WSAGetLastError함수를 호출해서 오류값을 확인하는데, WSA_IO_PENDING이면 오버랩드I/O가 정상적으로 요청되어 진행중임을 의미.

BOOL WSAGetOverlappedResult(
  SOCKET s,
  LPWSAOVERLAPPED lpOverlapped,
  LPDWORD lpcbTransfer,
  BOOL fWait,
  LPDWORD lpdwFlags
);

- lpcbTransfer: 실제 전송된 데이터의 바이트 수
- fWait: TRUE이면 Blocking/ False이면 비Blocking
- lpdwFlags: WSARecv/WSARecvFrom함수를 호출했을때 사용된 dwFlags, 오버랩드 I/O완료 후의 변경된 값으로 리턴

오버랩드 I/O는 받을 데이터가 없어도 미리 사용자 버퍼를 마련해서 WSARecv함수를 여러번 호출해서 예약해둘 수 있음. 만약 받을 버퍼를 예약해두지 않은 상태에서 WSARecv를 미리 호출하게 된 경우 데이터가 오면 FD_READ이벤트가 발생. FD_READ이벤트를 처리할때 WSARecv를 호출(이전과 같이)

section04. 완료 함수를 통한 오버랩드 I/O 결과 통보 받기

완료함수에서 어떤 데이터를 처리했고 어떤 소켓핸들의 수행이 완료되었는지 알수가 없다. 따라서 완료 루틴의 세번째 인자인 lpOverlapped를 오버랩드I/O에 대한 I/O컨텍스트로 사용해야 함.

typedef struct _MYOVERLAPPED : public WASOVERLAPPED {
  //WSAOVERLAPPED는 정보가 부족하므로 상속을 하거나 MYOVERLLAPPED의 첫 멤버 자료로 위치시킴.
  SOCKET hSocket;
  WSABUF wsabuf;
} MYOVERLAPPED;

void CALLBACK SendCompletionROUTINE(
  IN DWORD dwError,
  IN DWORD cbTransferred,
  IN LPWSAOVERLAPPED  lpOverlapped,
  IN DWORD dwFlags
) {
  MYOVERLAPPED * myov = (MYOVERLAPPED*) lpOverlapped;
  ...
  delete [] myov->wsabuf.buf;
  delete myov;
}

int send() {
MYOVERLAPPED * myov = new MYOVERLAPPED;
myov->hSocket = g_hSocket;
...
nError = WSASend(g_hSocket, &myov->wsabuf, 1, &dwSent, 0, myov, SendCompletionROUTINE);
if(nError == 0) {
//한번에 보내기 성공
} else if(nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING) {
  //오버랩드 전송이 시작됨, 전부 완료되면 오나료함수가 호출됨.
  //오버랩드 I/O함수를 호출한 스레드는 경고성 대기모드로 들어가 있어야 완료함수가 호출될수 잇음
  SleepEx(INFINITE, TRUE);
  return ;
} else {
  //보내기 오류
  closesocket(g_hSocket);
}
delete [] myov->wsabuf.buf;
delete myov;
}


void CALLBACK RecvCompletionROUTINE(
  IN DWORD dwError,
  IN DWORD cbTransferred,
  IN LPWSAOVERLAPPED  lpOverlapped,
  IN DWORD dwFlags
) {
  MYOVERLAPPED * myov = (MYOVERLAPPED*) lpOverlapped;
  ...
  delete [] myov->wsabuf.buf;
  delete myov;
}

void onrecv() {
MYOVERLAPPED * myov = new MYOVERLAPPED;
myov->hSocket = g_hSocket;
...
nError = WSARecv(g_hSocket, &myov->wsabuf, 1, &dwRead, &dwFlags, myov, RecvCompletionRoutine);
if(nError == 0) {
 //한번에 받기 성공
} else if(nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING) {
  SleepEx(INFINITE, TRUE);
  return;
} else {
  closesocket(g_hSocket);
}
delete [] myov->wsabuf.buf;
delete myov;
}

WSARecv를 호출한 스레드가 SleepEx를 호출하면 경고성 대기 상태가 되어 멈추게 됨. 경고성 대기 상태가 된 스레드는 APC함수가 APC큐에 추가될때 대기상태에서 빠져나와서 APC함수를 호출하고 동작을 계속하게 됨. 완료함수는 Overlapped I/O가 완료될때 APC큐에 추가되기 때문에 경고성 대기 상태인 스레드는 오버랩드 I/O가 오나료되었을 때 완료 함수를 수행.

section05. APC

윈도우 스레드는 내부에 APC함수를 호출할 수 있도록 APC(Asynchronous Procedure Call)큐를 지니고 있음(윈도우 스레드 특징). 오버랩드 I/O완료 함수 통지방식과 IOCP가 APC메커니즘을 이용해서 동작.

QueueUserAPC로 APC함수를 등록하게 되면 이 스레드는 APC함수를 바로 호출하지 않고 APC큐에 쌓아둠. 그런 후 해당 스레드가 alertable(경고 또는 알림이 가능한) 상태가 되면 APC함수들이 큐에 들어왔던 순서대로 호출하게 됨(FIFO, FIrst In First Out)

DWORD QueueUserAPC(
  PAPCFUNC pfnAPC,
  HANDLE hThread,
  ULONG_PTR dwData
);

- pfnAPC: 해당 스레드의 APC큐에 pfnAPC함수 추가
- dwData: APC함수의 파라미터로 dwData를 사용

스레드가 경고성 대기상태가 되면 호출되는 APC함수의 원형은 아래와 같음
VOID CALLBACK APCProc(
  ULONG_PTR dwParam
);

스레드가 alertable wait 상태, 경고성 대기 상태가 될 수 있는 함수에는 SleepEx, SignalObjectAndWait, WaitForSingleObjectex, WaitForMutipleObjectsEx, MsgWaitForMutipleObjectsEx 등이 있다.
윈속의 완료루팅 오버랩드 I/O에서는 윈속 서비스 제공함수인 WPUQueueAPC를 이용한 후 완료루틴을 APC큐에 추가해 오버랩드 완료를 통보. WPUQueueApc에 대해서는 프로토콜 스택 서비스 제공과 관련된 함수이며 내부적 구현은 QueueUserApc와 비슷함.

** QueueUserAPC: undeclared identifier 컴파일 오류시 Visual C++의 라이브러리가 최신이  아니기 때문에 발생하는 문제라서, Platform SDK를 설치한뒤 Tools->Options->Directories에서 Include files에서 디렉토리 지정.





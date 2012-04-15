ch16. 유용한 윈속2 프로그래밍
section01. Winsock2 소개

윈속1.1에서 여러 프로토콜 스택을 공통의 인터페이스로 사용할수 있도록 WOSA(Windows Open System Architecture)모델로 바뀜. 여러 프로토콜 스택을 사용하기 위해 애플리케이션 개발자는 WS2_32.DLL에서 제공하는 Windows Sockets2 API(Winsock2)의 공통 인터페이스만 사용하면 됨. 필요한 경우 SPI를 사용 전송계층ㅇ ㅔ프로토콜을 생성해 추가 가능. 소켓 네트워크 감시 및 제어가 필요함녀 SPI계층의 LSP(Layered Service Provider)라는 특수 DLL을 만들 수 있음

section02. 조건부 억셉트

서버가 원치않는 클라이언트를 reject하기 위해서는 어떤 호스트인지 알아내기 위해, 무조건 Accept를 해야만 한다. 그렇기 때문에 불필요한 서버 리소스를 써버리게 된다. Winsock2의 새로운 WSAAccept함수는 조건부 억셉트 기능을 두어 원치않는 클라이언트에 대해 조건적으로 받아들이지 않게 할수 있다.

* LSP를 작성할 경우, 조건부 억셉트를 적용하면 원치 않는 호스트의 접속 공격을 막는 방화벽 기능도 마련할수 있음.


SOCKET WSAAccept(
  SOCKET s,
  struct sockaddr FAR * addr,
  LPINT addrlen,
  LPCONDITIONPROC lpfnCondition,
  DWORD dwCallbackData
);
- s: 리슨 소켓의 핸들
- addr: 억셉트될 상대편 주소의 구조체
- addrlen: addr구조체의 길이
- lpfnCondition: 조건부 억셉트 함수
- dwCallbackData: 조건부 억셉트 함수 호출 데이터

int CALLBACK ConditionFunc(
  IN LPWSABUF lpCallerId,
  IN LPWSABUF lpCallerData,
  IN OUT LPQOS lpSQOS,
  IN OUT LPQOS lpGQOS,
  IN LPWSABUF lpCalleeId,
  OUT LPWSABUF lpCalleeData,
  OUT GROUP FAR * g,
  IN DWORD dwCallbackData
);
* Caller: 서버에 접속해오는 클라이언트 정보
* Callee: 서버 자기자신에 대한 내용
- lpCallerId: WSABUF의 buf에 SOCKADDR구조체를 담고 있어 접속을 시도한 클라이언트 주소 정보를 얻어낼수 있음
- lpCallerData: 연결시점에서 받은 데이터가 있다면 WSABUF로 받는다
- lpQOS, lpGQOS: QoS관련 인자
- lpCalleeId: 서버의 주소 정보
- lpCalleeData: 접속 시점에 클라이언트로 보낼 데이터 설정 인자. TCP에서는 조건부 함수가 호출되기 전에 Client와 접속 과정이 완료되므로 의미 없음.
- dwCallbackData는 WSAAccept호출시 마지막에 넣었던 Callback 인자. 처리시 도움될만한 추가 데이터를 사용 가능.

WSABUF는 버퍼정보를 담기위한 구조체
typedef struct _WSABUF {
  u_long len;
  char FAR * buf;
} WSABUF, FAR * LPWSABUF;


새로운 클라이언트가 리슨 소켓으로 접속하면 접속 대기큐(크기는 listen의 인자인 backlog로 설정)에 들어감. 이때 비블로킹 모드 소켓은 FD_ACCEPT 발생. 

억셉트를 위해 WSAAccept가 호출되고, 조건부 억셉트 함수에서 CF_ACCEP를 리턴하면 억셉트된 소켓핸들을 리턴하고, CF_REJECT를 리턴하면 접속거절을 의미하는 WSAECONNREFUSED라는 오류코드 발생. 조건판단을 미루기 위해서는 CF_DEFER를 사용하여 나중으로 미룰수 있음(WSAAccept실행되는 스레드에서 돌아가므로, 조건이 길어질 경우 스레드가 Block될수 있음)

접속대기큐가 비어있다면 blocking모드에서는 접속이 발생할때까지 멈춰있고, non-blocking에서는 WSAEWOULDBLOCK오류 리턴


무조건 reject 시키는 예제
int CALLBACK ConditionFunc(
  IN LPWSABUF lpCallerId,
  IN LPWSABUF lpCallerData,
  IN OUT LPQOS lpSQOS,
  IN OUT LPQOS lpGQOS,
  IN LPWSABUF lpCalleeId,
  OUT LPWSABUF lpCalleeData,
  OUT GROUP FAR * g,
  IN DWORD dwCallbackData
) {
  SOCKADDR_IN * psa;
  psa = (SOCKADDR_IN *) lpCallerId->buf;
  if(strcmp("127.0.0.1", inet_ntoa(psa->sin_addr)) == 0)
    MessageBox(g_hWnd, "로컬 루프백", "addrr", MB_OK);
  
  return CF_REJECT;
}


section03. scatter-gather I/O

또는 vectored I/O라고 부름

메시지의 데이터 형태가 정해져 있고, 보내거나 받기 위해 하나의 버퍼에 복사하는 비용을 없애서 네트워크 성능을 유지하는 방법. 

WSARecv/WSARecvFrom를 사용시 버퍼 배열 사용

int WSASend(
  SOCKET s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesSent,
  DWORD dwFlags,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

- lpBuffers, dwBufferCount: scatter-gather I/O와 관련
- lpNumberofBytesSent: 한번에 보내진 양
- lpOverlapped, lpCompletionRoutine: 오버랩드 I/O와 관련

한방에 보내기 예제
char * pHeader;  //10바이트 헤더
char * pContents  //30바이트 메시지 본체

WSABUF wbuf[2];
wbuf[0].buf = pHeader;
wbuf[0].len = 10;

wbuf[1].buf = pContents;
wbuf[1].len = 30;

DWORD dwSent;
WSASend(s, wbuf, 2, &dwSent, 0, NULL, NULL);

한방에 받기 예제
char * pHeader = new char[10]; //10바이트 헤더
char * pContents = new char[30]; //30바이트 메시지 본체

WSABUF wbuf[2];
wbuf[0].buf = pHeader;
wbuf[0].len = 10;

wbuf[1].buf = pContents;
wbuf[1].len = 30;

DWORD dwRead;
WSARecv(s, wbuf, 2, &dwRead, 0, NULL, NULL);

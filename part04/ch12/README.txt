CH12. Win32 윈속 프로그래밍
section01. 윈속 I/O 모델

윈속 I/O는 BSD소켓 사용.
윈도우에서는 BSD소켓의 select함수보다 다음의 방식으로 좀더 효율적으로 사용가능
- WSAAsyncSelect: 윈도우 메시지 메커니즘 사용하여 동작결과 통보.
즉 메시지펌핑을 통해 메시지 큐에서 확인. 단일 스레드 환경(GUI 스레드)일 경우, 소켓 입출력 메시지와 GUI메시지가 이벤트처리에 방해를 줄 수 있음. 이차 스레드에서도 사용하면 해결이 되나, 그런 경우는 WSAEventSelect로 해결.

- WSAEventSelect: 윈도우 이벤트 메커니즘 이용 동작결과 통보.
이차 스레드에서 WSAWaitForMultipleEvents를 이용해 이벤트 발생시 WSAEnumNetworkEvents로 이벤트 확인후 동작처리

section02. WSAAsyncSelect모델

WSAAsyncSelect 함수호출이 성공하면 해당 소켓은 비블로킹 소켓으로 변경. CAsyncSocket은 WSAAsyncSelect를 사용하여 구현됨. 동시에 여러 네트워크 이벤트를 통보받지만, 한번에 하나의 네트워크 이벤트 메시지 발생.
int WSAAsyncSelect(
  SOCKET s,
  HWND hWnd,
  unsigned int wMsg,
  long lEvent
);
- s: 설정하고자 하는 소켓 해늗ㄹ
- hWnd: 메시지를 받을 윈도우 핸들
- wMsg: 네트워크 이벤트 발생시 받고 싶은 메시지.
  * FD_READ: 새로운 데이터알림
  * FD_WRITE: 데이터 보내기 가능
  * FD_OOB: Out-Of-Band 데이터 받음
  * FD_ACCEPT: 새로운 클라이언트 접속
  * FD_CONNECT: 접속이 완료 또는 Multi-point Join 수행이 완료)
  * FD_CLOSE: 소켓접속 끊김
  * FD_QOS: Quality of Service 변경되었음
  * FD_GROUP_QOS: 예약되어 사용안함.
  * FD_ROUTING_INTERFACE_CHANGE: 특정복적지에 대한 라우팅 이벤트 변경됨
  * FD_ADDRESS_LIST_CHANGE: 소켓 프로토콜 패밀리 주소 목록이 변경됨
 (일반적으로 FD_READ, FD_WRITE, FD_ACCEPT, FD_CONNECT, FD_CLOSE 만 이해해두면 됨)

<사용하기>

#define WM_MY_SOCKET (WM_APP + 1)
WSAAsyncSelect(s, hWnd, WM_MY_SOCKET, FD_CONNECT|FD_WRITE|FD_READ|FD_CLOSE);

LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
  int nEvent, nError;
  switch(nMsg) {
    case WM_MY_SOCKET:
    //어떤 이벤트가 있는지 알아낼때 사용. Winsocket2.h에 선언되어 있음
    nEvent = WSAGETSELECTEVENT(lParam);
    //오류가 발생한 경우, 오류코드를 담음. WSAGetLastError의 오류코드가 다를수 있기 때문에
    //WSAGETSELECTERROR쪽을 신뢰할 것.
    nError = WSAGETSELECTERROR(lParam);
    if(nEvent == FD_READ) {
      //새로 받은데이터가 있으므로 recv 또는 WSARecv호출. MFC Socket은 OnReceive 호출 해당.
    } else if(nEvent == FD_WRITE) {
      //데이터를 보내기 가능 send 또는 WSASend호출, MFC Socket은 OnSend호출해당
    }
    break;
  }
  ...
}

* 리슨소켓 처리

#define WM_LISTEN_SOCKET (WM_APP+1)
#define WM_CLIENT_SOCKET (WM_APP+2)

WSAAsyncSelect(g_hListen, hWnd, WM_MY_SOCKET, FD_ACCEPT);

// ...
// accept함수로 클라이언스 담당소켓 연결

g_hClient = accept(g_hListen, 생략);

WSAAsyncSelect(g_hClient, hWnd, WM_MYSOCKET_NOTIFY,FD_WRITE|FD_READ,FD_CLOSE);

accept후 얻은 클라이언트 담당 소켓의 이벤트 속성은 리슨소켓의 지정된값을 가져옴.
수정하려면 WSAAsyncSelect 호출.

* 이벤트 별 메시지 나눠받기
WSAAsyncSelect(s, hWnd, WM_MY_SOCKET_WRITE, FD_WRITE);
WSAAsyncSelect(s, hWnd, WM_MY_SOCKET_READ, FD_READ);

* 블로킹 소켓 전환
int rc; unsigned long val;
//WSAAsyncSelect 취소
rc = WSAAsyncSelect(s, hWnd, 0, 0);
//소켓을 블로킹 모드 전환
val = 0; //블로킹
ioctlsocket(s, FIONBIO, &val);

WSAAsyncSelect를 취소했더라도 되기전의 메시지가 메시지큐에 남아있을수 있으므로, 취소후에 메시지를 처리해야함. closesocket으로 소켓을 닫으면 취소효과가 나타나, 마찬가지로 메시지큐 처리해줘야함.

* WSAEWOULDBLOCK이 발생할 것을 염두
ioctlsocket(s, FIONREAD, ...)로 메시지 도착 확인 및 recv 처리 루틴이 들어갈 경우, 발생 확인 높음. CAsyncSocket에서 이야기 된 내용. 즉 FD_READ가 발생했을 경우, recv만 한 번 호출하면 생기지 않을 문제. 항상 WSAEWOULDBLOCK이 발생할것을 염두해두고 이를 처리하는 루틴 작성.

* 네트워크 발생 시각
FD_READ는 데이터가 아무리 많이 들어와도 한 번만 발생. 네트웍 이벤트가 여러번 발생하더라도 메시지는 여러번 발생하지 않음을 의미. FD_READ가 담긴 메시지가 한번 발생되면, recv를 호출하기 전까지 메시지 발생안함. 이런 함수를 메시지 재활성화 함수(Message re-enable function)라고 부름.

한 FD_READ에 recv를 여러번 호출하지 않아도, 한 번만 호출하면 다시 메시지가 발생하므로, 그런식으로 처리.

- FD_READ: recv, recvfrom, WSARecv, WSARecvFrom
- FD_WRITE: send, sendto, WSASend, WSASendTo
- FD_OOB: recv, recvfrom, WSARecv, WSARecvFrom
- FD_ACCEPT: accept,WSAAccept
(조건 함수 리턴이 CF_DEFER가 되어 lParam이 WSATRY_AGAIN인경우 WSAAccept 제외)
- FD_CONNECT: 없음
- FD_CLOSE: 없음
- FD_QOS: SIO_GET_QOS를 쓰는 WSAIoCtl
- FD_GROUP_QOS: 예약됨(사용할일 없음)
- FD_ROUTING_INTERFACE_CHANGE: SIO_ROUTING_INTERFACE_CHANGE를 쓰는 WSAIoCtl
- FD_ADDRESS_LIST_CHANGE: SIO_ADDRESS_LIST_CHANGE를 쓰는 WSAIoCtl

FD_WRITE는 FD_CONNECT발생후 바로 FD_WRITE 발생. 서버의 경우 accept후 억셉트 소켓에서 FD_WRITE 발생. FD_WRITE 발생전에 보내면 WSAEWOULDBLOCK 오류 발생. udp의 경우 bind후 FD_WRITE발생

FD_CLOSE는 lParam의 오류코드가 0이면 Graceful Closeure. WSAECONNRESET이면 강제 재부팅 또는 상대방 소켓의 예상치 못하게 끊김을 의미. setsockopt함수의 SO_LINGER 사용시 데이터가 TCP스택에서 다보내지 못한 상태에서 끊긴(hard close) 경우에도 발생(TCP가상회선 리셋). 상대방의 shutdown호출 또는 closesocket호출로 소켓상태가 TIME WAIT이나 CLOSE WAIT일 때도 발생. 자신의 closesocket시에는 발생안함.
FD_CLOSE에서는 closesocket호출해서 끊으면 됨.

------------------
#pragma comment(lib, "라이브러리명") //컴파일러 지시자. 라이브러리 링크
#pragma warning(disable: 4705) //해당 워닝 무시.
------------------

소켓 기능을 추가하려면 StdAfx.h를 연후 아래 라인을 추가. Winsock API를 사용하기 위해서 winsock2.h를 include하고 pragma comment를 사용. winsock2 lib파일을 링커 옵션에 추가

#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

만약 MFC에서 직접  WSAAsyncSelect를 쓰고자 한다면, 메시지 맵 매크로를 사용하여 해당 메시지를 등록하고, WSAAsyncSelect를 사용해서 설정해주면 된다.


section04. WSAEventSelect 모델

	
int WSAEventSelect(
  SOCKET s,
  WSAEVENT hEventObject,
  long lNetworkEvents
);
- s: Event를 사용해서 비블로킹될 소켓을 넣으면 됨.
- hEvnetObject: 설정한 네트워크 이벤트 중 하나 이상이 발생되면 시그널 되는 이벤틑 객체
- lNetworkEvents: 받고 싶은 네트워크 이벤트에 대한 OR값

WSAAsyncSelect와는 다르게 각 이벤트를 중첩 적용할 수 없고, 한 번에 설정해야 됨(최근 설정만 적용됨). 설저오딘 이벤트를 취소하려면 WSAEventSelect(s, hEvent,Object, 0) 호출

int WSAEnumNetworkEvents(
  SOCKET s,
  WSAEVENT hEventObject,
  LPWSANETWORKEVENTS lpNetworkEvents
);
- s: 소켓 핸들
- hEventObject: 이벤트 객체
- lpNetworkEvents: 발생된 네트워크 이벤트
typdef struct _WSANETWORKEVENTS {
  long lNetworksEvents; //네트워크 이벤트의 OR값
  int iErrorCode[FD_MAX_EVENTS]; //각 네트워크 이벤트의 오류코드
} WSANETWORKEVENTS, FAR * LPWSANETWORKEVENTS;


아래과 같은 방식으로 네트워크 이벤트 발생을 확인가능
WSANETWORKEVENTS NetEvents;
int ce = WSAEnumNetworkEvents(g_hSocket, g_hEvent, &NetEvents);

//하나 이상의 이벤트를 동시에 받을 수 있다
if(NetEvents.lNetworkEvents & FD_CONNECT)
  OnConnect();
if(NetEvents.lNetworkEvents & FD_WRITE)
  OnSend;

OnConnect호출시 오류값 전달하려면
if(NetEvents.lNetworkEvents & FD_CONNECT)
  OnConnect(NetEvents.iErrorCode[FD_CONNECT_BIT]);

간단한 조각 코드
WSAEVENT g_hCLose = WSACreateEvent();
WSAEVENT g_hEvent = WSACreateEvent();
...
//메인 스레드에서 네트워크 이벤트를 감시하기 위한 이차 스레드를 생성
WSAEventSelect(s, g_hEvent, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE);
CreateThread(NUL, SocketTHread. ...);
...
//네트워크 이벤트를 받아서 처리하는 스레드
DWORD WINAPI SocketTHread(LPVOID lpData) {
  WSAEVENT EventToWaitFor[2];
  EventToWaitFor[0] = g_hClose; //소켓 대기 이벤트
  EventToWaitFor[1] = g_hEvent; //네트워크 이벤트를 받는 이벤트 객체

  while(1) {
    DWORD dwHandleSignaled = WSAWaitForMutipleEvents(
     2, EventToWaitFor, FALSE, INFINITE, TRUE);

  //어떤 이벤트가 발생한건가?
   switch(dwHandleSignaled) {
   case WSA_WAIT_EVENT_0:
     //Close이벤트 발생. 스레드 종료
     return 0;
   case WSA_WAIT_EVENT_0 + 1: //SocketEvent 발생
   {
      WSANETWORKEVENTS NetEvents;
      //그동안 발생됨 이벤트를 받음
      int ce = WSAEnumNetworkEvents(g_hSocket, g_hEvent, &NetEvents);
      //하나 이상의 이벤트를 동시에 받을 수 있음
      if(NetEvents.lNetworkEvents & FD_CONNECT)
        OnConnect();
      if(NetEvents.lNetworkEvents & FD_WRITE)
        OnSend();
      if(NetEvents.lNetworkEvents & FD_READ)
        OnReceive();
      if(NetEvents.lNetworkEvents & FD_CLOSE)
        OnClose();
      break;
   }
   }
}

section05. WSAEventSelect 프로그램 작성

CAsyncSocket과 유사하게 자신만의 Socket Class 작성.
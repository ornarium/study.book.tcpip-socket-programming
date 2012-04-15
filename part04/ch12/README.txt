CH12. Win32 ���� ���α׷���
section01. ���� I/O ��

���� I/O�� BSD���� ���.
�����쿡���� BSD������ select�Լ����� ������ ������� ���� ȿ�������� ��밡��
- WSAAsyncSelect: ������ �޽��� ��Ŀ���� ����Ͽ� ���۰�� �뺸.
�� �޽��������� ���� �޽��� ť���� Ȯ��. ���� ������ ȯ��(GUI ������)�� ���, ���� ����� �޽����� GUI�޽����� �̺�Ʈó���� ���ظ� �� �� ����. ���� �����忡���� ����ϸ� �ذ��� �ǳ�, �׷� ���� WSAEventSelect�� �ذ�.

- WSAEventSelect: ������ �̺�Ʈ ��Ŀ���� �̿� ���۰�� �뺸.
���� �����忡�� WSAWaitForMultipleEvents�� �̿��� �̺�Ʈ �߻��� WSAEnumNetworkEvents�� �̺�Ʈ Ȯ���� ����ó��

section02. WSAAsyncSelect��

WSAAsyncSelect �Լ�ȣ���� �����ϸ� �ش� ������ ����ŷ �������� ����. CAsyncSocket�� WSAAsyncSelect�� ����Ͽ� ������. ���ÿ� ���� ��Ʈ��ũ �̺�Ʈ�� �뺸������, �ѹ��� �ϳ��� ��Ʈ��ũ �̺�Ʈ �޽��� �߻�.
int WSAAsyncSelect(
  SOCKET s,
  HWND hWnd,
  unsigned int wMsg,
  long lEvent
);
- s: �����ϰ��� �ϴ� ���� �؈f��
- hWnd: �޽����� ���� ������ �ڵ�
- wMsg: ��Ʈ��ũ �̺�Ʈ �߻��� �ް� ���� �޽���.
  * FD_READ: ���ο� �����;˸�
  * FD_WRITE: ������ ������ ����
  * FD_OOB: Out-Of-Band ������ ����
  * FD_ACCEPT: ���ο� Ŭ���̾�Ʈ ����
  * FD_CONNECT: ������ �Ϸ� �Ǵ� Multi-point Join ������ �Ϸ�)
  * FD_CLOSE: �������� ����
  * FD_QOS: Quality of Service ����Ǿ���
  * FD_GROUP_QOS: ����Ǿ� ������.
  * FD_ROUTING_INTERFACE_CHANGE: Ư���������� ���� ����� �̺�Ʈ �����
  * FD_ADDRESS_LIST_CHANGE: ���� �������� �йи� �ּ� ����� �����
 (�Ϲ������� FD_READ, FD_WRITE, FD_ACCEPT, FD_CONNECT, FD_CLOSE �� �����صθ� ��)

<����ϱ�>

#define WM_MY_SOCKET (WM_APP + 1)
WSAAsyncSelect(s, hWnd, WM_MY_SOCKET, FD_CONNECT|FD_WRITE|FD_READ|FD_CLOSE);

LRESULT CALLBACK WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
  int nEvent, nError;
  switch(nMsg) {
    case WM_MY_SOCKET:
    //� �̺�Ʈ�� �ִ��� �˾Ƴ��� ���. Winsocket2.h�� ����Ǿ� ����
    nEvent = WSAGETSELECTEVENT(lParam);
    //������ �߻��� ���, �����ڵ带 ����. WSAGetLastError�� �����ڵ尡 �ٸ��� �ֱ� ������
    //WSAGETSELECTERROR���� �ŷ��� ��.
    nError = WSAGETSELECTERROR(lParam);
    if(nEvent == FD_READ) {
      //���� ���������Ͱ� �����Ƿ� recv �Ǵ� WSARecvȣ��. MFC Socket�� OnReceive ȣ�� �ش�.
    } else if(nEvent == FD_WRITE) {
      //�����͸� ������ ���� send �Ǵ� WSASendȣ��, MFC Socket�� OnSendȣ���ش�
    }
    break;
  }
  ...
}

* �������� ó��

#define WM_LISTEN_SOCKET (WM_APP+1)
#define WM_CLIENT_SOCKET (WM_APP+2)

WSAAsyncSelect(g_hListen, hWnd, WM_MY_SOCKET, FD_ACCEPT);

// ...
// accept�Լ��� Ŭ���̾� ������ ����

g_hClient = accept(g_hListen, ����);

WSAAsyncSelect(g_hClient, hWnd, WM_MYSOCKET_NOTIFY,FD_WRITE|FD_READ,FD_CLOSE);

accept�� ���� Ŭ���̾�Ʈ ��� ������ �̺�Ʈ �Ӽ��� ���������� �����Ȱ��� ������.
�����Ϸ��� WSAAsyncSelect ȣ��.

* �̺�Ʈ �� �޽��� �����ޱ�
WSAAsyncSelect(s, hWnd, WM_MY_SOCKET_WRITE, FD_WRITE);
WSAAsyncSelect(s, hWnd, WM_MY_SOCKET_READ, FD_READ);

* ���ŷ ���� ��ȯ
int rc; unsigned long val;
//WSAAsyncSelect ���
rc = WSAAsyncSelect(s, hWnd, 0, 0);
//������ ���ŷ ��� ��ȯ
val = 0; //���ŷ
ioctlsocket(s, FIONBIO, &val);

WSAAsyncSelect�� ����ߴ��� �Ǳ����� �޽����� �޽���ť�� ���������� �����Ƿ�, ����Ŀ� �޽����� ó���ؾ���. closesocket���� ������ ������ ���ȿ���� ��Ÿ��, ���������� �޽���ť ó���������.

* WSAEWOULDBLOCK�� �߻��� ���� ����
ioctlsocket(s, FIONREAD, ...)�� �޽��� ���� Ȯ�� �� recv ó�� ��ƾ�� �� ���, �߻� Ȯ�� ����. CAsyncSocket���� �̾߱� �� ����. �� FD_READ�� �߻����� ���, recv�� �� �� ȣ���ϸ� ������ ���� ����. �׻� WSAEWOULDBLOCK�� �߻��Ұ��� �����صΰ� �̸� ó���ϴ� ��ƾ �ۼ�.

* ��Ʈ��ũ �߻� �ð�
FD_READ�� �����Ͱ� �ƹ��� ���� ���͵� �� ���� �߻�. ��Ʈ�� �̺�Ʈ�� ������ �߻��ϴ��� �޽����� ������ �߻����� ������ �ǹ�. FD_READ�� ��� �޽����� �ѹ� �߻��Ǹ�, recv�� ȣ���ϱ� ������ �޽��� �߻�����. �̷� �Լ��� �޽��� ��Ȱ��ȭ �Լ�(Message re-enable function)��� �θ�.

�� FD_READ�� recv�� ������ ȣ������ �ʾƵ�, �� ���� ȣ���ϸ� �ٽ� �޽����� �߻��ϹǷ�, �׷������� ó��.

- FD_READ: recv, recvfrom, WSARecv, WSARecvFrom
- FD_WRITE: send, sendto, WSASend, WSASendTo
- FD_OOB: recv, recvfrom, WSARecv, WSARecvFrom
- FD_ACCEPT: accept,WSAAccept
(���� �Լ� ������ CF_DEFER�� �Ǿ� lParam�� WSATRY_AGAIN�ΰ�� WSAAccept ����)
- FD_CONNECT: ����
- FD_CLOSE: ����
- FD_QOS: SIO_GET_QOS�� ���� WSAIoCtl
- FD_GROUP_QOS: �����(������� ����)
- FD_ROUTING_INTERFACE_CHANGE: SIO_ROUTING_INTERFACE_CHANGE�� ���� WSAIoCtl
- FD_ADDRESS_LIST_CHANGE: SIO_ADDRESS_LIST_CHANGE�� ���� WSAIoCtl

FD_WRITE�� FD_CONNECT�߻��� �ٷ� FD_WRITE �߻�. ������ ��� accept�� ���Ʈ ���Ͽ��� FD_WRITE �߻�. FD_WRITE �߻����� ������ WSAEWOULDBLOCK ���� �߻�. udp�� ��� bind�� FD_WRITE�߻�

FD_CLOSE�� lParam�� �����ڵ尡 0�̸� Graceful Closeure. WSAECONNRESET�̸� ���� ����� �Ǵ� ���� ������ ����ġ ���ϰ� ������ �ǹ�. setsockopt�Լ��� SO_LINGER ���� �����Ͱ� TCP���ÿ��� �ٺ����� ���� ���¿��� ����(hard close) ��쿡�� �߻�(TCP����ȸ�� ����). ������ shutdownȣ�� �Ǵ� closesocketȣ��� ���ϻ��°� TIME WAIT�̳� CLOSE WAIT�� ���� �߻�. �ڽ��� closesocket�ÿ��� �߻�����.
FD_CLOSE������ closesocketȣ���ؼ� ������ ��.

------------------
#pragma comment(lib, "���̺귯����") //�����Ϸ� ������. ���̺귯�� ��ũ
#pragma warning(disable: 4705) //�ش� ���� ����.
------------------

���� ����� �߰��Ϸ��� StdAfx.h�� ���� �Ʒ� ������ �߰�. Winsock API�� ����ϱ� ���ؼ� winsock2.h�� include�ϰ� pragma comment�� ���. winsock2 lib������ ��Ŀ �ɼǿ� �߰�

#include <windows.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

���� MFC���� ����  WSAAsyncSelect�� ������ �Ѵٸ�, �޽��� �� ��ũ�θ� ����Ͽ� �ش� �޽����� ����ϰ�, WSAAsyncSelect�� ����ؼ� �������ָ� �ȴ�.


section04. WSAEventSelect ��

	
int WSAEventSelect(
  SOCKET s,
  WSAEVENT hEventObject,
  long lNetworkEvents
);
- s: Event�� ����ؼ� ����ŷ�� ������ ������ ��.
- hEvnetObject: ������ ��Ʈ��ũ �̺�Ʈ �� �ϳ� �̻��� �߻��Ǹ� �ñ׳� �Ǵ� �̺��z ��ü
- lNetworkEvents: �ް� ���� ��Ʈ��ũ �̺�Ʈ�� ���� OR��

WSAAsyncSelect�ʹ� �ٸ��� �� �̺�Ʈ�� ��ø ������ �� ����, �� ���� �����ؾ� ��(�ֱ� ������ �����). �������� �̺�Ʈ�� ����Ϸ��� WSAEventSelect(s, hEvent,Object, 0) ȣ��

int WSAEnumNetworkEvents(
  SOCKET s,
  WSAEVENT hEventObject,
  LPWSANETWORKEVENTS lpNetworkEvents
);
- s: ���� �ڵ�
- hEventObject: �̺�Ʈ ��ü
- lpNetworkEvents: �߻��� ��Ʈ��ũ �̺�Ʈ
typdef struct _WSANETWORKEVENTS {
  long lNetworksEvents; //��Ʈ��ũ �̺�Ʈ�� OR��
  int iErrorCode[FD_MAX_EVENTS]; //�� ��Ʈ��ũ �̺�Ʈ�� �����ڵ�
} WSANETWORKEVENTS, FAR * LPWSANETWORKEVENTS;


�Ʒ��� ���� ������� ��Ʈ��ũ �̺�Ʈ �߻��� Ȯ�ΰ���
WSANETWORKEVENTS NetEvents;
int ce = WSAEnumNetworkEvents(g_hSocket, g_hEvent, &NetEvents);

//�ϳ� �̻��� �̺�Ʈ�� ���ÿ� ���� �� �ִ�
if(NetEvents.lNetworkEvents & FD_CONNECT)
  OnConnect();
if(NetEvents.lNetworkEvents & FD_WRITE)
  OnSend;

OnConnectȣ��� ������ �����Ϸ���
if(NetEvents.lNetworkEvents & FD_CONNECT)
  OnConnect(NetEvents.iErrorCode[FD_CONNECT_BIT]);

������ ���� �ڵ�
WSAEVENT g_hCLose = WSACreateEvent();
WSAEVENT g_hEvent = WSACreateEvent();
...
//���� �����忡�� ��Ʈ��ũ �̺�Ʈ�� �����ϱ� ���� ���� �����带 ����
WSAEventSelect(s, g_hEvent, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE);
CreateThread(NUL, SocketTHread. ...);
...
//��Ʈ��ũ �̺�Ʈ�� �޾Ƽ� ó���ϴ� ������
DWORD WINAPI SocketTHread(LPVOID lpData) {
  WSAEVENT EventToWaitFor[2];
  EventToWaitFor[0] = g_hClose; //���� ��� �̺�Ʈ
  EventToWaitFor[1] = g_hEvent; //��Ʈ��ũ �̺�Ʈ�� �޴� �̺�Ʈ ��ü

  while(1) {
    DWORD dwHandleSignaled = WSAWaitForMutipleEvents(
     2, EventToWaitFor, FALSE, INFINITE, TRUE);

  //� �̺�Ʈ�� �߻��Ѱǰ�?
   switch(dwHandleSignaled) {
   case WSA_WAIT_EVENT_0:
     //Close�̺�Ʈ �߻�. ������ ����
     return 0;
   case WSA_WAIT_EVENT_0 + 1: //SocketEvent �߻�
   {
      WSANETWORKEVENTS NetEvents;
      //�׵��� �߻��� �̺�Ʈ�� ����
      int ce = WSAEnumNetworkEvents(g_hSocket, g_hEvent, &NetEvents);
      //�ϳ� �̻��� �̺�Ʈ�� ���ÿ� ���� �� ����
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

section05. WSAEventSelect ���α׷� �ۼ�

CAsyncSocket�� �����ϰ� �ڽŸ��� Socket Class �ۼ�.
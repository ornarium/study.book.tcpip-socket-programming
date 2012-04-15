ch16. ������ ����2 ���α׷���
section01. Winsock2 �Ұ�

����1.1���� ���� �������� ������ ������ �������̽��� ����Ҽ� �ֵ��� WOSA(Windows Open System Architecture)�𵨷� �ٲ�. ���� �������� ������ ����ϱ� ���� ���ø����̼� �����ڴ� WS2_32.DLL���� �����ϴ� Windows Sockets2 API(Winsock2)�� ���� �������̽��� ����ϸ� ��. �ʿ��� ��� SPI�� ��� ���۰����� ������������ ������ �߰� ����. ���� ��Ʈ��ũ ���� �� ��� �ʿ��Գ� SPI������ LSP(Layered Service Provider)��� Ư�� DLL�� ���� �� ����

section02. ���Ǻ� ���Ʈ

������ ��ġ�ʴ� Ŭ���̾�Ʈ�� reject�ϱ� ���ؼ��� � ȣ��Ʈ���� �˾Ƴ��� ����, ������ Accept�� �ؾ߸� �Ѵ�. �׷��� ������ ���ʿ��� ���� ���ҽ��� ������� �ȴ�. Winsock2�� ���ο� WSAAccept�Լ��� ���Ǻ� ���Ʈ ����� �ξ� ��ġ�ʴ� Ŭ���̾�Ʈ�� ���� ���������� �޾Ƶ����� �ʰ� �Ҽ� �ִ�.

* LSP�� �ۼ��� ���, ���Ǻ� ���Ʈ�� �����ϸ� ��ġ �ʴ� ȣ��Ʈ�� ���� ������ ���� ��ȭ�� ��ɵ� �����Ҽ� ����.


SOCKET WSAAccept(
  SOCKET s,
  struct sockaddr FAR * addr,
  LPINT addrlen,
  LPCONDITIONPROC lpfnCondition,
  DWORD dwCallbackData
);
- s: ���� ������ �ڵ�
- addr: ���Ʈ�� ����� �ּ��� ����ü
- addrlen: addr����ü�� ����
- lpfnCondition: ���Ǻ� ���Ʈ �Լ�
- dwCallbackData: ���Ǻ� ���Ʈ �Լ� ȣ�� ������

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
* Caller: ������ �����ؿ��� Ŭ���̾�Ʈ ����
* Callee: ���� �ڱ��ڽſ� ���� ����
- lpCallerId: WSABUF�� buf�� SOCKADDR����ü�� ��� �־� ������ �õ��� Ŭ���̾�Ʈ �ּ� ������ ���� ����
- lpCallerData: ����������� ���� �����Ͱ� �ִٸ� WSABUF�� �޴´�
- lpQOS, lpGQOS: QoS���� ����
- lpCalleeId: ������ �ּ� ����
- lpCalleeData: ���� ������ Ŭ���̾�Ʈ�� ���� ������ ���� ����. TCP������ ���Ǻ� �Լ��� ȣ��Ǳ� ���� Client�� ���� ������ �Ϸ�ǹǷ� �ǹ� ����.
- dwCallbackData�� WSAAcceptȣ��� �������� �־��� Callback ����. ó���� ����ɸ��� �߰� �����͸� ��� ����.

WSABUF�� ���������� ������� ����ü
typedef struct _WSABUF {
  u_long len;
  char FAR * buf;
} WSABUF, FAR * LPWSABUF;


���ο� Ŭ���̾�Ʈ�� ���� �������� �����ϸ� ���� ���ť(ũ��� listen�� ������ backlog�� ����)�� ��. �̶� ����ŷ ��� ������ FD_ACCEPT �߻�. 

���Ʈ�� ���� WSAAccept�� ȣ��ǰ�, ���Ǻ� ���Ʈ �Լ����� CF_ACCEP�� �����ϸ� ���Ʈ�� �����ڵ��� �����ϰ�, CF_REJECT�� �����ϸ� ���Ӱ����� �ǹ��ϴ� WSAECONNREFUSED��� �����ڵ� �߻�. �����Ǵ��� �̷�� ���ؼ��� CF_DEFER�� ����Ͽ� �������� �̷�� ����(WSAAccept����Ǵ� �����忡�� ���ư��Ƿ�, ������ ����� ��� �����尡 Block�ɼ� ����)

���Ӵ��ť�� ����ִٸ� blocking��忡���� ������ �߻��Ҷ����� �����ְ�, non-blocking������ WSAEWOULDBLOCK���� ����


������ reject ��Ű�� ����
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
    MessageBox(g_hWnd, "���� ������", "addrr", MB_OK);
  
  return CF_REJECT;
}


section03. scatter-gather I/O

�Ǵ� vectored I/O��� �θ�

�޽����� ������ ���°� ������ �ְ�, �����ų� �ޱ� ���� �ϳ��� ���ۿ� �����ϴ� ����� ���ּ� ��Ʈ��ũ ������ �����ϴ� ���. 

WSARecv/WSARecvFrom�� ���� ���� �迭 ���

int WSASend(
  SOCKET s,
  LPWSABUF lpBuffers,
  DWORD dwBufferCount,
  LPDWORD lpNumberOfBytesSent,
  DWORD dwFlags,
  LPWSAOVERLAPPED lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

- lpBuffers, dwBufferCount: scatter-gather I/O�� ����
- lpNumberofBytesSent: �ѹ��� ������ ��
- lpOverlapped, lpCompletionRoutine: �������� I/O�� ����

�ѹ濡 ������ ����
char * pHeader;  //10����Ʈ ���
char * pContents  //30����Ʈ �޽��� ��ü

WSABUF wbuf[2];
wbuf[0].buf = pHeader;
wbuf[0].len = 10;

wbuf[1].buf = pContents;
wbuf[1].len = 30;

DWORD dwSent;
WSASend(s, wbuf, 2, &dwSent, 0, NULL, NULL);

�ѹ濡 �ޱ� ����
char * pHeader = new char[10]; //10����Ʈ ���
char * pContents = new char[30]; //30����Ʈ �޽��� ��ü

WSABUF wbuf[2];
wbuf[0].buf = pHeader;
wbuf[0].len = 10;

wbuf[1].buf = pContents;
wbuf[1].len = 30;

DWORD dwRead;
WSARecv(s, wbuf, 2, &dwRead, 0, NULL, NULL);

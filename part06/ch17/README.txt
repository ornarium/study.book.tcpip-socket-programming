ch17. ���� ���� ��Ʈ��ũ ���α׷���
section. �������� I/O
IOCP�� �����ϴµ� ����.

��������I/O�� �Է°� ����� ��ø�Ǿ� ó���ȴٴ� ��. ������ ����� ����� �Էµ����� �̷������ ���� �� ������ ����� ���� �� ���� ��ٸ��� ���������, �Է°� ���� ����� ��ø�Ǿ� ���������� ó����.

���� ���� I/O�� ù��° ������ ����̽��κ��� �о�� �����͸� �߰� ���ۿ� �����ϴ� �������� ����� ���۷� �����Ͱ� ���´ٴ� ��.  �׸���, ���� �񵿱� I/O�� ������  First Come First Serve ������ ����� �ƴ�, SCAN������ ������� ���� ��ġ�κ��� ����� ������ ��ġ�� ���� �о��.

�� ��° ������ ���� ��ũ ������ �ϰ� �ɶ�, �񵿱������� ���ÿ� ���� ��ũ�� ���� ����ó���� �Ͼ ���ÿ� I/Oó���� ��û�ϹǷ� ����̽� ���� I/O ���� ó����� ���Ѵ�

�������� I/O�� �̺�Ʈ ��ü�� �ñ׳� �ǰų� �Ϸ� �Լ��� ȣ��Ǵ� ���� ���ؼ� ����̽� ����� ������ ����� �뺸���� �� �ִ�.

���� ����¿����� ���� I/OȮ�� �Լ��� ReadFileEx�� WriteFileEx�� ����ϸ� APC(Asynchronous Procedure Call)����� ���� ���� I/O �Ϸ� �Լ�(Completion ��ƾ)�� ����� �뺸���� �� �ִ�. ���� ���ư� ����I/O������ ���� IOCP(I/O Completion Port)�� ���� ���� I/O�� ������ �� �ִ�

���� I/O����� TCP���� �������� �������� �Ź� ���� ���� �����͸� ������ �ǹǷ� ��Ʈ��ũ ���ɿ� ���� ������ �ְ� ��. �������� I/O�� ��쿡�� TCP��Ȳ�� ���� ���� WSASend�Լ��� ������ ȣ��ȴٰ� �ϴ��� ���۰������� �ϳ��� ū ����� ���� �����ְ� �Ǿ� ȿ�������� �����Ⱑ ����.

���� I/O����� TCP���� ������ Send, Receive������ ũ�Ⱑ ������ �ֱ� ������ ���� ���ÿ� �ִ� ���۰� �� ���� �Ǹ�, �ŷڼ� �ִ� ������ ������ ó���ϱ� ���� �߰��� ��Ʈ��ũ ���ϰ� �߻��� ���ִµ�, �������� I/O������ �������� ����� ���۸� WSARecvȣ��� ������ ���� �� �ְ�, �̸� ��û�ؼ� I/O�� ������ �� �ֱ� ������, ���� ���۰� �� ���� ��Ȳ�� ���� �� �ִ�(UDP������ Receive ���۸� ������� �������� ���� ���۰� �� ���� �����ϸ� ���ϼ� �ִ� ������ ����)

section02. winsock2 �������� I/O ���α׷���
���Ͽ��� �������� I/O�������ϰ� �Ϸ��� WSASocket �Լ� ȣ���  �������� �Ӽ�(WSA_FLAG_OVERLAPPED)�� �ش�. UDP�������� I/O�� SOCK_DGRAM�� �ְ� WSASendTo/WSAReceiveFrom�� ȣ��

g_hSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
if(g_hSocket == INVALID_SOCKET) {
  MessageBox(g_hWnd, "�������� ���� ���� ����", "�˸�", MB_OK);
  return ;
}

�����⸦ �Ҷ�  ȣ�� ��� ���ϵǰ� ���ۿ� �Բ� �������� I/O��û(����)�� �̷������, �Լ� ȣ��� �ش� ���ۿ� �������� ��ü�� �������� I/O���� ������ �����Ѵ�. �Ϸ� ������ ȣ�� ������� ���� �ʰ�, �Ϸ� ������ �ö����� ���۸� �ı���Ű�� �ȵȴ�.

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


WSASend �Լ��� �������� ������� ȣ���ϰ� �Ǹ� Overlapped I/O�� �߻��Ǿ��ٰ� ǥ��. WSASend ȣ���� ������ ���� �ٸ� �������� ��ü�� �־�� �ϱ� ������ Overlapped����ü�� Overlapped IO Context������ ������ ���.

typdef struct _WSAOVERLAPPED {
  DWORD Internal;
  DWORD InternalHigh;
  DWORD Offset;
  DWORD OffsetHigh;
  WSAEVENT hEvent;
} WSAOVERLAPPED, * LPWSAOVERLAPED;

hEvent�� �������� I/O�� �Ϸ�Ǵ� ���� �˸��µ� ���Ǵ� �̺�Ʈ ��ü. hEvent�� �˸� �ǰ� �������� ����� ��� �ڷ��
�̱� ������ �� �ʿ䰡 ����. 

�������� I/O���� ����� ����� �뺸�޴� ����� 2����, WSAOVERLAPPED ����ü�� �ñ׳� ����ϴ°Ͱ� �Ϸ��Լ��� �̿��ϴ� ��. lpCompletionRoutine�� NULL�� �ָ� hEvent�� ���ؼ� �ϷḦ �뺸�ϰ� lpCOmpletionRoutine�� NULL�� �ƴϸ� hEvent�� ���� �ʰ�, �Ϸ��Լ� ȣ��.

WSAWaitForMultipleEvents/WSAGetOverLappedResult �Լ��� �̿��ؼ� �Ϸᰡ �Ǿ����� Block/��Block ó���� �� �� �ִ�. �ַ� �󸶸�ŭ ������ �޾Ҵ��� �˼� �ֱ� ������, WSAGetOverlappedResult �Լ��� ���� ���. 

�Ϸ� �Լ��� �̿��ϴ� ���� WSASend�Լ��� ������ ������ lpCompletionRoutine�� Callback �Լ��� �־ ȣ��.

void CALLBACK CompletionROUTINE(
  IN DWORD dwError,
  IN DWORD cbTransferred,
  IN LPWSAOVERLAPPED lpOverlapped,
  IN DWORD dwFlags
);

section03. �̺�Ʈ ��ü�� �̿��� �������� I/O ��� �뺸�ޱ�

�������� �����Ͱ� ������ WSASend�Լ��� SOCKET_ERROR�� �����ϰ� ��. WSAGetLastError�Լ��� ȣ���ؼ� �������� Ȯ���ϴµ�, WSA_IO_PENDING�̸� ��������I/O�� ���������� ��û�Ǿ� ���������� �ǹ�.

BOOL WSAGetOverlappedResult(
  SOCKET s,
  LPWSAOVERLAPPED lpOverlapped,
  LPDWORD lpcbTransfer,
  BOOL fWait,
  LPDWORD lpdwFlags
);

- lpcbTransfer: ���� ���۵� �������� ����Ʈ ��
- fWait: TRUE�̸� Blocking/ False�̸� ��Blocking
- lpdwFlags: WSARecv/WSARecvFrom�Լ��� ȣ�������� ���� dwFlags, �������� I/O�Ϸ� ���� ����� ������ ����

�������� I/O�� ���� �����Ͱ� ��� �̸� ����� ���۸� �����ؼ� WSARecv�Լ��� ������ ȣ���ؼ� �����ص� �� ����. ���� ���� ���۸� �����ص��� ���� ���¿��� WSARecv�� �̸� ȣ���ϰ� �� ��� �����Ͱ� ���� FD_READ�̺�Ʈ�� �߻�. FD_READ�̺�Ʈ�� ó���Ҷ� WSARecv�� ȣ��(������ ����)

section04. �Ϸ� �Լ��� ���� �������� I/O ��� �뺸 �ޱ�

�Ϸ��Լ����� � �����͸� ó���߰� � �����ڵ��� ������ �Ϸ�Ǿ����� �˼��� ����. ���� �Ϸ� ��ƾ�� ����° ������ lpOverlapped�� ��������I/O�� ���� I/O���ؽ�Ʈ�� ����ؾ� ��.

typedef struct _MYOVERLAPPED : public WASOVERLAPPED {
  //WSAOVERLAPPED�� ������ �����ϹǷ� ����� �ϰų� MYOVERLLAPPED�� ù ��� �ڷ�� ��ġ��Ŵ.
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
//�ѹ��� ������ ����
} else if(nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING) {
  //�������� ������ ���۵�, ���� �Ϸ�Ǹ� �������Լ��� ȣ���.
  //�������� I/O�Լ��� ȣ���� ������� ��� ������ �� �־�� �Ϸ��Լ��� ȣ��ɼ� ����
  SleepEx(INFINITE, TRUE);
  return ;
} else {
  //������ ����
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
 //�ѹ��� �ޱ� ����
} else if(nError == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING) {
  SleepEx(INFINITE, TRUE);
  return;
} else {
  closesocket(g_hSocket);
}
delete [] myov->wsabuf.buf;
delete myov;
}

WSARecv�� ȣ���� �����尡 SleepEx�� ȣ���ϸ� ��� ��� ���°� �Ǿ� ���߰� ��. ��� ��� ���°� �� ������� APC�Լ��� APCť�� �߰��ɶ� �����¿��� �������ͼ� APC�Լ��� ȣ���ϰ� ������ ����ϰ� ��. �Ϸ��Լ��� Overlapped I/O�� �Ϸ�ɶ� APCť�� �߰��Ǳ� ������ ��� ��� ������ ������� �������� I/O�� ������Ǿ��� �� �Ϸ� �Լ��� ����.

section05. APC

������ ������� ���ο� APC�Լ��� ȣ���� �� �ֵ��� APC(Asynchronous Procedure Call)ť�� ���ϰ� ����(������ ������ Ư¡). �������� I/O�Ϸ� �Լ� ������İ� IOCP�� APC��Ŀ������ �̿��ؼ� ����.

QueueUserAPC�� APC�Լ��� ����ϰ� �Ǹ� �� ������� APC�Լ��� �ٷ� ȣ������ �ʰ� APCť�� �׾Ƶ�. �׷� �� �ش� �����尡 alertable(��� �Ǵ� �˸��� ������) ���°� �Ǹ� APC�Լ����� ť�� ���Դ� ������� ȣ���ϰ� ��(FIFO, FIrst In First Out)

DWORD QueueUserAPC(
  PAPCFUNC pfnAPC,
  HANDLE hThread,
  ULONG_PTR dwData
);

- pfnAPC: �ش� �������� APCť�� pfnAPC�Լ� �߰�
- dwData: APC�Լ��� �Ķ���ͷ� dwData�� ���

�����尡 ��� �����°� �Ǹ� ȣ��Ǵ� APC�Լ��� ������ �Ʒ��� ����
VOID CALLBACK APCProc(
  ULONG_PTR dwParam
);

�����尡 alertable wait ����, ��� ��� ���°� �� �� �ִ� �Լ����� SleepEx, SignalObjectAndWait, WaitForSingleObjectex, WaitForMutipleObjectsEx, MsgWaitForMutipleObjectsEx ���� �ִ�.
������ �Ϸ���� �������� I/O������ ���� ���� �����Լ��� WPUQueueAPC�� �̿��� �� �Ϸ��ƾ�� APCť�� �߰��� �������� �ϷḦ �뺸. WPUQueueApc�� ���ؼ��� �������� ���� ���� ������ ���õ� �Լ��̸� ������ ������ QueueUserApc�� �����.

** QueueUserAPC: undeclared identifier ������ ������ Visual C++�� ���̺귯���� �ֽ���  �ƴϱ� ������ �߻��ϴ� ������, Platform SDK�� ��ġ�ѵ� Tools->Options->Directories���� Include files���� ���丮 ����.





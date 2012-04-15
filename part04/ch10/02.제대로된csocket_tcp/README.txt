����ε� MFC CSocket

GET ����� �̿��� ������ �� ���� HTML�ҽ� ���� ����.

��ӹ��� MFC Ŭ�������� �̺�Ʈ Overriding
- �ҽ�â���� Ctrl+w�� ClassWizard ����
- ���� MFC Ŭ�����κ��� Overriding �ϰ� ���� Message(�̺�Ʈ)�� ������,
�ش� Message�� �����ϰ� Add Functino���ش�.


CSocket�� �� messsage�� overriding�� �ؼ� Accept�� �ڵ������� ó���ϰ�
Clientsocket�� ListenSocket�� ���⵵�� ó��.

<< CSocket�� ����, �Ѱ�>>
CSocket::Send�Լ��� ���������� ������ ���� CSocket::SendCheck�� ��� üũ�ϸ�, ��� �����͸� ��������.
CAsyncSocket�� �������� ������, WSAEWOULDBLOCK�� �߻��Ѵ�. �̰��� Receive�� ��������

Connect�� ������ �ɶ����� Block�Ǳ� ������, ��ٷ��߸� �Ѵ�. �ʹ� ���� ��ٸ� ���� OnMessagePendding�� Overriding�Ͽ� Ÿ�Ӿƿ��� �����Ѵ�.

1. Ÿ�Ӿƿ� �����Լ�
BOOL CClientSocket::ConnectTimeout(LPCSTR lpszHostAddresss, DWORD dwTimeout) {
  BOOL ret;
  //AfxGetMainWnd()�� ����Ͽ� ����â�� ������ �ּҸ� ��´�.
  AfxGetMainWnd()->SetTimer(65, dwTimeout, NULL);
  ret = Connect(lpszHostAddress, nHostPort);
  AfxGetMainWnd()->KillTimer(65);
  return ret;
}

2. OnMessagePending overriding
BOOL CClientSocket::OnMessagePending() {
  MSG Message; 
  //PeekMessage�� �ֱٿ� ���� �޽����� �����Ѵ�
  if(::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE)) {
    if(Message.wParam == 65) {
      //PM_REMOVE�� �޽��� ť���� �����.
      ::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
      //��ŷ ȣ���� ���
      CancelBlockingCall();
      AfxGetMainWnd()->KillTimer(65);
    }
  }

  return CSocket::OnMessagePending();
}
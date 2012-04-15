제대로된 MFC CSocket

GET 명령을 이용한 간단한 웹 서버 HTML소스 보기 구현.

상속받은 MFC 클래스에서 이벤트 Overriding
- 소스창에서 Ctrl+w로 ClassWizard 띄우기
- 상위 MFC 클래스로부터 Overriding 하고 싶은 Message(이벤트)가 있으면,
해당 Message를 선택하고 Add Functino해준다.


CSocket의 각 messsage를 overriding을 해서 Accept를 자동적으로 처리하고
Clientsocket을 ListenSocket에 숨기도록 처리.

<< CSocket의 장점, 한계>>
CSocket::Send함수는 내부적으로 루프를 돌며 CSocket::SendCheck가 계속 체크하며, 모든 데이터를 내보낸다.
CAsyncSocket은 보낼수가 없을때, WSAEWOULDBLOCK가 발생한다. 이것은 Receive도 마찬가지

Connect도 접속이 될때까지 Block되기 때문에, 기다려야만 한다. 너무 많이 기다릴 때는 OnMessagePendding를 Overriding하여 타임아웃을 설정한다.

1. 타임아웃 설정함수
BOOL CClientSocket::ConnectTimeout(LPCSTR lpszHostAddresss, DWORD dwTimeout) {
  BOOL ret;
  //AfxGetMainWnd()를 사용하여 메인창의 포인터 주소를 얻는다.
  AfxGetMainWnd()->SetTimer(65, dwTimeout, NULL);
  ret = Connect(lpszHostAddress, nHostPort);
  AfxGetMainWnd()->KillTimer(65);
  return ret;
}

2. OnMessagePending overriding
BOOL CClientSocket::OnMessagePending() {
  MSG Message; 
  //PeekMessage는 최근에 받은 메시지를 조사한다
  if(::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE)) {
    if(Message.wParam == 65) {
      //PM_REMOVE로 메시지 큐에서 지운다.
      ::PeekMessage(&Message, NULL, WM_TIMER, WM_TIMER, PM_REMOVE);
      //블럭킹 호출을 취소
      CancelBlockingCall();
      AfxGetMainWnd()->KillTimer(65);
    }
  }

  return CSocket::OnMessagePending();
}
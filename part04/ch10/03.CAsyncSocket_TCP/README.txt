CAsyncSocket TCP

 * CAsyncSocket은 비동기 소켓이다. 
Connect함수를 호출하며 0을 반환. 소켓이 성공했거나 실패하면 OnConnect로 반환된다. GetLastError로 확인해보면 WSAEWOULDBLOCK가 나오게 된다.

1. 접속
if(m_Client.Connect("localhost", 6565) == 0) {
  if(m_Client.GetLastError() != WSAWOULDBLOCK) {
    AfxMessageBox("socket connect failed!");
    return ;
  }
}

2. OnConnect 처리
void CMySocketSocket::OnEonnect(int nErrorCode) {
  if(nErrorCode == 0)
    AfxMessageBox("connect success");
  else
    AfxMessageBox("connect fail");
  CAsyncSocket::OnConnect(nErrorCOde);
}

Send시에도 500바이트 정도는 보통 한번에 나가지만, 그 이상이 되면 한번에 보낼수 없다(물론 500바이트라는 것도 경험적이며, 네트워크 상태에 따라 다르다)

 *  OnReceive에서는 Receive함수를 한 번만 호출한다
윈도우 TCP스택 내부에서 새로운 데이터가 도착하면 어플에 네트워크 이벤트로 FD_READ를 받게 되는데, CSocket과 CAsyncSocket은 OnReceive함수를 호출하게 된다. 그런데 이 FD_READ에 여러번의 Receive함수를 호출하면 안된다. FD_READ이벤트를 잃거나, 발생하지 않을수가 있다. 꼭 리턴값을 확인해서 SOCKET_ERROR가 리턴되면 CAsync::GetLastError를 호출해야 한다. 오류값이 WSAEWOULDBLOCK이면 현재 네트워크 리소스가 없다는 뜻이므로, 소켓을 close하지 않고, 그냥 리턴하면 다시 읽을 수 있을때 OnReceive가 호출된다.

 *  CAsyncSocket에서는 OnSend의 의미를 제대로 파악해야 한다.
CSocket에서는 데이터를 보낼수 없으면 WSAEWOULDBLOCK로 확인해서 "알아서"처리한다.
하지만, CAsyncSocket::Send는 그냥 Winsock의 send로 구현되어 있어서(Receive역시 recv로 구현) Send나 Receive호출시 WSAEWOULDBLOCK오류가 발생할 수 있다.

OnSend는 데이터를 보내고 나서 다시 데이터를 보낼수 있는 시점이 되었을때 호출되는 함수이다(역시, 데이터를 전부 못보낼수도 있기 때문에, 실제 보낸 데이터 양을 리턴하게 된다)

OnSend를 제대로 구현하지 않으면 전송도중 연결이 끊기거나 멈추는 문제가 발생할 수 있다

 * 종료할때 ShutDown으로..
Close로 종료할수 있으나, 내부 버퍼 데이터등을 처리한뒤에 닫아준다. 이런 저치를 gracefull close라고 부른다

 * AfxGetMainWnd, AfxGetApp;
1. AfxGetMainWnd는 어플의 메인 윈도우 주소를 CWnd*형식으로 리턴하는 기능 수행.
CWinApp의 멤버변수 CWnd * m_pMainWnd를 리턴.

2. AfxGetApp은 CWinApp 객체 주소를 리턴.

 * 교차 참조를 조심할것
AfxGetMainWnd를 사용하기 위해서 Dlg.h 를 include해야 하는데, 헤더파일에서 include해버리면, 교차 참조가 일어나 컴파일 단계에서 에러나게 된다. 정의부에서 include해주던가 class 키워드로 미리 선언을 해둔뒤 써야 한다. 안그러면, 찾을수 없다고 에러 발생.
section07. 프로세스를 사용한 다중 접속 서버 모델

유닉스/리눅스의 프로세스를 사용한 다중 접속 서버를 우니도우로 포팅하는 경우 다중 스레드 모델로 바꾸어야 한다. 하지만 그럴 여유나 시간이 없을 경우 우니도우에서도 프로세스를 생성하는 다중 접속 서버 모델로 만든다.
프로세스를 사용한 다중 접속 서버 모델에 관한 기술 문서로 MSDN: BUG: Socket Inheritance on Different Windows Platforms Q150523문서를 제공

억셉트된 소켓핸들을 DuplicateHandle이라는 Win32 API를 이용해서 복사본을 만든후 자식 프로세스를 생성, 프로세스에게 전달하는 방법. 기술문서에는 윈속2 버전에서 개발하는 것이라면 socket과 DuplicateHandle을 사용하기 보다 WSASocket과 WSADuplicateSocket을 사용하도록 권고하고 있음.

사용 빈도가 극히 드문 방식이라 예제는 없음

//윈속 서버가 리슨중
//클라이언트로부터 억셉트된 소켓을 자식 프로세스로 넘겨줄수 있도록
//DuplicateHandle로 삼속가능한 소켓 핸들을 복사해서
//자식 프로세스를 생성할때 인자로 해당 소켓의 핸들 값을 넘겨줌
OrigSock = accept(listen_socket, (struct sockaddr *)&from, &fromlen);

if(OrigSock == INVALID_SOCKET) {
  fprintf(stderr, "accept failed %d\n", GetLastError());
  return -1;
} else
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  char argbuf[256];

  memset(&si, 0, sizeof(si));

  //상속가능한 복사체인 DuplicateSock소켓을 마련
  if(!DuplicateHandle(GetCurrentProcess(),
     (HANDLE)OrigSock,
     GetCurrentProcess(),
     (Handle*) &DuplicateSock,
     0,
     TRUE, //Inheritable
     DUPLICATE_SAME_ACCESS)) {
    frprintf(stderr, "dup error %d\n", GetLastError());
    return -1;
  }

  //자식 프로세스를 생성. 첫번째 argv[1]인자는 소켓 핸들
  wsprintf(argbuf, "child.exe %d", DuplicateSock);
  if(!CreateProcess(NULL, argbuf, NULL, NULL,
     TRUE, //inherit handle
     0, NULL, NULL, &si, &pi)) {
    fprintf(stderr, "createprocess failed %d\n", GetLastError());
    return -1;
  }

  //자식 프로세스가 소켓을 받기 전에 복사도니 소켓을 닫으면 안됨
  WaitForSingleObject(pi.hProcess, INFINITE);
}

//부모 프로세스의 원본 소켓핸들과 복사본 소켓 핸드을 꼭 닫아줘야 한다
//닫지 않으면 소켓 핸들 릭 발생
closesocket(OrigSock);
closesocket(DuplicateSock);

자식 프로세스의 경우는 아래처럼 사용가능하다
void main (int argc, char * argv[]) {
  SOCKET Sock;

  /*WSAStartup etc. 
    ...*/
  if(2 == argc) {
    Sock = atoi(argv[1]); //use Sock
  }
}
section07. ���μ����� ����� ���� ���� ���� ��

���н�/�������� ���μ����� ����� ���� ���� ������ ��ϵ���� �����ϴ� ��� ���� ������ �𵨷� �ٲپ�� �Ѵ�. ������ �׷� ������ �ð��� ���� ��� ��ϵ��쿡���� ���μ����� �����ϴ� ���� ���� ���� �𵨷� �����.
���μ����� ����� ���� ���� ���� �𵨿� ���� ��� ������ MSDN: BUG: Socket Inheritance on Different Windows Platforms Q150523������ ����

���Ʈ�� �����ڵ��� DuplicateHandle�̶�� Win32 API�� �̿��ؼ� ���纻�� ������ �ڽ� ���μ����� ����, ���μ������� �����ϴ� ���. ����������� ����2 �������� �����ϴ� ���̶�� socket�� DuplicateHandle�� ����ϱ� ���� WSASocket�� WSADuplicateSocket�� ����ϵ��� �ǰ��ϰ� ����.

��� �󵵰� ���� �幮 ����̶� ������ ����

//���� ������ ������
//Ŭ���̾�Ʈ�κ��� ���Ʈ�� ������ �ڽ� ���μ����� �Ѱ��ټ� �ֵ���
//DuplicateHandle�� ��Ӱ����� ���� �ڵ��� �����ؼ�
//�ڽ� ���μ����� �����Ҷ� ���ڷ� �ش� ������ �ڵ� ���� �Ѱ���
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

  //��Ӱ����� ����ü�� DuplicateSock������ ����
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

  //�ڽ� ���μ����� ����. ù��° argv[1]���ڴ� ���� �ڵ�
  wsprintf(argbuf, "child.exe %d", DuplicateSock);
  if(!CreateProcess(NULL, argbuf, NULL, NULL,
     TRUE, //inherit handle
     0, NULL, NULL, &si, &pi)) {
    fprintf(stderr, "createprocess failed %d\n", GetLastError());
    return -1;
  }

  //�ڽ� ���μ����� ������ �ޱ� ���� ���絵�� ������ ������ �ȵ�
  WaitForSingleObject(pi.hProcess, INFINITE);
}

//�θ� ���μ����� ���� �����ڵ�� ���纻 ���� �ڵ��� �� �ݾ���� �Ѵ�
//���� ������ ���� �ڵ� �� �߻�
closesocket(OrigSock);
closesocket(DuplicateSock);

�ڽ� ���μ����� ���� �Ʒ�ó�� ��밡���ϴ�
void main (int argc, char * argv[]) {
  SOCKET Sock;

  /*WSAStartup etc. 
    ...*/
  if(2 == argc) {
    Sock = atoi(argv[1]); //use Sock
  }
}
CAsyncSocket TCP

 * CAsyncSocket�� �񵿱� �����̴�. 
Connect�Լ��� ȣ���ϸ� 0�� ��ȯ. ������ �����߰ų� �����ϸ� OnConnect�� ��ȯ�ȴ�. GetLastError�� Ȯ���غ��� WSAEWOULDBLOCK�� ������ �ȴ�.

1. ����
if(m_Client.Connect("localhost", 6565) == 0) {
  if(m_Client.GetLastError() != WSAWOULDBLOCK) {
    AfxMessageBox("socket connect failed!");
    return ;
  }
}

2. OnConnect ó��
void CMySocketSocket::OnEonnect(int nErrorCode) {
  if(nErrorCode == 0)
    AfxMessageBox("connect success");
  else
    AfxMessageBox("connect fail");
  CAsyncSocket::OnConnect(nErrorCOde);
}

Send�ÿ��� 500����Ʈ ������ ���� �ѹ��� ��������, �� �̻��� �Ǹ� �ѹ��� ������ ����(���� 500����Ʈ��� �͵� �������̸�, ��Ʈ��ũ ���¿� ���� �ٸ���)

 *  OnReceive������ Receive�Լ��� �� ���� ȣ���Ѵ�
������ TCP���� ���ο��� ���ο� �����Ͱ� �����ϸ� ���ÿ� ��Ʈ��ũ �̺�Ʈ�� FD_READ�� �ް� �Ǵµ�, CSocket�� CAsyncSocket�� OnReceive�Լ��� ȣ���ϰ� �ȴ�. �׷��� �� FD_READ�� �������� Receive�Լ��� ȣ���ϸ� �ȵȴ�. FD_READ�̺�Ʈ�� �Ұų�, �߻����� �������� �ִ�. �� ���ϰ��� Ȯ���ؼ� SOCKET_ERROR�� ���ϵǸ� CAsync::GetLastError�� ȣ���ؾ� �Ѵ�. �������� WSAEWOULDBLOCK�̸� ���� ��Ʈ��ũ ���ҽ��� ���ٴ� ���̹Ƿ�, ������ close���� �ʰ�, �׳� �����ϸ� �ٽ� ���� �� ������ OnReceive�� ȣ��ȴ�.

 *  CAsyncSocket������ OnSend�� �ǹ̸� ����� �ľ��ؾ� �Ѵ�.
CSocket������ �����͸� ������ ������ WSAEWOULDBLOCK�� Ȯ���ؼ� "�˾Ƽ�"ó���Ѵ�.
������, CAsyncSocket::Send�� �׳� Winsock�� send�� �����Ǿ� �־(Receive���� recv�� ����) Send�� Receiveȣ��� WSAEWOULDBLOCK������ �߻��� �� �ִ�.

OnSend�� �����͸� ������ ���� �ٽ� �����͸� ������ �ִ� ������ �Ǿ����� ȣ��Ǵ� �Լ��̴�(����, �����͸� ���� ���������� �ֱ� ������, ���� ���� ������ ���� �����ϰ� �ȴ�)

OnSend�� ����� �������� ������ ���۵��� ������ ����ų� ���ߴ� ������ �߻��� �� �ִ�

 * �����Ҷ� ShutDown����..
Close�� �����Ҽ� ������, ���� ���� �����͵��� ó���ѵڿ� �ݾ��ش�. �̷� ��ġ�� gracefull close��� �θ���

 * AfxGetMainWnd, AfxGetApp;
1. AfxGetMainWnd�� ������ ���� ������ �ּҸ� CWnd*�������� �����ϴ� ��� ����.
CWinApp�� ������� CWnd * m_pMainWnd�� ����.

2. AfxGetApp�� CWinApp ��ü �ּҸ� ����.

 * ���� ������ �����Ұ�
AfxGetMainWnd�� ����ϱ� ���ؼ� Dlg.h �� include�ؾ� �ϴµ�, ������Ͽ��� include�ع�����, ���� ������ �Ͼ ������ �ܰ迡�� �������� �ȴ�. ���Ǻο��� include���ִ��� class Ű����� �̸� ������ �صе� ��� �Ѵ�. �ȱ׷���, ã���� ���ٰ� ���� �߻�.
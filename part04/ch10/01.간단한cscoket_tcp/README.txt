������ MFC CSocket ���α׷�

���ҽ� �����Ϳ��� ���� ��Ʈ�Ѱ� �̺�Ʈ�� �����Ϸ���, ����Ŭ���ϸ� �Լ������� ���´�
- Ŭ���� ���ڵ带 ���� �ʰ�

�� �ҽ� �� ���̾�α� �����͵��� �̵���  Ctrl+Tab���� �̵�

�ܺ� C�ҽ��� ���� Pre compiled Headers ���� ������ ���ų� ������ �����, Project-Settings-C/C++��-Precompiled Headers���� ������� �������� ���������ش�.

������Ʈ ������, Window Socket�� �߰������� �ʰ� ������ ���¿��� ������ �߰��ϰ��� �Ѵٸ�
1. StdAfx.h�� �߰�
#include <afxsock.h>

2. CWndApp(��ӹ���) ���� InitInstance�� ó���� �߰�
BOOL C~~App::InitInstance()
{
  if(!AfxSocketInit()) {
    AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
    return ;
}

3. Resource.h�� IDP_SOCKETS_INIT_FAILED ���� �߰�
#define IDP_SOCKETS_INIT_FAILED         103



#define IDP_SOCKETS_INIT_FAILED         103
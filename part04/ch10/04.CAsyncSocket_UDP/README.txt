MFC CAsyncSocket UDP ���α׷�

 * �����ڽ��κ��� �ؽ�Ʈ ������
CString sPort;
GetDlgItem(IDC_PORT, sPort);

 * ��Ʈ���� ������ ��ü�� ������, GetWindowText�Լ��� ����ϴ� �Ͱ� ����
CString sPort;
GetDlgItem(IDC_PORT)->GetWindowText(sPort);

 * _ttoi�Լ��� atoi(���ڿ�->����)�� TCHAR�� �Լ�

 * TCHAR
���ڿ� �Լ��� 3���� ������ ����. �⺻ c���ڿ��Լ�, �����ڵ带 ���� �Լ�, mbcs�� ���� �Լ�.
TCHAR.h ��������� �����ϸ�, TCHAR�� ���� �ִ�. MFC�� pch�� �ٷ� ���� �ִ�.

������ strcpy�� _tsccpy �� ����ϰ�,  �����ڵ�� �������Ϸ��� _UNICODE��ũ�θ� �����ϰ�, MBCS�� �Ϸ��� _MBCS��� ��ũ�η� �����Ѵ�. �ƹ��͵� �������� ������ �⺻ strcpy�� �νĵȴ�.

������ ���ĵ� SBCS�� MBCS�� char���� _UNICODE�� wchar_t(2byte) �������� �����µ�, wchar_t�� ���ڿ��տ� L�� �ٿ��� ǥ�� �Ѵ�. ���� �� ��쵵 _T��ũ�θ� ����Ͽ� SBCS(ascii)�� _UNICODE���¸� ���� ������ �� �ִ�.

_UNICODE�� _MBCS�� �����Ϸ��� project->settings->c/c++�� �� ����ó�� ���Ǻο��� ��ũ�θ� �����Ѵ�.

* CString::Format - sprintf�� ����(��Ȯ���� TCHAR������ _strprintf �Լ�)

 * ��ε�ĳ��Ʈ �޽��� �ޱ�
UDP������ ���� �� �Ʒ��� �ڵ带 �߰�
if(m_Udp.Create(_ttoi(sPort), SOCK_DGRAM)) {
  BOOL bBroadcast = TRUE;
  m_Udp.SetSockOpt(SO_BROADCAST, &bBroadcast, sizeof(BOOL));
  AfxMessageBox(_T("���� ����"));
} else {
  AfxMessageBox(_T("���� ����"));
}

 * ��ε�ĳ��Ʈ �޽��� ������
���� ���� ��Ʈ��ũ�� ��ε� ĳ��Ʈ�ϰ� �ȴ�.
m_Udp.SendTo("broadcast udp", 14, _ttoi(sRemotePort), _T("255.255.255.255"));
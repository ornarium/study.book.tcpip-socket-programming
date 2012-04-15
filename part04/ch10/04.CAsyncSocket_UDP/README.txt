MFC CAsyncSocket UDP 프로그램

 * 에딧박스로부터 텍스트 얻어오기
CString sPort;
GetDlgItem(IDC_PORT, sPort);

 * 컨트롤의 윈도우 객체를 얻은뒤, GetWindowText함수를 사용하는 것과 동일
CString sPort;
GetDlgItem(IDC_PORT)->GetWindowText(sPort);

 * _ttoi함수는 atoi(문자열->숫자)의 TCHAR형 함수

 * TCHAR
문자열 함수는 3가지 버전이 존재. 기본 c문자열함수, 유니코드를 위한 함수, mbcs를 위한 함수.
TCHAR.h 헤더파일을 포함하면, TCHAR를 쓸수 있다. MFC는 pch로 바로 쓸수 있다.

쓸때는 strcpy는 _tsccpy 로 사용하고,  유니코드로 컴파일하려면 _UNICODE매크로를 선언하고, MBCS로 하려면 _MBCS라는 매크로롤 정의한다. 아무것도 정의하지 않으면 기본 strcpy로 인식된다.

데이터 형식도 SBCS나 MBCS의 char형과 _UNICODE의 wchar_t(2byte) 형식으로 나뉘는데, wchar_t는 문자열앞에 L을 붙여서 표현 한다. 따라서 이 경우도 _T매크로를 사용하여 SBCS(ascii)나 _UNICODE형태를 선택 적용할 수 있다.

_UNICODE나 _MBCS를 설정하려면 project->settings->c/c++탭 의 전ㅈ처리 정의부에서 매크로를 수정한다.

* CString::Format - sprintf와 유사(정확히는 TCHAR버전의 _strprintf 함수)

 * 브로드캐스트 메시지 받기
UDP소켓을 생성 후 아래의 코드를 추가
if(m_Udp.Create(_ttoi(sPort), SOCK_DGRAM)) {
  BOOL bBroadcast = TRUE;
  m_Udp.SetSockOpt(SO_BROADCAST, &bBroadcast, sizeof(BOOL));
  AfxMessageBox(_T("생성 성공"));
} else {
  AfxMessageBox(_T("생성 실패"));
}

 * 브로드캐스트 메시지 보내기
로컬 서브 네트워크에 브로드 캐스트하게 된다.
m_Udp.SendTo("broadcast udp", 14, _ttoi(sRemotePort), _T("255.255.255.255"));
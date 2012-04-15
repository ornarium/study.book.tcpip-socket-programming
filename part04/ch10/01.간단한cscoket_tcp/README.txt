간단한 MFC CSocket 프로그램

리소스 에디터에서 쉽게 컨트롤과 이벤트를 연결하려면, 더블클릭하면 함수생성이 나온다
- 클래스 위자드를 열지 않고

각 소스 및 다이얼로그 에디터들을 이동시  Ctrl+Tab으로 이동

외부 C소스를 사용시 Pre compiled Headers 관련 에러가 나거나 문제가 생기면, Project-Settings-C/C++탭-Precompiled Headers에서 사용하지 않음으로 해제시켜준다.

프로젝트 생성시, Window Socket을 추가해주지 않고 진행한 상태에서 소켓을 추가하고자 한다면
1. StdAfx.h에 추가
#include <afxsock.h>

2. CWndApp(상속받은) 쪽의 InitInstance의 처음에 추가
BOOL C~~App::InitInstance()
{
  if(!AfxSocketInit()) {
    AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
    return ;
}

3. Resource.h에 IDP_SOCKETS_INIT_FAILED 선언 추가
#define IDP_SOCKETS_INIT_FAILED         103



#define IDP_SOCKETS_INIT_FAILED         103
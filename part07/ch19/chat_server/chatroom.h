#define MAX_SOCKET_BUF_SIZE 1024
#define NICKNAME_SIZE 12 //닉네임 문자열 크기

//사용자의 상태 값 정의
#define USER_EMPTY       0 // 비어있는 데이터
#define USER_IN_LOBBY    1 //대기실에 있음
#define USER_IN_CHATROOM 2 //대화방에 있음

typedef struct _userData {
        int fd; //파일 지시자
        char nickName[NICKNAME_SIZE+1]; //대화명
        int roomNum; //대화방 번호
        int state; //현재 상태
} user;

#define ROOMNAME_SIZE   100 //대화방 제목 문자열의 크기
#define MAXUSER_IN_ROOM 10  //대화방 하나에 최대 접속 가능한 사용자 수

//대화방의 상태 값 정의
#define ROOM_CLOSED 0 //대화방이 개설되어 있지 않음
#define ROOM_OPENED 1 //대화방이 개설되어 있음

typedef struct _roomData{
        int roomNum;
        char roomName[ROOMNAME_SIZE+1];
        int userIndex[MAXUSER_IN_ROOM];         //방에 참여하는 사용자의 사용자 리스트의 인덱스
        int userCount; //방에 있는 사용자 수
        int state; //상태 값
} room;

//패킷 구조: packet_type(2byte) + data_lengeth(2byte) + data(n byte)
/*
  패킷 명명 규칙
  X_Y_Z
  - X : 전송 방향. C2S(Client To Server, S2C(Server To Client)
  - Y : 패킷 성격. REQ(Request), REP(Response), SND(Send)
  - Z : 패킷의 주제. 어떠한 내용을 보낼지
  */
// 패킷 type
//Client To Server
#define C2S_REQ_LOBBY_DATA 1001
#define C2S_REQ_CREATE_ROOM 1002
#define C2S_IS_POSSIBLE_JOIN_ROOM 1003
#define C2S_REQ_ROOM_DATA 1004
#define C2S_SND_CHAT_MSG 1005
#define C2S_REQ_QUIT_ROOM 1006

//Server To Client
#define S2C_SND_CONNECT_OK 2001
#define S2C_REP_LOBBY_ROOM 2002
#define S2C_REP_LOBBY_USER 2003
#define S2C_REP_CREATE_ROOM 2004
#define S2C_REP_POSSIBLE_JOIN_ROOM 2005
#define S2C_REP_IMPOSSIBLE_JOIN_ROOM 2006
#define S2C_ADD_USER_IN_ROOM 2007
#define S2C_REJECT_JOIN_ROOM 2008
#define S2C_SND_DELUSER_IN_LOBBY 2009
#define S2C_SND_CHAT_MSG 2010
#define S2C_SND_DELUSER_IN_ROOM 2011
#define S2C_REP_QUIT_ROOM 2012


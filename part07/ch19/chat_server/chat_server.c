#define _GNU_SOURCE

#define MAX_USER_COUNT 100
#define MAX_ROOM_COUNT 200

#include "chatroom.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

user userList[MAX_USER_COUNT]; //사용자 리스트
room roomList[MAX_ROOM_COUNT]; //대화방 리스트

int listen_sockfd;


///////////////////////////////chat logic begin

void sendPacket(int fd, int16_t code, char * buf) {
        int16_t buflen = 0;
        if(buf)
                buflen = strlen(buf);
        
        if(code == S2C_REP_POSSIBLE_JOIN_ROOM)
                printf("possible value %s\n ", buf);
        
        char * buffer = (char *) malloc(4 + buflen);
        memset(buffer, 0x00, 4 + buflen);        
        /*
        write(fd, &code, 2);
        write(fd, &buflen, 2);
        if(buflen > 0)
                write(fd, buf, buflen);
        */
        

        /*
        memcpy(&code, &buffer[0], 2);
        memcpy(&buflen, &buffer[2], 2);
        if(buflen > 0)
                memcpy(buf, &buffer[4], buflen);
                */

        memcpy(&buffer[0], &code,  2);
        memcpy(&buffer[2], &buflen, 2);
        if(buflen > 0)
                memcpy(&buffer[4], buf, buflen);
        
        
        write(fd, buffer, 4 + buflen);
}

void disconnect(int fd) {
         close(fd);
}

void newUserSetting(int fd, char * nickName) {
        userList[fd].fd = fd; //파일 지시자
        strcpy(userList[fd].nickName, nickName); //닉네임
        userList[fd].state = USER_IN_LOBBY; //대기실
}

void sendRoomData(int fd) {
        int i = 0;
        char buf[128];

        for(i = 0; i < MAX_ROOM_COUNT; ++i) {
                //개설된 대화방일 경우
                if(roomList[i].state == ROOM_OPENED) {
                        memset(&buf, 0x00, 128);
                        sprintf(buf, "%d|%s|%d",
                                roomList[i].roomNum,
                                roomList[i].roomName,
                                roomList[i].userCount);
                        printf("%s\n", buf);
                        
                        sendPacket(fd, S2C_REP_LOBBY_ROOM, buf);
                }
        }
}

void sendUserData(int fd) {
        int i = 0;

        for(i = 0; i < MAX_USER_COUNT; ++i) {
                //데이터를 보내줘야 하는 클라이언트를 제외한 대기실에 있는 클라이언트일 경우
                if(fd != i && userList[i].state == USER_IN_LOBBY) {
                        sendPacket(fd, S2C_REP_LOBBY_USER, userList[i].nickName);
                }
        }
        
}

void sendNewuserInLobby(int fd) {
        int i = 0;

        for(i = 0; i < MAX_USER_COUNT; ++i) {
                //대기실 사용자
                if(fd != i && userList[i].state == USER_IN_LOBBY) {
                        sendPacket(i, S2C_REP_LOBBY_USER, userList[fd].nickName);
                }
        }
        
}


void resetRoom(int index) {
        roomList[index].roomNum = index+1;
        roomList[index].state = ROOM_CLOSED;
        roomList[index].userCount = 0;
}

int createRoom(char * roomName) {
        int i = 0;
        char buf[128];

        for(i = 0; i < MAX_ROOM_COUNT; ++i) {
                //비어있는(개설되지 않은) 대화방일 경우
                if(roomList[i].state == ROOM_CLOSED) {
                        resetRoom(i);
                        
                        strcpy(roomList[i].roomName, roomName);
                        roomList[i].state = ROOM_OPENED;
                        //개설된 대화방 번호를 리턴
                        return roomList[i].roomNum;
                }
        }
        return -1;
}

void sendNewRoomData(int roomNum) {
        int i = 0;
        char buf[128];

        memset(&buf, 0x00, 128);
        roomList[roomNum - 1].roomNum = roomNum;
        
        sprintf(buf, "%d|%s|%d", roomList[roomNum - 1].roomNum,
                roomList[roomNum-1].roomName, roomList[roomNum-1].userCount);

        for(i = 0; i < MAX_USER_COUNT; ++i) {
                //대기실에 있는 클라이언트일 경우
                if(userList[i].state == USER_IN_LOBBY)
                        sendPacket(userList[i].fd, S2C_REP_LOBBY_ROOM, buf);
        }
}

/* 채팅창에 들어가면서 Lobby에서 제거 */
void delUserInLobby(int fd) {
        sendPacket(fd, S2C_SND_DELUSER_IN_LOBBY, NULL);
}

/* 채팅방에 들어가면서, 기존의 사용자에게 알림 */
int joinChatRoom(int fd, int roomNum) {
        int index = roomNum - 1;
        int i;
        int checkInserted = 0;

        if(roomList[index].userCount < MAXUSER_IN_ROOM) {
                //대화방 참가 가능
                for(i = 0; i < MAXUSER_IN_ROOM; ++i) {
                        //비어있는 공간이면
                        if(checkInserted == 0 && roomList[index].userIndex[i] == 0) {
                                roomList[index].userIndex[i] = fd;
                                roomList[index].userCount++;
                                checkInserted = 1;
                        } else if(roomList[index].userIndex[i] != fd && roomList[index].userIndex[i] != 0) {
                                //기존 대화방 사용자에게 새로운 사용자를 알림
                                sendPacket(fd, S2C_ADD_USER_IN_ROOM, userList[roomList[index].userIndex[i]].nickName);
                        }
                }
                return 1;
        } else
                return -1;
}

void sendChatMsg(int fd, char * chatMsg) {
        int index = userList[fd].roomNum-1;
        int i;
        for(i = 0; i < MAXUSER_IN_ROOM; ++i) {
                //자신을 제외한 대화방에 있는 사용자들에게만
                if(roomList[index].userIndex[i] != fd &&
                   roomList[index].userIndex[i] != 0)
                        sendPacket(roomList[index].userIndex[i], S2C_SND_CHAT_MSG,chatMsg);
        }
}


void quitRoom(int fd) {
        int index = userList[fd].roomNum - 1;
        int i;
        printf("%d user Room number\n", userList[fd].roomNum);
        
        for(i = 0; i < MAXUSER_IN_ROOM; ++i) {
                //접속을 종료하려는 클라이언트를 찾음
                if(roomList[index].userIndex[i] == fd) {
                        roomList[index].userIndex[i] = 0;
                        --roomList[index].userCount;
                } else if(roomList[index].userIndex[i] != fd &&
                        roomList[index].userIndex[i] != 0) {
                        sendPacket(roomList[index].userIndex[i],
                                   S2C_SND_DELUSER_IN_ROOM, userList[fd].nickName);
                }
        }
        printf("quit room - userCount %d\n", roomList[i].userCount);
        //접속 사용자가 없을 경우 대화방 삭제
        if(roomList[index].userCount <= 0) {
                printf("접속 사용자 없음, %d 대화방 폭파\n", index);
                
                resetRoom(index);
        }

        printf("quit room - roomState %d\n", roomList[index].state);
}

///////////////////////////////chat logic ends

void do_sigio(int signum) {
        
}

void do_sigpipe(int signum) {
        
}

//시그널 핸들러를 등록
//SIGIO가 발생했을 경우 RTS오버플로우를 처리하기 위한 핸들러 실행
int init_signal_handler() {
        struct sigaction sigact, pipe_sig;
        
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags = SA_SIGINFO;
        sigact.sa_restorer = NULL;
        sigact.sa_handler = do_sigio;
        sigemptyset(&pipe_sig.sa_mask);
        pipe_sig.sa_handler = do_sigpipe;
        pipe_sig.sa_flags = 0;

        //SIGIO 핸들러 등록
        if(sigaction(SIGIO, &sigact, NULL) < 0) {
                printf("sigio handler error");
                return -1;
        }

        if(sigaction(SIGPIPE, &pipe_sig, NULL) < 0) {
                puts("signal(SIGPIPE) error\n");
                return -2;
        }
}

int setup_sigio(int fd) {
        if(fcntl(fd, F_SETFL, O_RDWR|O_NONBLOCK|O_ASYNC) < 0) {
                printf("Nonblocking error \n");
                return -1;
        }

        if(fcntl(fd, F_SETSIG, SIGRTMIN) < 0) {
                printf("Couldn't set signal %d on %d\n", SIGRTMIN, fd);
                return -1;
        }

        if(fcntl(fd, F_SETOWN, getpid()) < 0) {
                printf("Couldn`t set owner %d on %d\n", getpid(), fd);
                return -1;
        }

        return 0;
}

//리슨 소켓을 생성하고 RTS에 대응
//일반적인 socket->bind->listen 과정
int get_listener_fd(int port) {
        int clilen;
        int state;
        
        struct sockaddr_in serveraddr;
        clilen = sizeof(serveraddr);

        //TCP소켓을 생성
        listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(listen_sockfd < 0) {
                printf("Socket create error\n");
                return -1;
        }

        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(port);

        int optvalue = 1;
        int optlen = sizeof(optvalue);
        setsockopt(listen_sockfd, SOL_SOCKET, SO_REUSEADDR, &optvalue, optlen);

        //바인딩
        state = bind(listen_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if(state < 0) {
                printf("Bind error\n");
                return -1;
        }

        state = listen(listen_sockfd, 5);
        if(state < 0) {
                printf("Listen error\n");
                return -1;
        }

        printf("Listen Socket[%d]이 생성되었습니다.\n", listen_sockfd);

        //리슨 소켓에 대해서 RTS를 설정
        return setup_sigio(listen_sockfd);
}

int get_connect_fd() {
        struct sockaddr_in clientaddr;
        int clilen = sizeof(clientaddr);
        int client_sockfd = -1;
        
        if((client_sockfd = accept(listen_sockfd,
                               (struct sockaddr *) &clientaddr,
                               &clilen)) < 0) {
                printf("accept error");
                return -1;
        }
        setup_sigio(client_sockfd);

        printf("[%d] 클라이언트를 받아들였습니다\n", client_sockfd);

        sendPacket(client_sockfd, S2C_SND_CONNECT_OK, NULL);
        
        return 0;
}

int procData(int fd) {
        char buf[MAX_SOCKET_BUF_SIZE] = {0x00, };
        int code = 0;
        int size = 0;

        int roomNum = 0;

        if(recv(fd, buf, 2, 0) <= 0) {
                disconnect(fd);
        } else {

                //code 복사
                memcpy(&code, &buf[0], sizeof(code));
                memset(&buf, 0x00, sizeof(code));
                
                //데이터 길이 읽어오기
                recv(fd, buf, 2, 0);
                memcpy(&size, &buf[0], sizeof(size));                
                memset(&buf, 0x00, sizeof(size));

                if(size > 0)
                        recv(fd, buf, size, 0);
                
                printf("code = %d, size = %d, data = %s\n", code, size, buf);

                switch(code) {
                case C2S_REQ_LOBBY_DATA:
                        printf("닉네임 : %s 클라이언트가 대기실에 들어옴\n", buf);
                        newUserSetting(fd, buf); //새로운 클라이언트 추가
                        sendRoomData(fd); //대화방 목록 전송
                        sendUserData(fd); //대기실 사용자 목록 전송
                        sendNewuserInLobby(fd); //기존 사용자에게 알림
                        break;
                        
                case C2S_REQ_CREATE_ROOM:
                        printf("[%d] 클라이언트가 %s대화방 개설을 신청함.\n",fd, buf);
                        if((roomNum = createRoom(buf)) > 0) {
                                char strRoomNum[20];
                                sprintf(strRoomNum, "%d", roomNum);
                                sendPacket(fd, S2C_REP_CREATE_ROOM, strRoomNum);
                                //새로 개설된 대화방을 알려줌
                                sendNewRoomData(roomNum);
                        } else {
                                printf("방 개설 실패\n");
                                sendPacket(fd, S2C_REP_CREATE_ROOM, NULL);
                        }                                
                        break;
                        
                case C2S_IS_POSSIBLE_JOIN_ROOM:
                        if(roomList[atoi(buf)-1].userCount < MAXUSER_IN_ROOM) {
                                char b[20];
                                sprintf(b, "%d", roomList[atoi(buf)-1].roomNum);
                                printf("this is possible join room %s\n", b);
                                
                                sendPacket(fd, S2C_REP_POSSIBLE_JOIN_ROOM, b);
                        } else
                                sendPacket(fd, S2C_REP_IMPOSSIBLE_JOIN_ROOM, NULL);
                        break;

                case C2S_REQ_ROOM_DATA:
                        printf("[%d] 클라이언트가 %d번 대화방에 참가를 요청했습니다\n", fd, atoi(buf));
                        if(joinChatRoom(fd, atoi(buf) > 0)) {
                                //채팅방
                                userList[fd].state = USER_IN_CHATROOM;
                                //접속한 대화방 갱신
                                userList[fd].roomNum = atoi(buf);
                                delUserInLobby(fd);
                        } else
                                sendPacket(fd, S2C_REJECT_JOIN_ROOM, NULL);
                        
                        break;

                case C2S_SND_CHAT_MSG:
                        sendChatMsg(fd, buf);
                        break;
                        
                case C2S_REQ_QUIT_ROOM:
                        quitRoom(fd);
                        userList[fd].state = USER_IN_LOBBY; //대기실
                        userList[fd].roomNum = 0; //대화방
                        sendPacket(fd, S2C_REP_QUIT_ROOM, NULL);
                        
                        break;
                }
        }
        
        return 0;
}

int main(int argc, char *argv[])
{
        sigset_t set;
        siginfo_t si;
        
        int ret;

        if(argc < 2) {
                printf("please specify your port number\n");
                return -1;
        }
        
        //SIGRTMIN가 블록되도록 설정
        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);
        sigprocmask(SIG_BLOCK, &set, NULL);

        //시그널 핸들러를 초기화
        //SIGIO와 SIGPIPE에대한 시그널 핸들러를 등록 RTS Queue Overflow발생시 프로그램이
        //종료되지 않도록 설정
        if(init_signal_handler() < 0) {
                printf("Init Signal Handler Error\n");
                return -1;
        }
        
        //리슨 소켓을 생성
        if(get_listener_fd(atoi(argv[1])) < 0) {
                printf("Socket Create Error\n");
                return -2;
        }

        while(1) {
                printf("sigwait\n");

                //소켓으로부터 이벤트를 기다림
                ret = sigwaitinfo(&set, &si);
                if(ret == SIGRTMIN) {
                        printf("ok signal %d : %d\n", si.si_fd, si.si_code);
                        //만약 리슨 소켓에 발생한 이벤트라면
                        //accept를 호출해서 연결 소켓을 생성
                        if(si.si_fd == listen_sockfd) {
                                get_connect_fd();
                        }
                        //그렇지 않으면 연결 소켓에서의 이벤트이므로
                        //데이터를 읽어서 처리
                        else {
                                procData(si.si_fd);
                        }
                }
        }

        
        return 0;
}

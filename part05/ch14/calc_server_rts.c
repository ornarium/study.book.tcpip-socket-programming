#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SA struct socketaddr
struct cal {
        int numa;
        int numb;
        char op;
        int result;
        int error;
};

//네트워크를 통해서 읽어들인 cal 구조체를 호스트 바이트 순서에 맞도록 변경
void H2N(struct cal * acal) {
        acal->numa = htonl(acal->numa);
        acal->numb = htonl(acal->numb);
        acal->result = htonl(acal->result);
        acal->error = htonl(acal->error);
}

//write(2)를 통해서 인터넷으로 보낼때 네트워크 바이트 순서에 맞도록 변환한다.
void N2H(struct cal * acal) {
        acal->numa = ntohl(acal->numa);
        acal->numb = ntohl(acal->numb);
        acal->result = ntohl(acal->result);
        acal->error = ntohl(acal->error);
}

//인자로 주어진 파일 지정자 fd에 대해서 리얼타임 시그널(시그널 번호 SIGRTMIN)을
//발생시키도록 설정한다
int setup_sigio(int fd) {
        if(fcntl(fd, F_SETFL, O_RDWR|O_NONBLOCK|O_ASYNC) < 0) {
                printf("Couldn't setup nonblcking io %d \n", fd);
                return -1;
        }

        if(fcntl(fd, F_SETSIG, SIGRTMIN) < 0) {
                printf("couldn't set signal %d on %d\n", SIGRTMIN, fd);
                return -1;
        }

        if(fcntl(fd, F_SETOWN, getpid()) < 0) {
                printf("couldn't set owner %d on %d \n", getpid(), fd);
                return -1;
        }
        return 0;
}

//리슨 소켓을 만들기 위한 함수
//socket->listen->bind를 분리해서 따로 함수화
int make_listener_fd(int port) {
        int clilen;
        int state;
        int sockfd;
        struct sockaddr_in serveraddr;

        clilen = sizeof(serveraddr);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0) {
                printf("socket create error \n");
                //exit(0);
                return 0;
        }

        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(port);

        state = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if(state == -1) {
                perror("bind error : ");
                //exit(0);
                return 0;
        }

        if(listen(sockfd, 5) == -1) {
                perror("listen error : ");
                //exit(0);
                return 0;
        }

        return sockfd;
}

int main(int argc, char *argv[])
{
        int client_sockfd, server_sockfd;
        int clilen;
        int ret;
        int n;
        struct sockaddr_in clientaddr, serveraddr;
        struct cal mycal;
        sigset_t set;
        siginfo_t si;

        sigemptyset(&set);
        sigaddset(&set, SIGRTMIN);
        sigprocmask(SIG_BLOCK, &set, NULL);

        //리슨 소켓이 만들어 졌다면
        //setup_sigio를 이용해서 리얼타임 시그널에 대응
        server_sockfd = make_listener_fd(4445);
        if(server_sockfd == 0)
                return 1;
        
        setup_sigio(server_sockfd);

        while(1) {
                clilen = sizeof(clientaddr);
                //sigwaitinfo를 이용해서 시그널을 기다린다
                printf("signal wait \n");
                ret = sigwaitinfo(&set, &si);
                //만약, 시그널 번호가 SIGRTMIN인 시그널이 발생했다면
                //시그널이 리슨 소켓에서 발생한 것인지
                //연결 소켓에서 발생한 것인지를 확인해서 루틴을 분기시킨다.
                if(ret == SIGRTMIN) {
                        //만약, 리슨 소켓에서 시그널이 발생했다면
                        //accept 함수를 호출해서 연결 소켓을 만든다
                        if(si.si_fd == server_sockfd) {
                                if((client_sockfd = accept(server_sockfd,
                                                           (SA *)&clientaddr,
                                                           &clilen)) < 0) {
                                        perror("accept error : ");
                                        close(client_sockfd);
                                        exit(0);
                                }
                                //만들어진 연결 소켓에 리얼타임 시그널을 대응시킨다
                                setup_sigio(client_sockfd);
                                continue;
                        }

                        client_sockfd = si.si_fd;
                        if((n = read(client_sockfd, (void *) &mycal,
                                    sizeof(mycal))) <= 0) {
                                close(client_sockfd);
                                perror("read error\n");
                                continue;
                        }

                        N2H(&mycal);

                        printf("op : %c\n", mycal.op);
                        switch(mycal.op) {
                        case '+':
                                mycal.result = mycal.numa + mycal.numb;
                                mycal.error = 0;
                                break;
                        case '-':
                                mycal.result = mycal.numa - mycal.numb;
                                mycal.error = 0;
                                break;
                        case '/':
                                mycal.result = mycal.numa / mycal.numb;
                                mycal.error = 0;
                                break;
                        case '*':
                                mycal.result = mycal.numa * mycal.numb;
                                mycal.error = 0;
                                break;
                        case 'e':
                                close(client_sockfd);
                                break;
                        }
                        printf("%d %c %d = %d\n",
                               mycal.numa,
                               mycal.op,
                               mycal.numb,
                               mycal.result);
                        //연산 결과를 클라이언트에 전송한다.
                        H2N(&mycal);
                        write(client_sockfd, (void *)&mycal, sizeof(mycal));
                }
        }
        printf("end\n");
        return 0;
}

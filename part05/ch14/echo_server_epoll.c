#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define SA struct sockaddr
#define EPOLL_SIZE 20

int main(int argc, char ** argv) {
        struct sockaddr_in addr, clientaddr;
        struct eph_comm * conn;
        int sfd;
        int cfd;
        int clilen;
        int flags = 1;
        int n, i;
        int readn;
        struct epoll_event * events;

        int efd;
        char buf_in[256];

        // 이벤트 풀을 생성시킨다
        events = (struct epoll_event *)malloc(sizeof(*events) * EPOLL_SIZE);

        if((efd = epoll_create(EPOLL_SIZE)) < 0) {
                perror("epoll_create error");
                return 1;
        }

        clilen = sizeof(clientaddr);
        sfd = socket(AF_INET, SOCK_STREAM, 0);

        if(sfd == -1) {
                perror("socket error");
                close(sfd);
                return 1;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(4445);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
                close(sfd);
                return 1;
        }
        listen(sfd, 5);

        //리슨 소켓에 대해서 이벤트 설정을 하고
        //epoll_ctl을 이용해서 이벤트 풀에 추가시킨다
        events->events = EPOLLIN;
        events->data.fd = sfd; //이렇게 지정하면, 이벤트 발생시 sfd가 넘어온다
        epoll_ctl(efd, EPOLL_CTL_ADD, sfd, events);
        while(1) {
                //이벤트 풀로부터 이벤트 발생을 기다린다
                n = epoll_wait(efd, events, EPOLL_SIZE, -1);
                if(n == -1) {
                        perror("epoll wait error");
                }
                //이벤트가 발생했다면 이벤트 발생 갯수만큼 이벤트 처리
                for(i = 0; i < n; ++i) {
                        //이벤트가 리슨소켓에서 발생한 것이면
                        //accept를 호출하여 연결 소켓 생성
                        if(events[i].data.fd == sfd) {
                                printf("accept\n");
                                cfd = accept(sfd, (SA *)&clientaddr, &clilen);
                                events->events = EPOLLIN;
                                events->data.fd = cfd;
                                epoll_ctl(efd, EPOLL_CTL_ADD, cfd, events);
                        } else {
                                memset(buf_in, 0x00, 256);
                                readn = read(events[i].data.fd, buf_in, 255);
                                if(readn <= 0) {
                                        epoll_ctl(efd, EPOLL_CTL_DEL,
                                                  events[i].data.fd,
                                                  events);
                                        close(events[i].data.fd);
                                        printf("close fd %d\n", cfd);
                                } else {
                                        printf("read data %s", buf_in);
                                        write(events[i].data.fd, buf_in, 255);
                                }
                        }
                        
                }
                        
        }
}

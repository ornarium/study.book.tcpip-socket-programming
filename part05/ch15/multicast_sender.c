#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define MULTICAST_IPADDR "225.178.39.110"
#define PORT_NUM 2005
#define MAX_BUF_SIZE 255

int main(void) {
        char buf_in[MAX_BUF_SIZE];
        struct sockaddr_in multicast_addr;
        int sender_sockfd;
        int multicast_len;
        int ttl_value=16;
        int state;

        sender_sockfd = socket(PF_INET, SOCK_DGRAM, 0); //UDP 소켓 생성
        multicast_addr.sin_family = AF_INET; //멀티캐스트 그룹 IP 주소 설정
        multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_IPADDR);
        multicast_addr.sin_port = htons(PORT_NUM); //포트 번호 설정

        multicast_len = sizeof(multicast_addr);

        //TTL값 설정
        state = setsockopt(sender_sockfd, IPPROTO_IP,
                           IP_MULTICAST_TTL, (void *) &ttl_value, sizeof(ttl_value));
        if(state) {
                perror("setsockopt error : ");
                exit(0);
        }


        while(1) {
                printf("입력 : ");
                fgets(buf_in, MAX_BUF_SIZE, stdin);
                buf_in[strlen(buf_in) - 1] = '\0';
                sendto(sender_sockfd, buf_in, strlen(buf_in), 0, (struct sockaddr*)&multicast_addr, multicast_len); //멀티캐스트 그룹으로 전송
                
        }
        close(sender_sockfd);
        return 0;
}

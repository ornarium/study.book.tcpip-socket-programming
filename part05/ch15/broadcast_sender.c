#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define BROADCAST_IPADDR "255.255.255.255" // 로컬 네트워크 브로드캐스트 주소

#define PORT_NUM 2005 //포트 번호
#define MAX_BUF_SIZE 255
#define TRUE 1

int main(int argc, char *argv[])
{
        char buf_in[MAX_BUF_SIZE];
        struct sockaddr_in broadcast_addr;
        int sender_sockfd;
        int broadcast_len;
        int state;
        int broadcast_opt = TRUE;

        sender_sockfd = socket(PF_INET, SOCK_DGRAM, 0); //UDP 소켓 생성
        broadcast_addr.sin_family = AF_INET; //브로드캐스트 그룹 IP주소 설정
        broadcast_addr.sin_addr.s_addr =
                inet_addr(BROADCAST_IPADDR);
        broadcast_addr.sin_port = htons(PORT_NUM); //포트번호 설정

        broadcast_len = sizeof(broadcast_addr);

        //브로드캐스트 설정
        state = setsockopt(sender_sockfd, SOL_SOCKET, SO_BROADCAST, (void *) &broadcast_opt, sizeof(broadcast_opt));
        if(state) {
                perror("setsockopt error : ");
                exit(0);
        }

        while(1) {
                printf("입력 : ");
                fgets(buf_in, MAX_BUF_SIZE, stdin);
                buf_in[strlen(buf_in)-1] = '\0';
                sendto(sender_sockfd, buf_in, strlen(buf_in), 0,
                       (struct sockaddr *) &broadcast_addr, broadcast_len);
        }

        close(sender_sockfd);
        return 0;
}

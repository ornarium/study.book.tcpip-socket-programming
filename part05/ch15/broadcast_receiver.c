#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define MULTICAST_IPADDR "255.255.255.255"
#define PORT_NUM 2005
#define MAX_BUF_SIZE 255

int main(int argc, char ** argv) {
        char buf[MAX_BUF_SIZE];
        struct sockaddr_in receiver_addr;
        int receiver_sockfd;
        int receiver_len;
        int state, r_size;


        receiver_sockfd = socket(PF_INET, SOCK_DGRAM, 0); //UDP 소켓 생성
        receiver_addr.sin_family = AF_INET; //브로드캐스트 그룹 IP주소 설정
        receiver_addr.sin_addr.s_addr =
                htonl(INADDR_ANY);
        receiver_addr.sin_port = htons(PORT_NUM); //포트번호 설정

        receiver_len = sizeof(receiver_addr);

        state = bind(receiver_sockfd, (struct sockaddr *)&receiver_addr, receiver_len);
        if(state == -1) {
                perror("bind error : ");
                exit(0);
        }

        while((r_size = recvfrom(receiver_sockfd, buf, MAX_BUF_SIZE, 0, NULL, 0)) > 0) {
                buf[r_size] = '\0';
                printf("브로드캐스트 그룹으로 수신된 메시지 : %s\n", buf);
        }
        close(receiver_sockfd);
        return 0;
}

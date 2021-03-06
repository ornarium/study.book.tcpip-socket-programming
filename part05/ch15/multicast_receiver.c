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
        char buf[MAX_BUF_SIZE];
        struct sockaddr_in receiver_addr;
        struct ip_mreq multicast_addr;
        
        int receiver_sockfd;
        int receiver_len;
        int state, r_size;

        receiver_sockfd = socket(PF_INET, SOCK_DGRAM, 0); //UDP 소켓 생성
        receiver_addr.sin_family = AF_INET; //멀티캐스트 그룹 IP 주소 설정
        receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        receiver_addr.sin_port = htons(PORT_NUM); //포트 번호 설정

        receiver_len = sizeof(receiver_addr);

        //바인딩
        state = bind(receiver_sockfd, (struct sockaddr *)&receiver_addr, receiver_len);
        if(state == -1) {
                perror("bind error : ");
                exit(0);
        }
        multicast_addr.imr_multiaddr.s_addr = inet_addr(MULTICAST_IPADDR);
        multicast_addr.imr_interface.s_addr = htonl(INADDR_ANY); //멀티 캐스트 그룹 가입
        state = setsockopt(receiver_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                           (void *) &multicast_addr, sizeof(multicast_addr));
        
        if(state) {
                perror("setsockopt error : ");
                exit(0);
        }


        while((r_size = recvfrom(receiver_sockfd, buf, MAX_BUF_SIZE, 0, NULL, 0)) > 0) {
                buf[r_size] = '\0';
                printf("멀티 캐스트 그룹으로 수신된 메시지 : %s\n", buf);
        }
        
        close(receiver_sockfd);
        return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define MAXBUF 256

int main() {
        int ssock;
        int clen;
        struct sockaddr_in client_addr, server_addr;
        char buf[MAXBUF];

        /* 소켓 생성 */
        if((ssock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                perror("socket error : ");
                //exit(1);
                return 1;
        }

        /* 소켓에 연결하기 위한 서버 옵션 결정 */
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(3317);

        /* 소켓에 IP 주소 등의 서버 옵션 설정 */
        if(bind(ssock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("bind error : ");
                //exit(1);
                return 1;
        }

        clen = sizeof(client_addr);
        /* UDP 데이터 수신 */
        recvfrom(ssock, (void *)buf, MAXBUF, 0, (struct sockaddr *)&client_addr, &clen);

        strcpy(buf, "I miss you");

        /* UDP 데이터 전송 */
        sendto(ssock, (void *)buf, MAXBUF, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));

        close(ssock);

        return 0;
}

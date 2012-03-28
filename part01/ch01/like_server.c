#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#define MAXBUF 256

int main() {
        //소켓 디스크립터 정의
        int ssock, csock;
        int clen;
        //주소 구조체 정의
        struct sockaddr_in client_addr, server_addr;
        //클라이언트에 보내줄 문자열
        char buf[MAXBUF] = "I like you!";

        //서버 소켓 생성
        if((ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
                perror("socket error : ");
                //exit(1);
                return 1;
        }

        clen = sizeof(client_addr);

        //주소 구조체에 주소 지정
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(3317);
        
        // 사용할 포트로 3317번 포트 사용

        //bind 함수를 사용해서 서버 소켓의 주소 설정
        if(bind(ssock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("bind error : ");
                //exit(1);
                return 1;
        }

        //위에서 지정한 주소로 클라이언트 접속을 기다림
        if(listen(ssock, 8) < 0) {
                perror("listen error : ");
                //exit(1);
                return 1;
        }

        while(1) {
                //클라이언트가 접속하면 접속을 허용하고 클라이언트 소켓을 생성함
                csock = accept(ssock, (struct sockaddr *)&client_addr, &clen);

                //클라이언트로 buf에 있는 "I like you!" 문자열 전송
                if(write(csock, buf, MAXBUF) <= 0)
                        perror("write error : ");

                //클라이언트 소켓을 닫음
                close(csock);
        }
        
        return 0;
}

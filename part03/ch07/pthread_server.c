#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXBUF 256

void * do_thread(void * arg);

int main() {
        int ssock;
        int clen;
        pthread_t tid;

        struct sockaddr_in client_addr, server_addr;

        /* 소켓 생성 */
        if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket error : ");
                exit(1);
        }

        /* 서버 주소 지정 */
        clen = sizeof(client_addr);

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(3317);

        if(bind(ssock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("bind error : ");
                exit(1);
        }

        /* 연결 수신 대기 */
        if(listen(ssock, 5) < 0) {
                perror("listen error : ");
                exit(1);
        }

        //무한 루프로 클라이언트의 접속 요구 처리
        while(1) {
                /* 연결 처리 */
                csock = accept(ssock, (struct sockaddr *)&client_addr, &clen);

                /* pthread_create 함수를 이용해서 멀티 스레드 방식으로 통신 처리 */
                if(pthread_create(&tid, NULL, do_thread, (void *)csock) < 0) {
                        perror("therad create error : ");
                        exit(1);
                }
        }

        return 0;
}

void * do_thread(void * arg) {

        /* 스레드의 클라이언트 소켓 처리 */
        int csock;
        pthread_t tid;
        char buf[MAXBUF] = "I like you!";

        tid = pthread_self();
        printf("새로운 스레드 0x%x번이 생성되었습니다\n", tid);

        csock = (int)arg;
        
        while(read(csock, buf, MAXBUF) > 0) {
                if(write(csock, buf, MAXBUF) <= 0) {
                        perror("write error : ");
                        exit(1);
                }
        }
        close(csock);
        printf("0x%x번 프로세스 소켓의 연결이 종료되었음.\n", tid);
        pthread_exit(NULL);
}

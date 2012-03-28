#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define KB 1024
int main(int argc, char *argv[])
{
        int sockfd;
        int snd_size, rcv_size;
        int optlen;

        sockfd = socket(PF_INET, SOCK_STREAM, 0);
        puts("변경전 버퍼 크기\n=========");
        getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &snd_size, &optlen);
        printf("송신 (send) 버퍼 크기 : %d KB (%d)\n", snd_size/KB, snd_size);
        getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen);
        printf("수신(receive) 버퍼 크기 : %d KB (%d)\n", rcv_size/KB, rcv_size);

        snd_size = KB * 10;
        optlen = sizeof(snd_size);
        setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &snd_size, optlen);
        rcv_size = KB * 8;
        optlen = sizeof(rcv_size);
        setsocketopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, optlen);

        puts("\n변경후 버퍼 크기\n=========");
        getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &snd_size, &optlen);
        printf("송신 (send) 버퍼 크기 : %d KB (%d)\n", snd_size/KB, snd_size);
        getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcv_size, &optlen);
        printf("수신(receive) 버퍼 크기 : %d KB (%d)\n", rcv_size/KB, rcv_size);
        
        
        return 0;
}

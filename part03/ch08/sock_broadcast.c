#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define TRUE 1

int main (int argc, char ** argv) {
        int status;
        int optvalue;
        int optlen;
        int sockfd;

        sockfd = socket(PF_INET, SOCK_STREAM, 0);

        optvalue = TRUE;
        optlen = sizeof(optvalue);
        // 옵션 설정
        status = setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optvalue, optlen);

        if(status)
                puts("error :: setsocket() SO_BROADCAST");
        //옵션값 얻어오기
        status = getsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
                            &optvalue, &optlen);

        if(status)
                puts("error :: getsocket() SO_BROADCAST");
        //결과 출력
        printf("SO_BROADCAST : %s\n", optvalue == 1 ? "설정" : "미설정");

        return 0;
}

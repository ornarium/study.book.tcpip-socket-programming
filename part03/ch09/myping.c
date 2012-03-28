#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

//IP헤더의 체크섬을 구할때도 이 함수 사용
int in_cksum(u_short *p, int n);

int main(int argc, char **argv) {
        int icmp_socket;
        int ret;
        struct icmp *p, *rp;
        struct sockaddr_in addr, from;
        struct ip *ip;
        char buffer[1024];
        int sl;
        int hlen;

        //ICMP에 대응하는 RAW소켓을 생성한다
        icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if(icmp_socket < 0) {
                perror("socket error : ");
                exit(0);
        }

        memset(buffer, 0x00, 1024);

        //icmp 패킷을 만든다. seq와 id값을 가지고 응답패킷을 판단.
        p = (struct icmp *)buffer;
        p->icmp_type = ICMP_ECHO;
        p->icmp_code = 0;
        p->icmp_cksum = 0;
        // icmp의 일련번호
        p->icmp_seq = 15;
        // icmp의 아이디
        p->icmp_id = getpid();

        p->icmp_cksum = in_cksum((u_short*)p, 1000);
        memset(&addr, 0, sizeof(0));
        addr,sin_addr.s_addr = inet_addr(argv[1]);
        addr.sin_family = AF_INET;

        //icmp패킷을 상대 호스트로 전송한다
        ret = sendto(icmp_socket, p, sizeof(*p), MSG_DONWAIT, (struct sockaddr *)&addr, sizeof(addr));

        if(ret < 0)
                perror("sendto error : ");

        //상대 호스트로부터 icmp응답을 기다림
        sl = sizeof(from);
        ret = recvfrom(icmp_socket, buffer, 1024, 0,
                       (struct sockaddr *)&from, &sl);

        if(ret < 0) {
                printf("%d %d %d\n", ret, errno, EAGAIN);
                perror("recvfrom error");
        }

        //응답을 받았다면, 해당 포스트의 IP정보와 icmp패킷정보 출력
        //RAW소켓으로 IP+ICMP를 받았기 때문에 hlen만큼 건너띈다
        ip = (struct ip *)buffer;
        hlen = ip->ip_hl * 4;
        rp = (struct icmp *) (buffer + hlen);
        printf("reply from %s \n", inet_ntoa(from.sin_addr));
        printf("type : %d \n", rp->icmp_type);
        printf("code : %d \n", rp->icmp_code);
        printf("seq : %d \n", rp->icmp_seq);
        printf("id : %d \n", rp->icmp_id);        
        return 1;
}

int in_cksum(u_short *p, int n) {
        register u_short answer;
        register long sum = 0;
        u_short odd_byte = 0;

        while(n > 1) {
                sum += *p++;
                n -= 2;
        }

        if(n == 1) {
                *(u_char*)(&odd_byte) = *(u_char*)p;
                sum += odd_byte;
        }

        sum = (sum >> 16) + (sum & 0xFFFF);
        sum += (sum >> 16);

        answer = ~sum;
        
        return (answer);
}

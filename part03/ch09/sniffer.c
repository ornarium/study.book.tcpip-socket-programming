#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "packet.h"

int main() {
        int sock, ret, len;
        char buffer[65535], *data;
        struct sockaddr_in from;
        struct in_addr src, dst;
        struct ifreq ifr;
        char * interface = "eth0";
        struct ip_header_t * ip = (struct ip_header_t *)&buffer;
        struct tcp_header_t * tcp;

        sock = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
        strncpy(ifr.ifr_name, interface, strlen(interface) + 1);
        if(ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
                perror("SIOCGIFFLAGS error : ");
                exit(1);
        }

        //원래 이더넷 디바이스 드라이버는 물리 헤더의 목적지 주소가 브로드 캐스트 또는 자신의
        //주소가 아닐 경우 버려버리는데, PROMISC로 설정하면, 모드 프레임에 대해서 받아들임
        ifr.ifr_flags |= IFF_PROMISC;
        if(ioctl(sock, SIOCSIFFLAGS, &ifr) < 0) {
                perror("Could not set the PORMISE flag : ");
                exit(1);
        }

        len = sizeof(from);

        while(1) {
                ret = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &len);
                printf("receive data = %d\n", ret);
                if(ip->protocol != PROTO_TCP)
                        continue;

                printf("===== IP Header =====\n");
                src.s_addr = ip->src_ip;
                dst.s_addr = ip->dst_ip;

                //IP주소를 10진 표기법으로 보여주기 위해 임시변수값에 넣어
                //inet_ntoa를 사용하여 점이 있는 10진 표기법으로 출력
                printf("src ip = %s\n", inet_ntoa(src));
                printf("dst ip = %s\n", inet_ntoa(dst));
                printf("total len = %d\n", ntohs(ip->length));
                printf("identification = %d \n", ntohs(ip->id));
                printf("TTL = %d\t", ip->ttl);
                printf("checksum = 0x%X\n", ntohs(ip->checksum));
                
                printf("===== TCP Header =====\n");
                tcp = (struct tcp_header_t *) (buffer + (4 * ip->hlen));
                printf("src port = %d\n", ntohs(tcp->src_port));
                printf("dst port = %d\n", ntohs(tcp->dst_port));
                printf("sequence number = 0x%X\n", ntohl(tcp->seqnum));
                printf("acknowledgment number = 0x%X\n", ntohl(tcp->acknum));
                printf("checksum = 0x%X\n", ntohs(tcp->checksum));
                
                printf("===== DATA field =====\n");
                data = (char *)(buffer + (4 * ip->hlen) + (4 * tcp->hlen));

                while(isprint(*data) || isspace(*data)) {
                        printf("%c", *data);
                        ++data;
                }

                printf("\n\n\n\n\n");
        }
        return 0;
}

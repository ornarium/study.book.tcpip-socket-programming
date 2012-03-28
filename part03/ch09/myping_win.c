#include "stdafx.h"
#include <time.h>
#include "PingClass.h"

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0

#define ICMP_MIN 8
#define DEF_PACKET_SIZE 32
#define MAX_PACKET 1024

int in_cksum(u_short *p, int n);

int main(int argc, char *argv[]) {
        int ret;
        icmp *p, *rp;
        struct sockaddr_in addr, from;
        IpHeader * ip;
        char buffer [1024];
        int sl;
        int hlen;
        int ping_pkt_size;

        WSADATA g_wsaData;
        SOCKET g_sockDgram;

        int bread;
        int timeout = 1000;

        if(WSAStartup(MAKEWORD(2, 2), &g_wsaData) != 0) {
                TRACE("WSAStartup failed: %d\n", GetLastError());
                return -1;
        }

        g_sockDgram = WSASocket(AF_INET,
                                SOCK_RAW,
                                IPPROTO_ICMP,
                                NULL,
                                0,
                                WSA_FLAG_OVERLAPPED);
        if(g_sockDgram == INVALID_SOCKET) {
                TRACE("WSASocket () failed: %d \n", WSAGetLastError());
                return -1;
        }

        bread = setsockopt(g_sockDgram, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

        if(bread == SOCKET_ERROR) {
                TRACE("failed to set recv timeout: %d\n",
                      WSAGetLastError());
                return -1;
        }

        timeout = 1000;

        bread = setsockopt(g_sockDgram, SOL_SOCKET, SO_SNDTIMEO,
                           (char*)&timeout, sizeof(timeout));
        if(bread == SOCKET_ERROR) {
                TRACE("failed to set send timeout: %d\n",
                      WSAGetLastError());
                return -1;
        }

        if(g_sockDgram < 0) {
                perror("socket error : ");
                exit(0);
        }

        memset(buffer, 0x00, 1024);

        p = (icmp *) buffer;
        p->icmp_type = ICMP_ECHO;
        p->icmp_code = 0;
        p->icmp_cksum = 0;
        p->icmp_seq = 15;
        p->icmp_id = (USHORT)GetCurrentProcessId();

        p->icmp_cksum = in_cksum((u_short *)p, 1000);
        memset(&addr, 0, sizeof(0));
        addr.sin_addr.s_addr = inet_addr(argv[1]);
        addr.sin_family = AF_INET;

        ret = sendto(g_sockDgram, buffer, sizeof(*p), 0,
                     (struct sockaddr *)&addr, sizeof(addr));
        if(ret < 0) {
                perror("sendto error : ");
        }

        sl = sizeof(from);
        ret = recvfrom(g_sockDgram, buffer, 1024, 0,
                       (struct sockaddr *) &from, &sl);
        if(ret < 0) {
                printf("%d %d \n", ret, errno);
                perror("recvfrom error : ");
        }

        ip = (IpHeader *) buffer;
        hlen = ip->h_len * 4;
        rp = (icmp *)(buffer + hlen);
        printf("reply from %s\n", inet_ntoa(from.sin_addr));
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

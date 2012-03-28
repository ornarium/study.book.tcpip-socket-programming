/*
 * protocol type list(IP헤더 Protocol 필드의 값에 해당)
 */
#define PROTO_IP   0x800
#define PROTO_ARP  0x806
#define PROTO_ICMP 0x806
#define PROTO_TCP  0x006
#define PROTO_UDP  0x011

/*
 * TCP flags list (TCP 헤더의 flags 필드 값에 해당)
 */
#define TCP_FIN  0x01
#define TCP_SYN  0x02
#define TCP_RST  0x04
#define TCP_PSH  0x08
#define TCP_ACK  0x10
#define TCP_URG  0x20
#define TCP_CTL  0x3f

// IP헤더의 구조를 가지는 구조체
struct __attribute__((packed)) ip_header_t {
        unsigned char hlen:4;
        unsigned char version:4;
        unsigned char service;
        unsigned short length;
        unsigned short id;

        unsigned short fl_offset0:4;
        unsigned char flags:3;
        unsigned short fl_offest1:1;
        unsigned short fl_offlet2:8;

        unsigned char ttl;
        unsigned char protocol;
        unsigned short checksum;
        unsigned long src_ip;
        unsigned long dst_ip;
};



//TCP 헤더의 구조를 가지는 구조체
struct __attribute__((packed)) tcp_header_t {
        unsigned short src_port;
        unsigned short dst_port;
        unsigned long seqnum;
        unsigned long acknum;

        unsigned char reserved2:4;
        unsigned char hlen:4;
        unsigned char flags:6;
        unsigned char reserved1:2;

        unsigned short window_size;
        unsigned short checksum;
        unsigned short urgent_point;
};

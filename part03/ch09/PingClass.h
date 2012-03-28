//
// ICMP 헤더 구조체
//

typedef struct _ihdr {
        BYTE icmp_type;
        BYTE icmp_code;
        USHORT icmp_cksum;
        USHORT icmp_id;
        USHORT icmp_seq;
        ULONG timestamp;
} icmp;

// IP 헤더 구조체
typedef struct iphdr {
        unsigned int h_len: 4;
        unsigned int version: 4;
        unsigned char tos;
        unsigned short total_len;
        unsigned short ident;
        unsigned short frag_and_flags;

        unsigned char ttl;
        unsigned char proto;
        unsigned short checksum;
        unsigned int sourceIP;
        unsigned int destIP;
}IpHeader;


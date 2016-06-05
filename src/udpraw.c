#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"
#include "net-checksum.h"

void printstderr(char *msg)
{
    fprintf(stderr, "%s", msg);
}

void usage()
{
    printstderr("usage: ./udpraw -d dst_ip [ -p dst_port ] [ -s src_ip ] [ -l src_port ] [ -c count ] [ -h ]\n");
    printstderr("\t -d dst_ip:\tDestination IP or hostname\n");
    printstderr("\t -p dst_port:\tDestination port where to send the packet\n");
    printstderr("\t -s src_ip:\tSource IP address\n");
    printstderr("\t -l src_port:\tSource port where from to send the packet\n");
    printstderr("\t -c count:\tHow many packets to send\n");
    printstderr("\t -h:\t\tPrint this usage message\n");
    exit(0);
}
void send_packet(char *src_ip, int src_port, char *dst_ip, int dst_port, int count)
{
    struct udphdr *udphdr;
    struct sockaddr_in address;

    unsigned char packet[28];
    struct ip *ip;

    int rawsock;

    rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    udphdr = (struct udphdr *)(packet + 20 );

    memset( (char *)udphdr, 0, 8);

    udphdr->uh_sport = htons(src_port);
    udphdr->uh_dport = htons(dst_port);
    udphdr->uh_ulen = htons(8);
    udphdr->uh_sum = 0;

    ip = (struct ip *)packet;
    memset( (char *)ip, 0, 20 );

    ip->ip_v = 4;
    ip->ip_hl = 5;
    ip->ip_p = IPPROTO_UDP;
    ip->ip_len = 40;
    ip->ip_id = htons(src_port);
    ip->ip_ttl = 60;
    ip->ip_sum = 1;
    ip->ip_src.s_addr = inet_addr(src_ip);
    ip->ip_dst.s_addr = inet_addr(dst_ip);

    printf("ip->ip_src.s_addr --> %08x\n",ip->ip_src.s_addr);
    printf("ip->ip_dst.s_addr --> %08x\n",ip->ip_dst.s_addr);

    address.sin_family = AF_INET;
    address.sin_port = htons(dst_port);
    address.sin_addr.s_addr = inet_addr(dst_ip);

    uint16_t csum1;
    /*
    The payload is 0
    Checksum is calculated from 16bit fragments of: 
    src ip, dst ip, proto, udp length and the udp payload.

    UDP length is the part of the payload that starts after IP header
    In this case Ip header is 20 bytes, so UPD paylod starts there and runs for 
    8 bytes and there is no actual payload, just the headers.
    */
    csum1 = net_checksum_tcpudp(8, 17, (unsigned char *)&ip->ip_src, packet+20);
    printf("Checksum: %04x\n", csum1);
    printf("Checksum: %04x (htoned) \n", htons(csum1));
    udphdr->uh_sum = htons(csum1);

    for(int i = 0; i < count; i++){
        printf("Sending UDP packet from %s:%d to %s:%d\n", src_ip, src_port, dst_ip, dst_port);
        int send_result = sendto( rawsock, &packet, sizeof(packet), 0x0,
            (struct sockaddr *)&address, sizeof(address));
        if(send_result > 0)
            printf("Packet sent (result: %d)\n", send_result);
        else
            printf("Failed to send packet\n");
    }
}

int main(int argc, char *argv[])
{

    // some defaults
    int dst_port = 200;
    int src_port = 200;
    int count = 1;
    int opt;
    extern char *optarg;
    char *src_ip = "10.111.103.201";
    char *dst_ip = NULL;

    if(argc < 3) // at least target is expected
        usage();

    while((opt = getopt(argc, argv, "hd:p:c:s:l:")) != -1){
        switch(opt){
            case 'd':
                dst_ip = optarg;
                break;
            case 's':
                src_ip = optarg;
                break;
            case 'p':
                dst_port = atoi(optarg); // cast to integer
                break;
            case 'l':
                src_port = atoi(optarg); // cast to integer
                break;
            case 'c':
                count = atoi(optarg); // cast to integer
                break;
            case 'h':
                usage();
                break;
            default:
                usage();
                break;
        }
    }
    if(dst_ip == NULL){
        printstderr("Target not provided.\n");
        usage();
    }
    if(src_ip == NULL){
        printf("Source IP not provided, default will be used.\n");
    }

    // send the packet
    send_packet(src_ip, src_port, dst_ip, dst_port, count);
    return 0;
}

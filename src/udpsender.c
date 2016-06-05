/*
 *
 * Send simple UDP datagrams to arbitrary destination.
 *
 */
 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h> 
#include <unistd.h>

void printstderr(char *msg)
{
    fprintf(stderr, "%s", msg);
}

void usage()
{
    printstderr("usage: ./udpsender -t target [ -p port ] [ -c count ] [ -h ]\n");
    printstderr("\t -t target:\tDestination IP or hostname\n");
    printstderr("\t -p port:\tDestination port where to send the packet\n");
    printstderr("\t -c count:\tHow many packets to send\n");
    printstderr("\t -h:\t\tPrint this usage message\n");
    exit(0);
}

void sender(char *target, int port, int count)
{
    int s;
    char *msg;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    msg = "Hi!"; // default message

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server = gethostbyname(target);

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
    (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(port);

    char *packet_string = "packet";     
    char buffer [20];
    if(count > 1){ // packet or packets depending on count
        sprintf(buffer, "%d packets", count);
        packet_string = buffer;
    }

    printf("Sending %s to %s:%d\n", packet_string, target, port);
    int i;
    for(i = 0; i < count; i++){
        sendto(s, msg, strlen(msg), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    }
}


int main(int argc, char *argv[])
{
    // some defaults
    int port = 200;
    int count = 1;

    int opt;
    extern char *optarg;
    char *target = NULL;
    
    if(argc < 3) // at least target is expected
        usage();

    while((opt = getopt(argc, argv, "ht:p:c:")) != -1){
        switch(opt){
            case 't':
                target = optarg;
                break;
            case 'p':
                port = atoi(optarg); // cast to integer
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

    if(target == NULL){
        printstderr("Target not provided.\n");
        usage();
    }

    // send the packet
    sender(target, port, count);
    return(0);
}

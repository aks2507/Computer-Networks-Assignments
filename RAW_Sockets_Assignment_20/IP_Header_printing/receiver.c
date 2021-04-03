#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>

#define h_addr h_addr_list[0]
#define PORT 51720
struct sockaddr_in source,dest;
FILE *logfile;

struct sockaddr_in serv_addr, cli_addr;
unsigned short chksum(void *b, int len) 
{    
    unsigned short *buf = b; 
    unsigned int sum = 0; 
    unsigned short result; 

    for(sum = 0;len > 1;len -= 2) 
        sum += *buf++; 
    if (len == 1) 
        sum += *(unsigned char*)buf; 
    sum = (sum >> 16) + (sum & 0xFFFF); 
    sum += (sum >> 16); 
    result = ~sum; 
    return result; 
}

void print_ip_header(char* buff)
{
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr*)buff;
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf("\n");
    printf("IP Header\n");
    printf("   |-IP Version        : %d\n",(unsigned int)iph->version);
    printf("   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf("   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf("   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf("   |-Identification    : %d\n",ntohs(iph->id));
    printf("   |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf("   |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf("   |-Checksum : %d\n",ntohs(iph->check));
    printf("   |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
    printf("   |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
}

int client_connect(int cli_port)
{
    int sfd;
    char *name = "localhost";
    
    struct hostent *server_details;
    server_details = gethostbyname(name);
    
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(cli_port);
    bcopy((char *) server_details->h_addr, (char *) &cli_addr.sin_addr.s_addr, server_details->h_length);

    if((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) 
    {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    if((setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0)
    {
        perror("setsockopt");
        exit(1);
    }

    if(bind(sfd, (struct sockaddr*)&cli_addr, sizeof(cli_addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    return sfd;
}

int main() 
{
    // logfile=fopen("log.txt","w");
    if(logfile==NULL) 
    {
        printf("Unable to create file.");
    }

    int sfd = client_connect(PORT);
    int i=0;
    while(1) 
    {
        char buff[100];
        int len = sizeof(serv_addr);
        int r;
        if((r = recvfrom(sfd, buff, 100, 0, (struct sockaddr *)&serv_addr, &len)) < 0)
        {
            perror("recvfrom");
            exit(1);
        }
        //extracting ip header
        struct iphdr *iph = (struct iphdr*)buff;

        //converting ip address from number to . format
        char buff1[100];
        struct in_addr in;
        in.s_addr = iph->saddr;
        strcpy(buff1, inet_ntoa(in));
        printf("%s from : %s\n", buff + sizeof(struct iphdr) + sizeof(struct tcphdr), buff1);
        if(i==0)
        {
            print_ip_header(buff);
            // close(fileno(logfile));
            i++;
        }
    }

    return 0;
}
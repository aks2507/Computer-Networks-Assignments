#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include <string.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stddef.h>
#include <sys/stat.h>

#define PORT1 8000
#define PORT2 8001
#define PORT3 8002
struct sockaddr_in servaddr_udp;

int tcp_connect(int port)
{
    int sfd;
    struct sockaddr_in servaddr; 

    int n, len; 
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        perror("socket"); 
        exit(1); 
    } 

    memset(&servaddr, 0, sizeof(servaddr)); 

    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    { 
        perror("Connect");
        exit(1); 
    }
    return sfd; 
}

int udp_connect(int port)
{
    int usfd; 

    if ((usfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror("socket"); 
        exit(1); 
    } 

    memset(&servaddr_udp, 0, sizeof(servaddr_udp)); 

    servaddr_udp.sin_family = AF_INET; 
    servaddr_udp.sin_port = htons(port); 
    servaddr_udp.sin_addr.s_addr = inet_addr("127.0.0.1");

    return usfd;
}

int main()
{
    while(1)
    {
        int option;
        int sfd;
        printf("Enter 0 for tcp, 1 for udp: ");
        scanf("%d",&option);
        if(option==0)
        {
            int port;
            printf("Enter port: ");
            scanf("%d",&port);
            
            sfd = tcp_connect(port);

            char buffer[1024];
            memset(buffer,0,sizeof(buffer)); 
            read(0,buffer,sizeof(buffer));
             
            write(sfd, buffer, sizeof(buffer)); 
            printf("Message from server: "); 
            read(sfd, buffer, sizeof(buffer));
            printf("%s\n",buffer); 

            // close(sfd);
        }
        else if(option==1)
        {
            int port;
            int n, len; 
            printf("Enter port: ");
            scanf("%d",&port);

            sfd = udp_connect(port);

            char buffer[1024];
            read(0,buffer,sizeof(buffer));

            sendto(sfd, (const char*)buffer, strlen(buffer), 0, (const struct sockaddr*)&servaddr_udp, sizeof(servaddr_udp)); 

            printf("Message from server: "); 
            struct sockaddr_in recvaddr;
            n = recvfrom(sfd, (char*)buffer, 1024, 0, (struct sockaddr*)&recvaddr, &len);
            char buf[len];
            inet_ntop( AF_INET, &recvaddr.sin_addr, buf, sizeof( buf ));
            printf( "%s\n", buffer );
            // close(sfd); 
        }
        else
        {   
            printf("Invalid option!! Bye!!\n");
        }
    }
    return 0;
}
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

#define PORT 8080
#define PORT2 8000

struct msg_st
{
    char name[100];
    char data[100];
} msg;

int sfd;
struct sockaddr_in ad;

int main()
{
    char name[100];
    printf("Enter your name\n");
    fgets(name, 100, stdin);
    name[strlen(name) - 1] = '\0';
    char conf[] = "Linked";
    int opt = 1;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    ad.sin_family = AF_INET;
    ad.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &ad.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sfd, (struct sockaddr *)&ad, sizeof(ad)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    int opt = 1,sfdj;
    if ((sfdj = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    ad.sin_family = AF_INET;
    ad.sin_port = htons(PORT2);
    if (inet_pton(AF_INET, "127.0.0.1", &ad.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sfdj, (struct sockaddr *)&ad, sizeof(ad)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    fd_set rfd;

    struct timeval tv;
    tv.tv_sec=5;
    tv.tv_usec=0;

    read(sfdj, &msg, sizeof(msg));

    if (strcmp(msg.data, conf))
    {
        printf("Link failed try again\n");
        return 0;
    }

    while (1)
    {
        FD_ZERO(&rfd);
        FD_SET(sfd,&rfd);
        FD_SET(0,&rfd);
        //printf("reached\n");
        int sefd=select(sfd+1,&rfd,NULL,NULL,&tv);
        if (FD_ISSET(sfd,&rfd))
        {
            read(sfd, &msg, sizeof(msg));
            printf("%s: %s\n", msg.name, msg.data);
        }
        if(FD_ISSET(0,&rfd))
        {
            char inp[100], fin[101];
            inp[0] = '\0';
            fgets(inp, 100, stdin);
            inp[strlen(inp) - 1] = '\0';
            strcpy(msg.name, name);
            strcpy(msg.data, inp);
            write(sfd, &msg, sizeof(msg));
        }
        if(FD_ISSET(sfdj,&rfd))
        {
            char* buff;
            read(sfdj,&buff,sizeof(buff));
            printf("%s\n",buff);
        }
    }
    return 0;
}
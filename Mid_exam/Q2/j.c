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

#define THRESHOLD 10
#define PORT 8000
char* name1="r1toj";
char* name2="r2toj";
char* name3="r3toj";
int passed=0;
char* selected[5];


struct msg_st
{
    char name[100];
    char data[100];
} msg;

struct student
{
    struct msg_st msg;
    int rounds;
    int sequence[3];
    int score;
};

int max(int a,int b)
{
    if(a>b)
        return a;
    return b;
}
int inet_serv_listen(int port)
{
    struct sockaddr_in servaddr;
    int sfd, len;
    int opt=1;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0))==0) 
    {
        perror("socket");
        exit(1);
    }
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit(1); 
    } 
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);
    if(bind(sfd, (struct sockaddr*)&servaddr,sizeof(servaddr))<0)
    {
        perror("bind");
        exit(1);
    } 

    if(listen(sfd,10)<0)
    {
        perror("listen");
        exit(1);
    }
    return sfd;
}
int main()
{
    mkfifo(name1,0666|O_CREAT);
    mkfifo(name2,0666|O_CREAT);
    mkfifo(name3,0666|O_CREAT);

    int fd1=open(name1,O_RDONLY);
    int fd2=open(name2,O_RDONLY);
    int fd3=open(name3,O_RDONLY);

    fd_set rfd;
    fd_set wfd;
    fd_set erfd;
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    FD_ZERO(&erfd);
    struct timeval tv;
    tv.tv_sec=5;
    tv.tv_usec=0;

    FD_SET(fd1,&rfd);
    FD_SET(fd2,&rfd);
    FD_SET(fd3,&rfd);

    while(1)
    {
        FD_SET(fd1,&rfd);
        FD_SET(fd2,&rfd);
        FD_SET(fd3,&rfd);

        int maxfd=max(fd1,fd2);
        maxfd=max(maxfd,fd3);

        int sefd=select(maxfd+1,&rfd,NULL,NULL,&tv);
        if(FD_ISSET(fd1,&rfd))
        {
            struct student buff;
            read(fd1,&buff,sizeof(int));
            if(buff.score>THRESHOLD)
            {
                selected[passed++]=buff.msg.name;
                if(passed==5)
                    break;
            }
        }
        if(FD_ISSET(fd2,&rfd))
        {
            struct student buff;
            read(fd1,&buff,sizeof(int));
            if(buff.score>THRESHOLD)
            {
                selected[passed++]=buff.msg.name;
                if(passed==5)
                    break;
            }
        }
        if(FD_ISSET(fd1,&rfd))
        {
            struct student buff;
            read(fd1,&buff,sizeof(int));
            if(buff.score>THRESHOLD)
            {
                selected[passed++]=buff.msg.name;
                if(passed==5)
                    break;
            }
        }
    }

    int sfd=inet_serv_listen(PORT);
    return 0;
}
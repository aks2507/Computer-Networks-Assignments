#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<ctype.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include <unistd.h>
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
#include <stddef.h>
#include <sys/stat.h>

#define PORT 8080
#define max_cl 10
#define NUM_SFDS 3
int sfd[NUM_SFDS];
struct sockaddr_in ad;
int nsfd[100];
struct sockaddr_in ninfo[100];
int nlen[100];
int used = 0;
int maxfd;

void encrypt(char* s)
{
	for(int i=0;i<strlen(s);i++)
	{
		s[i] = s[i]-'a'+'1';
	}
}

void lowercase(char* s)
{
    for(int i=0;i<strlen(s);i++)
    {
        if(s[i]>='A' && s[i]<='Z')
            s[i] = tolower(s[i]);
    }
}

void capitalize(char* buff)
{
    int i=0;
    while(buff[i]!='\0')
    {
        if(buff[i]>='a' && buff[i]<='z')
        {
            buff[i]=toupper(buff[i]);
        }
        i++;
    }
}

struct msg_st
{
    char name[100];
    char data[100];
} msg;

int tcp_connect(int port)
{
    int sfd;
    int opt = 1;
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(port);
    if (bind(sfd, (struct sockaddr *)&ad, sizeof(ad)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, 10) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return sfd;
}

int main()
{
    int port[NUM_SFDS];
    for(int i=0;i<NUM_SFDS;i++)
    {
        port[i] = 8080 + 5*i;
        // printf("%d    ",port[i]);
    }

    for(int i=0;i<NUM_SFDS;i++)
    {
        sfd[i] = tcp_connect(port[i]);
    }

    printf("Server up and running\n");
    char conf[] = "Linked";
    fd_set rfd;
    fd_set wfd;
    fd_set erfd;
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    FD_ZERO(&erfd);
    struct timeval tv;
    tv.tv_sec=5;
    tv.tv_usec=0;


    while (1)
    {
        //setting read fd from nsfd
        for(int i=0;i<used;i++)
            FD_SET(nsfd[i],&rfd);
        for(int i=0;i<NUM_SFDS;i++)
        {
            FD_SET(sfd[i],&rfd);
            maxfd = (sfd[i]>maxfd)?sfd[i]:maxfd;
        }
        
        for(int i=0;i<used;i++)
        {
            if(nsfd[i]>maxfd)
                maxfd=nsfd[i];
        }

        int sefd=select(maxfd+1,&rfd,NULL,NULL,&tv);
        for(int i=0;i<NUM_SFDS;i++)
        {
            if(FD_ISSET(sfd[i],&rfd))
            {
                printf("Set: %d\n",sfd[i]);
                if((nsfd[used] = accept(sfd[i], (struct sockaddr *)&ninfo[used], (socklen_t *)&nlen[used])) < 0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                strcpy(msg.data, conf);
                write(nsfd[used], &msg, sizeof(msg));
                used++;
                continue;
            }
        }
        
        for (int i = 0; i < used; i++)
        {
            if (FD_ISSET(nsfd[i],&rfd))
            {
                read(nsfd[i], &msg, sizeof(msg));
                printf("Sending to all: %s-->\n",msg.data);
                if(strcmp(msg.name,"lowercase")==0)
                {
                    lowercase(msg.data);
                    write(nsfd[i], &msg, sizeof(msg));
                }
                else if(strcmp(msg.name,"capitalize")==0)
                {
                    capitalize(msg.data);
                    write(nsfd[i], &msg, sizeof(msg));
                }
                else if(strcmp(msg.name,"encrypt")==0)
                {
                    encrypt(msg.data);
                    write(nsfd[i], &msg, sizeof(msg));
                }
                else
                {
                    for (int j = 0; j < used; j++)
                    {
                        if (j != i)
                            write(nsfd[j], &msg, sizeof(msg));
                    }
                }
            }
        }


    }
    return 0;
}

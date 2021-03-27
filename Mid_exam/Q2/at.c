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
#define max_cl 10

#define CONTROLLEN CMSG_LEN(sizeof(int))
#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;

char* path1="./fsock1";
char* path2="./fsock2";
char* path3="./fsock3";

struct sockaddr_in ad;
int nsfd[100];
struct sockaddr_in ninfo[100];
int nlen[100];
int used = 0;
int maxfd;

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

int inet_serv_listen(int port)
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

int send_fd(int fd, int fd_to_send) 
{
    struct iovec iov[1];
    char ptr[2] = {0, 0};
    int newfd;

    struct msghdr m;

    iov[0].iov_base = ptr;
    iov[0].iov_len = 2;
    m.msg_iov = iov;
    m.msg_iovlen = 1;
    m.msg_name = NULL;
    m.msg_namelen = 0;

    cmptr = malloc(CONTROLLEN);
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    cmptr->cmsg_len = CONTROLLEN;
    m.msg_control = cmptr;
    m.msg_controllen = CONTROLLEN;
    *(int *)CMSG_DATA(cmptr) = fd_to_send;

    if(sendmsg(fd, &m, 0) < 0) 
	{
        perror("sendmsg");
        return -1;
    }

    return 0;
}

int serv_listen(char* path) 
{
    int fd, len;
    struct sockaddr_un u;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0))<0) 
    {
        perror("socket");
        exit(1);
    }

    unlink(path);
    u.sun_family = AF_UNIX;
    strcpy(u.sun_path, path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(path);
    if(bind(fd, (struct sockaddr*)&u, len)<0) 
    {
        perror("bind");
        exit(1);
    }

    if(listen(fd,10)<0) 
    {
        perror("listen");
        exit(1);
    }

    return fd;
}

int main()
{
    int sfd=inet_serv_listen(PORT);

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

    int sfd1, sfd2, sfd3, nufd_r1, nufd_r2, nufd_r3;
    struct sockaddr_un r1_addr, r2_addr, r3_addr;
    int len1, len2, len3;

    len1=sizeof(r1_addr);
    len2=sizeof(r2_addr);
    len3=sizeof(r3_addr);

    sfd1=serv_listen(path1);
    sfd2=serv_listen(path2);
    sfd3=serv_listen(path3);

    if((nufd_r1=accept(sfd1, (struct sockaddr*)&r1_addr, &len1))<0)
    {
        perror("af_unix connection R1");
        exit(1); 
    }
    else
        printf("fd passing connection estd with R1\n");
    if((nufd_r2=accept(sfd2, (struct sockaddr*)&r2_addr, &len2))<0)
    {
        perror("af_unix connection R2");
        exit(1); 
    }
    else
        printf("fd passing connection estd with R2\n");
    if((nufd_r3=accept(sfd3, (struct sockaddr*)&r3_addr, &len3))<0)
    {
        perror("af_unix connection R3");
        exit(1); 
    }
    else
        printf("fd passing connection estd with R3\n");

    while (1)
    {
        //setting read fd from nsfd
        for(int i=0;i<used;i++)
            FD_SET(nsfd[i],&rfd);
        
        FD_SET(sfd,&rfd);
        
        maxfd=sfd;
        
        for(int i=0;i<used;i++)
        {
            if(nsfd[i]>maxfd)
                maxfd=nsfd[i];
        }

        int sefd=select(maxfd+1,&rfd,NULL,NULL,&tv);
        
        if(FD_ISSET(sfd,&rfd))
        {
            if((nsfd[used] = accept(sfd, (struct sockaddr *)&ninfo[used], (socklen_t *)&nlen[used])) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            strcpy(msg.data, conf);
            write(nsfd[used], &msg, sizeof(msg));
            used++;
            continue;
        }
        for (int i = 0; i < used; i++)
        {
            if (FD_ISSET(nsfd[i],&rfd))
            {
                read(nsfd[i], &msg, sizeof(msg));
                printf("%s ---> %s\n",msg.name,msg.data);
                struct student st;
                strcpy(st.msg.data,msg.data);
                strcpy(st.msg.name,msg.name);
                st.rounds=0;
                st.score=strlen(msg.data);
                write(nsfd[i],&st,sizeof(st));
                if(i%3==0)
                {
                    st.sequence[0]=1;
                    st.sequence[0]=2;
                    st.sequence[0]=3;
                    if((send_fd(nufd_r1,nsfd[i]))<0)
                    {
                        perror("send_fd_to_R1");
                        exit(1);
                    }
                    else
                        printf("Sent to R1\n");
                }
                else if(i%3==1)
                {
                    st.sequence[0]=2;
                    st.sequence[0]=3;
                    st.sequence[0]=1;
                    if((send_fd(nufd_r2,nsfd[i]))<0)
                    {
                        perror("send_fd_to_R2");
                        exit(1);
                    }
                    else
                        printf("Sent to R2\n");
                }
                else
                {
                    st.sequence[0]=3;
                    st.sequence[0]=1;
                    st.sequence[0]=2;
                    if((send_fd(nufd_r3,nsfd[i]))<0)
                    {
                        perror("send_fd_to_R3");
                        exit(1);
                    }
                    else
                        printf("Sent to R3\n");
                }
            }
        }
    }
    return 0;
}

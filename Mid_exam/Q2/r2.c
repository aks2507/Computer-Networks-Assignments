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

#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
char *path1 = "./fsock2"; // for at

char* path2 = "./fsock4"; // for R1
char* path3 = "./fscok6"; // for R3

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

int max(int a,int b)
{
    if(a>b)
        return a;
    return b;
}

int recv_fd(int fd) 
{

    struct iovec iov[1];
    char ptr[2];
    int newfd;

    struct msghdr m;

    iov[0].iov_base = ptr;
    iov[0].iov_len = sizeof(ptr);
    m.msg_iov = iov;
    m.msg_iovlen = 1;
    m.msg_name = NULL;
    m.msg_namelen = 0;

    
    cmptr = malloc(CONTROLLEN);
    m.msg_control = cmptr;
    m.msg_controllen = CONTROLLEN;

    if(recvmsg(fd, &m, 0) < 0) 
    {
        perror("recvmsg");
        return -1;
    }

    newfd = *(int *)CMSG_DATA(cmptr);

    return newfd;
}


int cli_conn(char *name, char* path) 
{
    int fd, len;
    struct sockaddr_un u, su;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    unlink(name);
    u.sun_family = AF_UNIX;
    strcpy(u.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if(bind(fd, (struct sockaddr*)&u, len)) 
    {
        perror("bind");
        exit(1);
    }

    su.sun_family = AF_UNIX;
    strcpy(su.sun_path, path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(path);
    if(connect(fd, (struct sockaddr*)&su, len) < 0) 
    {
        perror("connect");
        exit(1);
    }

    return fd;
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
    int ufd,ufdr1,ufdr3;
    char *name = "./csock2";
    char *name2 = "./csock4";
    char *name3 = "./csock6";
    ufd = cli_conn(name,path1);
    ufdr1 = cli_conn(name2,path2);
    sleep(1);
    
    char conf[] = "Linked with R2";
    fd_set rfd;
    fd_set wfd;
    fd_set erfd;
    FD_ZERO(&rfd);
    FD_ZERO(&wfd);
    FD_ZERO(&erfd);
    struct timeval tv;
    tv.tv_sec=5;
    tv.tv_usec=0;

    int sfd2, sfd3, nufd_r1, nufd_r2, nufd_r3;
    struct sockaddr_un r2_addr, r3_addr;
    int len2, len3;
    sfd2=serv_listen(path2);
    sfd3=serv_listen(path3);

    if((nufd_r2=accept(sfd2, (struct sockaddr*)&r2_addr, &len2))<0)
    {
        perror("af_unix connection R1");
        exit(1); 
    }
    else
        printf("fd passing connection estd with R1\n");
    if((nufd_r3=accept(sfd3, (struct sockaddr*)&r3_addr, &len3))<0)
    {
        perror("af_unix connection R3");
        exit(1); 
    }
    else
        printf("fd passing connection estd with R3\n");
    ufdr3 = cli_conn(name3,path3);
    while (1)
    {
        //setting read fd from nsfd
        for(int i=0;i<used;i++)
            FD_SET(nsfd[i],&rfd);
        
        FD_SET(ufd,&rfd);
        FD_SET(ufdr1,&rfd);
        FD_SET(ufdr3,&rfd);

        maxfd=ufd;
        maxfd=max(maxfd,ufdr1);
        maxfd=max(maxfd,ufdr3);
        
        for(int i=0;i<used;i++)
        {
            if(nsfd[i]>maxfd)
                maxfd=nsfd[i];
        }

        int sefd=select(maxfd+1,&rfd,NULL,NULL,&tv);
        if(FD_ISSET(ufd,&rfd))
        {
            if((nsfd[used] = recv_fd(ufd)) < 0) 
            {
                perror("recv_fd");
                exit(1);
            }
            else
                printf("Received [fd == %d] from AT\n",nsfd[used]);
            strcpy(msg.data, conf);
            write(nsfd[used], &msg, sizeof(msg));
            used++;
            continue;
        }
        if(FD_ISSET(ufdr1,&rfd))
        {
            if((nsfd[used] = recv_fd(ufdr1)) < 0) 
            {
                perror("recv_fd");
                exit(1);
            }
            else
                printf("Received [fd == %d] from R1\n",nsfd[used]);
            used++;
            continue;
        }
        if(FD_ISSET(ufdr3,&rfd))
        {
            if((nsfd[used] = recv_fd(ufdr3)) < 0) 
            {
                perror("recv_fd");
                exit(1);
            }
            else
                printf("Received [fd == %d] from R3\n",nsfd[used]);
            used++;
            continue;
        }
        for (int i = 0; i < used; i++)
        {
            if (FD_ISSET(nsfd[i],&rfd))
            {  
                struct student st;
                read(nsfd[i], &st, sizeof(st));
                printf("%s ---> %s\n",st.msg.name,st.msg.data);
                st.rounds+=1;
                st.score+=(nsfd[i]%5); // Just an example
                write(nsfd[i],&st,sizeof(st));
                if(st.rounds<=2)
                {
                    if(st.sequence[st.rounds]==1)
                    {
                        if((send_fd(nufd_r2,nsfd[i]))<0)
                        {
                            perror("send_fd_to_R1");
                            exit(1);
                        }
                        else
                            printf("Sent to R1\n");
                    }
                    else if(st.sequence[st.rounds]==3)
                    {
                        if((send_fd(nufd_r3,nsfd[i]))<0)
                        {
                            perror("send_fd_to_R3");
                            exit(1);
                        }
                        else
                            printf("Sent to R3\n");
                    }
                }
                else
                {
                    //sending to J
                    char* name="r2toj";
                    mkfifo(name,0666|O_CREAT);
                    int fd=open(name,O_WRONLY);
                    write(fd,&st,sizeof(st));
                    close(fd);
                }   
            }
        }
    }
    return 0;
}
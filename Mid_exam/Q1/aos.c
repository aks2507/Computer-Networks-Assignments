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

#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
char *path1 = "./fsock1";
char *path2 = "./fsock2";
char *path3 = "./fsock3";
char *path4 = "./fsock4";
int client_count=0;
#define PORT1 5000
#define PORT2 6000
#define PORT3 7000
#define PORT4 8000

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
    /* AF_UNIX connections started */

    struct sockaddr_un s1_addr,s2_addr,s3_addr,s4_addr;
    int len1, len2, len3, len4, nufd_s1, nufd_s2, nufd_s3, nufd_s4, sfd1, sfd2, sfd3, sfd4;
    int ufd_s1, ufd_s2, ufd_s3, ufd_s4;

    len1 = sizeof(s1_addr);
    len2 = sizeof(s2_addr);
    len3 = sizeof(s3_addr);
    len4 = sizeof(s4_addr);

    ufd_s1=serv_listen(path1);
    ufd_s2=serv_listen(path2);
    ufd_s3=serv_listen(path3);
    ufd_s4=serv_listen(path4);

    if((nufd_s1 = accept(ufd_s1, (struct sockaddr*)&s1_addr, &len1)) < 0) 
    {
        perror("accept s1");
        exit(1);
    }
    else
        printf("Connection established with S1...\n");
    
    if((nufd_s2 = accept(ufd_s2, (struct sockaddr*)&s2_addr, &len2)) < 0) 
    {
        perror("accept s2");
        exit(1);
    }
    else
        printf("Connection established with S2...\n");

    if((nufd_s3 = accept(ufd_s3, (struct sockaddr*)&s3_addr, &len3)) < 0) 
    {
        perror("accept s3");
        exit(1);
    }
    else    
        printf("Connection established with S3...\n");
    
    if((nufd_s4 = accept(ufd_s4, (struct sockaddr*)&s4_addr, &len2)) < 0) 
    {
        perror("accept s4");
        exit(1);
    }
    else
        printf("Connection established with S4...\n");

    /* AF_UNIX connections finished */

    key_t h=ftok(".",65);
 	int shmid=shmget(h,sizeof(int),IPC_CREAT|0666);
 	int* share_memory=shmat(shmid,(const void*)0,0);
    
    int s4_pid=*share_memory;
    printf("Pid of S4 is: %d\n",s4_pid);

    /* Client connections and fd passing started*/
    struct sockaddr_in inet_cliaddr1 ,inet_cliaddr2_1, inet_cliaddr2_2, inet_cliaddr3, inet_cliaddr4;
    int clen1, clen2_1 ,clen_2_2 ,clen3 ,clen4;
    int nsfd1, nsfd2_1 ,nsfd_2_2, nsfd3, nsfd4;
    sfd1 = inet_serv_listen(PORT1);

    clen1=sizeof(inet_cliaddr1);
    if((nsfd1=accept(sfd1,(struct sockaddr*)&inet_cliaddr1,&clen1))<0)
    {
        perror("inet 1 accept");
        exit(1);
    }
    else
        printf("Inet connection estd with client %d:\n",nsfd1);
    
    if(send_fd(nufd_s1,nsfd1)<0)
    {
        perror("send fd");
        exit(1);
    }
    else
        printf("Sent to S1\n");
    
    sfd2=inet_serv_listen(PORT2);
    clen2_1=sizeof(inet_cliaddr2_1);
    if((nsfd2_1=accept(sfd2,(struct sockaddr*)&inet_cliaddr2_1,&clen2_1))<0)
    {
        perror("inet 2_1 accept");
        exit(1);
    }
    else
        printf("Inet connection estd with client %d:\n",nsfd2_1);
    
    if(send_fd(nufd_s2,nsfd2_1)<0)
    {
        perror("send fd");
        exit(1);
    }
    else
        printf("Sent to S2\n");
    
    // sfd2=inet_serv_listen(PORT2);
    // clen_2_2=sizeof(inet_cliaddr2_2);
    // if((nsfd_2_2=accept(sfd2,(struct sockaddr*)&inet_cliaddr2_2,&clen_2_2))<0)
    // {
    //     perror("inet 2_2 accept");
    //     exit(1);
    // }
    // else
    //     printf("Inet connection estd with client %d:\n",nsfd_2_2);
    
    // if(send_fd(nufd_s2,nsfd_2_2)<0)
    // {
    //     perror("send fd");
    //     exit(1);
    // }
    // else
    //     printf("Sent to S2\n");

    sfd3=inet_serv_listen(PORT3);
    clen3=sizeof(inet_cliaddr3);
    if((nsfd3=accept(sfd3,(struct sockaddr*)&inet_cliaddr3,&clen3))<0)
    {
        perror("inet 3 accept");
        exit(1);
    }
    else
        printf("Inet connection estd with client %d:\n",nsfd3);
    
    if(send_fd(nufd_s3,nsfd3)<0)
    {
        perror("send fd");
        exit(1);
    }
    else
        printf("Sent to S3\n");
    
    sfd4=inet_serv_listen(PORT4);
    clen4=sizeof(inet_cliaddr4);
    if((nsfd4=accept(sfd4,(struct sockaddr*)&inet_cliaddr4,&clen4))<0)
    {
        perror("inet 4 accept");
        exit(1);
    }
    else
    {
        kill(s4_pid,SIGUSR1);
        printf("Inet connection estd with client %d:\n",nsfd4);
    }
    
    if(send_fd(nufd_s4,nsfd4)<0)
    {
        perror("send fd");
        exit(1);
    }
    else
        printf("Sent to S4\n");
    return 0;
}
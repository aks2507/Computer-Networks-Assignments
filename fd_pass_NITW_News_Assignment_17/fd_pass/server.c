#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stddef.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/un.h>
#define PORT 5000
#define PORT2 6000
#define PORT3 7000
#define CONTROLLEN CMSG_LEN(sizeof(int))

#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
char *path = "./fsock";

int send_fd(int fd, int fd_to_send) {
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

int serv_listen() 
{
    int fd, len;
    struct sockaddr_un u;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0))==0) 
    {
        perror("socket");
        exit(1);
    }
    int opt=1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
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

    if(listen(fd, 10) < 0) 
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

int inet_udp_serv_listen(int port)
{
    struct sockaddr_in servaddr;
    int usfd;
    int opt=1;
    if((usfd=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("UDP socket");
        exit(1);
    }
    if(setsockopt(usfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit(1); 
    }

    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);
    if(bind(usfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0)
    {
        perror("UDP bind");
        exit(1);
    }

    return usfd;
}

int main() 
{
    struct sockaddr_un cli_addr;
    struct sockaddr_in inet_cliaddr1,inet_cliaddr2;
    int len, ufd, nufd, sfd1, nsfd1, sfd2, nsfd2,usfd;

    len = sizeof(cli_addr);
    ufd = serv_listen();

    sfd1=inet_serv_listen(PORT);
    sfd2=inet_serv_listen(PORT2); 
    usfd=inet_udp_serv_listen(PORT3);

    if((nufd = accept(ufd, (struct sockaddr*)&cli_addr, &len)) < 0) 
    {
        perror("accept");
        exit(1);
    }

    len=sizeof(inet_cliaddr1);
    if((nsfd1=accept(sfd1,(struct sockaddr*)&inet_cliaddr1,&len))<0)
    {
        perror("inet 1 accept");
        exit(1);
    }

    if(send_fd(nufd, nsfd1) < 0) 
    {
        perror("send_fd 1");
        exit(1);
    }
    else
        printf("[nsfd == %d, %d] sent to alternate server...\n",nsfd1,sfd1);

    len=sizeof(inet_cliaddr2);
    if((nsfd2=accept(sfd2,(struct sockaddr*)&inet_cliaddr2,&len))<0)
    {
        perror("inet 2 accept");
        exit(1);
    }

    if(send_fd(nufd, nsfd2) < 0) 
    {
        perror("send_fd 2");
        exit(1);
    }
    else
        printf("[nsfd == %d, %d] sent to alternate server...\n",nsfd2,sfd2);
    
    if(send_fd(nufd, usfd) < 0) 
    {
        perror("send_fd 3");
        exit(1);
    }
    else
        printf("[usfd == %d] sent to alternate server...\n",usfd);

    unlink(path);
}
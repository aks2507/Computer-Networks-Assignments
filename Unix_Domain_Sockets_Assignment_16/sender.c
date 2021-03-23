#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>

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

    if(sendmsg(fd, &m, 0) < 0) {
        perror("sendmsg");
        return -1;
    }

    return 0;
}

int serv_listen() {
    int fd, len;
    struct sockaddr_un u;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    unlink(path);
    u.sun_family = AF_UNIX;
    strcpy(u.sun_path, path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(path);
    if(bind(fd, (struct sockaddr*)&u, len)) 
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

int main() 
{
    struct sockaddr_un cli_addr;
    int len, ufd, nufd;

    len = sizeof(cli_addr);
    ufd = serv_listen();
    if((nufd = accept(ufd, (struct sockaddr*)&cli_addr, &len)) < 0) 
    {
        perror("accept");
        exit(1);
    }

    int fd1 = open("file.txt", O_RDONLY);
    char buff[100];
    read(fd1, buff, 14);
    printf("%s\n", buff);

    if(send_fd(nufd, fd1) < 0) 
    {
        perror("send_fd");
        exit(1);
    }
    recv(nufd, buff, 100, 0);
    read(fd1, buff, 7);
    printf("%s\n", buff);
    
    unlink(path);
}
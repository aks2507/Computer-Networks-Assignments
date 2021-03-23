#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define CONTROLLEN CMSG_LEN(sizeof(int))
#define REPORTERS 3
#define BUFFSIZE 1024
static struct cmsghdr *cmptr = NULL;
char *path = "./fsock";
int doc_reporters=0;


int recv_fd(int fd) {

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

    cmptr=malloc(CONTROLLEN);
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

int cli_conn(char *name) 
{
    int fd, len;
    struct sockaddr_un u, su;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket");
        exit(1);
    }

    unlink(name);
    u.sun_family = AF_UNIX;
    strcpy(u.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if(bind(fd, (struct sockaddr*)&u, len)<0) 
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

int main()
{
	int ufd;
    char *name = "./csock";
    int reportfds[REPORTERS];
    ufd = cli_conn(name);

	while(1)
	{	
		if((reportfds[doc_reporters++] = recv_fd(ufd)) < 0) 
		{
			perror("recv_fd");
			exit(1);
		}
        char buff[BUFFSIZE];
        for(int i=0;i<doc_reporters;i++)
        {
            if(read(reportfds[i],buff,sizeof(buff)))
            {
                printf("From Reporter: %d ---> %s\n",reportfds[i],buff);
            }
        }
	}
    
	return 0;
}
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

//Related to select in reporter messages
#define REPORTERS 3
#define BUFFSIZE 1024
int nready;


//Related to passing the fd of reporters
#define CONTROLLEN CMSG_LEN(sizeof(int))
static struct cmsghdr *cmptr = NULL;
char *path = "./fsock";

int max(int a,int b)
{
	if(a>b)
		return a;
	return b;
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

int serv_listen() 
{
    int fd, len;
    struct sockaddr_un u;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0))<0) {
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

int main(int argc, char* argv[])
{
	/* SELECT SECTION FOR REPORTERS STARTS */
	fd_set rfd;
	fd_set wfd;
	fd_set exfd;

	FD_ZERO(&rfd);
	FD_ZERO(&wfd);
	FD_ZERO(&exfd);
	struct timeval tv;
	tv.tv_sec=3;
	tv.tv_usec=0;

	int maxfdp1=0;

	FILE* report_files[REPORTERS];
	int reporter_fds[REPORTERS];
	for(int i=0;i<3;i++)
	{
		if((report_files[i]=popen("./reporter","r"))==NULL)
		{
			perror("popen");
			exit(1);
		}
		reporter_fds[i]=fileno(report_files[i]);
	}
	/* SELECT SECTION FOR REPORTERS ENDS */

	/* FD PASSING TO document.c SECTION STARTS */
	struct sockaddr_un cli_addr;
    int len, passfd, npassfd;
	len=sizeof(cli_addr);
	passfd=serv_listen();
	if((npassfd=accept(passfd, (struct sockaddr*)&cli_addr, &len))<0)
	{
		perror("accept");
		exit(1);
	}
	/* FD PASSING TO document.c SECTION ENDS */

	while(1)
	{
		for(int i=0;i<3;i++)
		{
			FD_SET(reporter_fds[i],&rfd);
			maxfdp1=max(maxfdp1,reporter_fds[i]);
		}
		maxfdp1=maxfdp1+1;

		nready=select(maxfdp1,&rfd,NULL,NULL,&tv);
		
		for(int i=0;i<3;i++)
		{
			if(FD_ISSET(reporter_fds[i],&rfd))
			{
				char buff[BUFFSIZE];
				if(read(reporter_fds[i],buff,sizeof(buff)))
				{
					if(buff[0]=='/' && buff[1]=='d')
					{
						if(send_fd(npassfd, reporter_fds[i]) < 0) 
						{
							perror("send_fd");
							exit(1);
						}
						else
						{	
							printf("Send of %d successful\n",reporter_fds[i]);
							//reporter_fds[i]=0;
						}
					}
					else
						printf("From Reporter: %d ---> %s\n",reporter_fds[i],buff);
				}
			}
		}
	}
	return 0;
}
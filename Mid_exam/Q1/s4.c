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
#define PORT 8000
static struct cmsghdr *cmptr = NULL;
char *path = "./fsock4";

void handler(int signal)
{
    printf("Signal received from All in one server. Serving now...");
}
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


int cli_conn(char *name) 
{
    int fd, len;
    struct sockaddr_un u, su;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    int opt=1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
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

int main()
{
    key_t h=ftok(".",65);
 	int shmid=shmget(h,sizeof(int),IPC_CREAT|0666);
 	int* share_memory=shmat(shmid,(const void*)0,0);
    
    *share_memory=getpid();

    printf("PID is: %d\n",*share_memory);

    int ufd, rfd;
    char buffer[1024];
    char *name = "./csock4";
    ssize_t n;
    socklen_t len;
    ufd = cli_conn(name);
    struct sockaddr_in cliaddr;
    // char* message = "Hello Client";

    if((rfd = recv_fd(ufd)) < 0) 
    {
        perror("recv_fd");
        exit(1);
    }
    else
        printf("[fd==%d] accepted from main server..\n",rfd);
    
    while(1)
    {
        signal(SIGUSR1,handler);
        pid_t c;
        char* mesg = "I_m_S4!!\n";
        
        if((c==fork())<0)
        {
            perror("fork");
            exit(1);
        }
        else if(c==0) // child
        {
            while(1)
            {
                char buff[1024];
                bzero(buff, sizeof(buff)); 
                printf("Message from client: ");
                read(rfd,buff,sizeof(buff));
                printf("%s\n",buff);
                write(rfd,(const char*)mesg,sizeof(mesg));
            }
            close(rfd);
        }
        else //parent
        {
            printf("Parent Waits...\n");
        }
    }
    return 0;

}
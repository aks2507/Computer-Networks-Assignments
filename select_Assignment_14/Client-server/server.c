#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define max_cl 10
int sfd;
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

int main()
{
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
    ad.sin_port = htons(PORT);
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
    printf("Server up and running\n");
    char conf[] = "Linked";
    fd_set rfd;
    fd_set wfd;for (int j = 0; j < used; j++)
                {
                    if (j != i)
                        write(nsfd[j], &msg, sizeof(msg));
                }
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
                printf("%s-->\n",msg.data);
                for (int j = 0; j < used; j++)
                {
                    if (j != i)
                        write(nsfd[j], &msg, sizeof(msg));
                }
            }
        }


    }
    return 0;
}

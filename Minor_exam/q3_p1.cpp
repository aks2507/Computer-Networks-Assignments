#include <bits/stdc++.h>
#include <sys/msg.h>
#include <stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
using namespace std;
int main()
{
    FILE *files[3];
    files[0]=popen("./q3_p2","r");
    files[1]=popen("./q3_p3","r");
    files[2]=popen("./q3_p4","r");
    struct pollfd fds[3];
    for(int i=0;i<3;i++)
    {
        fds[i].fd=fileno(files[i]);
        fds[i].events=POLLIN;
    }
    int cnt=3,return_val;
    while(cnt--) 
    {
        return_val=poll(fds, 3,-1);
        if (return_val==-1)
        {
            exit(0);
        }
        for(int i=0;i<3;i++)
        {
            sleep(2);
            if (fds[i].revents & POLLIN)
            {
                char buff[100];
                read(fds[i].fd,buff,sizeof(buff));
                printf("%s",buff);
            }
        }
    }
    for(int i=0;i<3;i++) 
        pclose(files[i]);
    return 0;
}
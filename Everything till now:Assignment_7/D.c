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
#include <errno.h>

int main()
{
    int* shmptr;
    key_t k_shmptr=ftok("spid",65);

    int* kill_call_shmptr;
    key_t k_kill_call=ftok("kill_call",65);

    int shm_spid=shmget(k_shmptr,sizeof(int),IPC_CREAT|0666);
    int shm_kill_call=shmget(k_kill_call,sizeof(int),IPC_CREAT|0666);

    shmptr=(int*)shmat(shm_spid,NULL,0);
    kill_call_shmptr=(int*)shmat(shm_kill_call,NULL,0);

    int pid=*shmptr;

    int x;
    while(1)
    {
        scanf("%d",&x);
        if(x==1)
        {
            printf("Signal sent for Group 1\n");
            int kills=*kill_call_shmptr;
            *kill_call_shmptr=kills+1;
            kill(pid,SIGUSR1);
        }
        else if(x==2)
        {
            printf("Signal sent for Group 2\n");
            int kills=*kill_call_shmptr;
            *kill_call_shmptr=kills+1;
		    kill(pid,SIGUSR2);
		}
        else
        {
            int kills=*kill_call_shmptr;
            printf("Kill calls till now: %d\n",kills);
        }
        //sleep(5);
    } 
    return 0;
}

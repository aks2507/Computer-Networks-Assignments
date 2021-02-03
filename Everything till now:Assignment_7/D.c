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
int* shmptr;
key_t k_shmptr;
int shm_spid;

int* kill_call_shmptr;
key_t k_kill_call;
int shm_kill_call;

int pid;
int main()
{
    
    k_shmptr=ftok(".",64);
    shm_spid=shmget(k_shmptr,sizeof(int),IPC_CREAT|0666);
    shmptr=(int*)shmat(shm_spid,NULL,0);


    k_kill_call=ftok(".",65); 
    shm_kill_call=shmget(k_kill_call,sizeof(int),0666);
    kill_call_shmptr=(int*)shmat(shm_kill_call,NULL,0);

    pid=*shmptr;
    printf("pid received: %d\n",pid);
    
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

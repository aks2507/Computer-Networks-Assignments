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

#define FILLED 0 
#define Ready 1 
#define NotReady -1 

struct memory
{
	char buff[100];
	int status, pid1, pid2;
};

struct memory* shmptr_x,*shmptr_y;

void handler(int sig)
{
	if(sig==SIGUSR2)
	{
		printf("Received from P1: ");
		puts(shmptr_x->buff);
	}
}

int main()
{
	int pid=getpid();
	
	int shmid_x,shmid_y;	
	key_t key_x = ftok("X",65);
	shmid_x=shmget(key_x,sizeof(struct memory),IPC_CREAT|0666); //Creation
	shmptr_x=(struct memory*)shmat(shmid_x,NULL,0); //Attachment
	
	shmptr_x->pid2=pid;
	shmptr_x->status=NotReady;
	
	key_t key_y = ftok("Y",65);
	shmid_y=shmget(key_y,sizeof(struct memory),IPC_CREAT|0666); //Creation
	shmptr_y=(struct memory*)shmat(shmid_y,NULL,0); //Attachment
	
	shmptr_y->pid2=pid;
	shmptr_y->status=NotReady;
	
	signal(SIGUSR2, handler);
	while(1)
	{
		sleep(2);
		
		printf("Me: ");
		fgets(shmptr_y->buff,100,stdin);
		
		shmptr_y->status=Ready;
		
		kill(shmptr_y->pid1,SIGUSR1);
		
		while(shmptr_x->status==Ready)
			continue;
	}
	shmdt((void*)shmptr_x); 
	shmdt((void*)shmptr_y); 
    //shmctl(shmid, IPC_RMID, NULL); 
	return 0;		
}

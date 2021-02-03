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
#include<signal.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include<stdio.h>

#define FILLED 0 
#define Ready 1 
#define NotReady -1 

struct memory
{
	int pid1, pid2;
};

struct memory* shmptr;
int received=1;
void handler(int signal)
{
	printf("Signal received from P1\n");
	received--;
}
void* func(void* args)
{	
	signal(SIGUSR1,handler);
	while(received);
	printf("Signal received!!\n");
}

int main()
{
	//signal(SIGUSR1,handler);
	int pid=getpid();
	int shmid;
	key_t key = ftok("X",65);
	shmid=shmget(key,sizeof(struct memory),IPC_CREAT|0666); //Creation
	shmptr=(struct memory*)shmat(shmid,NULL,0); //Attachment	
	shmptr->pid2=pid;
	
	pthread_t t2;
	pthread_create(&t2,NULL,func,NULL);
	//wait(NULL);
	pthread_join(t2,NULL);
	
	
	return 0;
}

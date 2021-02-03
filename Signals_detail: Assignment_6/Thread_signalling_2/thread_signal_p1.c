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

void* func(void* args)
{
	printf("P1_thread signalling P2_thread\n");
	sleep(7);
	kill(shmptr->pid2,SIGUSR1);
}

int main()
{
	int pid=getpid();
	int shmid;
	key_t key = ftok("X",65);
	shmid=shmget(key,sizeof(struct memory),IPC_CREAT|0666); //Creation
	shmptr=(struct memory*)shmat(shmid,NULL,0); //Attachment	
	shmptr->pid1=pid;
	
	pthread_t t1;
	pthread_create(&t1,NULL,func,NULL);
	//wait(NULL);
	pthread_join(t1,NULL);
	return 0;
}

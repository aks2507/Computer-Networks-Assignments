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

pthread_t tid2,tid;

void handler(int signum)
{
	if(signum==SIGUSR1)
	printf("Received a signal from other thread\n");
	else
		printf("Received a signal from main thread\n");
}

void* func(void* args)
{
	printf("I am thread1 and I will trouble thread2\n");
	pthread_kill(tid2,SIGUSR1);
	sleep(5);
}

void* func2(void* args)
{
	printf("I am thread2\n");
	sleep(5);
	pthread_kill(tid,SIGUSR2);
}

int main()
{
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	pthread_create(&tid2,NULL,func2,NULL);
	int val = pthread_create(&tid,NULL,func,NULL);
	if(val)
	{
		printf("Error!\n");
		exit(1);
	}
	pthread_join(tid,NULL);
	pthread_join(tid2,NULL);
	return 0;
}

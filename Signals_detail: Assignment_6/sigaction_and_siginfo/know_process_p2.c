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

struct sigaction act;
		
void handler(int sig, siginfo_t *siginfo, void *context)
{
	printf("Signal received by Process with PID: %ld, UID: %ld\n",(long) siginfo->si_pid, (long) siginfo->si_uid);
}

int main()
{
	int pid = getpid();
	printf("Process P2 with PID: %d started!\n",pid);
	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;

	int k = sigaction(SIGUSR1 , &act , NULL);
	sleep(1);
	if(k < 0)
	{
		printf("Error in SIGUSR1\n");
		return 1;
	}
	else
	{
		printf("Signal received.\n");
	}
	//sleep(5);
	while(1);
	return 0;
}

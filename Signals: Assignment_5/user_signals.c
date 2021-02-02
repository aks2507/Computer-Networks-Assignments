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

#define SIGHUP  1   /* Hangup the process */ 
#define SIGINT  2   /* Interrupt the process */ 
#define SIGQUIT 3   /* Quit the process */ 
#define SIGILL  4   /* Illegal instruction. */ 
#define SIGTRAP 5   /* Trace trap. */ 
#define SIGABRT 6   /* Abort. */

int received=0;
void user_handler(int sig)
{
	signal(SIGUSR1,user_handler);
	printf("Child: I have received user signal\n");
}

void parent_handler(int sig)
{
	signal(SIGUSR2,parent_handler);
	printf("Parent: I have received signal from child\n");
	exit(0);
}
void handle_sighup(int sig){
	signal(SIGHUP , handle_sighup);
	printf("Child : I have received a SIGHUP\n");
}

void handle_sigint(int sig){
	signal(SIGINT , handle_sigint);
	printf("child : I have received a SIGINT\n");
}

void handle_sigquit(int sig){
	printf("I am Dead\n");
	received=1;
	//exit(0);
}


int main(){
	int pid;
	pid = fork();

	if(pid == 0)
	{
		printf("child\n");
		signal(SIGUSR1,user_handler);
		signal(SIGHUP , handle_sighup);
		signal(SIGINT  , handle_sigint);
		signal(SIGQUIT , handle_sigquit);
		while(!received);
		sleep(2);
		printf("CHILD: Sending SIGUSR2\n\n");
		kill(getppid(),SIGUSR2);
		while(1);
	}
	else
	{
		sleep(2);
		
		printf("\nPARENT: Sending SIGUSR1\n\n");
		kill(pid,SIGUSR1);
		
		sleep(2);
		printf("\nPARENT: sending SIGHUP\n\n"); 
        kill(pid, SIGHUP); 
  
        sleep(2);
        printf("\nPARENT: sending SIGINT\n\n"); 
        kill(pid, SIGINT); 
  
        sleep(2); 
        printf("\nPARENT: sending SIGQUIT\n\n"); 
        kill(pid, SIGQUIT);
        sleep(2);
        
        
        signal(SIGUSR2,parent_handler);
        while(1);
	}
	return 0;
}

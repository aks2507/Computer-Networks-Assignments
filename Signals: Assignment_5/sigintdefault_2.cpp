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
/*
int a[2];

void handler_function(int);

void handler_function(int signum)
{
	int cnt;
	read(a[0],&cnt,sizeof(cnt));
	if(cnt>5) 
		signal(SIGINT,SIG_DFL);
	else 
		printf(">>>  executing handler for %d time <<<\n",cnt);
}

int main()
{

	pipe(a);
	int count=1;

	signal(SIGINT,handler_function);
	while(1)
	{
		printf("Program executing (1s delay between updates)...\n");
		write(a[1],&count,sizeof(count));
		count++;
		sleep(1);
	}

	return(0);
}
*/

int c=0;
void handler(int sig)
{
	c++;
	if(c>5) 
		signal(2,SIG_DFL);
	else
	{
		printf("No of times ctrl+c pressed: %d\n",c);
	}
}

int main()
{
    signal(2,handler);
	while(1)
	{
        printf("Program Executing.....\n");
        sleep(1);
    }
    return 0;
}

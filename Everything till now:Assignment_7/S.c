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
#include <sys/stat.h>

char* c1="c1";
char* c2="c2";
char* sc1="sc1";
char* sc2="sc2";
char* a="a";
char* b="b";

char buff_c1[1024];//buff_c2[1024];
int children[4];

int* shmptr;
int* kill_call_shmptr;

key_t k_shmptr;
key_t k_kill_call;

int shm_spid;
int shm_kill_call;

sigset_t set;

void handler(int sig);
void handler_sc1(int sig);
void handler_sc2(int sig);
void handler_a(int sig);
void handler_b(int sig);
void *polling(void* args);

void handler(int sig)
{
	if(sig==SIGUSR1)
	{
		printf("Sending signal to Group1\n");
		int kills=*kill_call_shmptr;
		*kill_call_shmptr=kills+3;
		killpg(children[3],SIGSTOP);
		killpg(children[0],SIGCONT);
		killpg(children[0],SIGUSR1);
		int fd_sc1=open(sc1,O_WRONLY);
		//write(fd_sc1,"message from S to G1: ",sizeof "message from S to G1: ");
		write(fd_sc1,buff_c1,sizeof(buff_c1));

		int fd_sc2=open(sc2,O_WRONLY);
		//write(fd_sc2,"message from S to G1: ",sizeof "message from S to G1: ");
		write(fd_sc2,buff_c1,sizeof(buff_c1));

	}
	else
	{
		printf("Sending signal to Group2\n");
		int kills=*kill_call_shmptr;
		*kill_call_shmptr=kills+3;
		killpg(children[0],SIGSTOP);
		killpg(children[3],SIGCONT);
		killpg(children[3],SIGUSR1);
        
		int fd_a=open(a,O_WRONLY);
		//write(fd_a,"message from S to G2: ",sizeof "message from S to G2: ");
		write(fd_a,buff_c1,sizeof(buff_c1));

		int fd_b=open(b,O_WRONLY);
		//write(fd_b,"message from S to G2: ",sizeof "message from S to G2: ");
		write(fd_b,buff_c1,sizeof(buff_c1));
		
	}
	
}

void handler_sc1(int sig)
{
	printf("S`1\n");
	char buff[1024];
	int fd_sc1=open(sc1,O_RDONLY);
	read(fd_sc1,buff,1024);
	close(fd_sc1);

	printf("S`1: Message read from S = %s\n",buff);
	sleep(1);
	//int kills=*kill_call_shmptr;
	//*kill_call_shmptr=kills+2;
	//sleep(1);
	//killpg(children[0],SIGSTOP);
	//killpg(children[3],SIGCONT);
}

void handler_sc2(int sig)
{
	printf("S`2\n");
	char buff[1024];
	int fd_sc2=open(sc2,O_RDONLY);
	read(fd_sc2,buff,1024);
	close(fd_sc2);

	printf("S`2: Message read from S = %s\n",buff);
}

void handler_a(int sig)
{
	printf("A\n");
	char buff[1024];
	int fd_a=open(a,O_RDONLY);
	read(fd_a,buff,1024);
	close(fd_a);

	printf("A: Message read from S = %s\n",buff);
	sleep(1);
	//int kills=*kill_call_shmptr;
	//*kill_call_shmptr=kills+2;
	//sleep(1);
	//killpg(children[3],SIGSTOP);
	//killpg(children[0],SIGCONT);
}

void handler_b(int sig)
{
	printf("B\n");
	char buff[1024];
	int fd_b=open(b,O_RDONLY);
	read(fd_b,buff,1024);
	close(fd_b);

	printf("B: Message read from S = %s\n",buff);
}

void *polling(void* args)
{
	printf("poll\n");
	pthread_sigmask(SIG_BLOCK,&set,NULL);

	struct pollfd pfd[2];
	int c1_pid=open(c1,O_RDWR|O_NONBLOCK);
	int c2_pid=open(c2,O_RDWR|O_NONBLOCK);
	pfd[0].fd=c1_pid;
	pfd[1].fd=c2_pid;

	pfd[0].events=POLLIN;
	pfd[1].events=POLLIN;

	while(1)
	{
		int ret=poll(pfd,2,5000);
		if(ret!=0)
		{
			if(ret==-1)
			{	
				printf("Error in polling\n");
				exit(1);
			}
			char buff[1024];
			for(int i=0;i<2;i++)
			{
				if(pfd[i].revents & POLLIN)
				{
					if(read(pfd[i].fd,buff,1024) > 0)
					{
						printf("Poll read works!!\n");
						printf("Read from %d: %s\n",pfd[i].fd,buff);
						// if(i==0)
						// {
						// 	strcpy(buff_c1,buff);
						// }
						// else
						// {
						// 	strcpy(buff_c2,buff);
						// }
						strcpy(buff_c1,buff);
						fflush(fdopen(pfd[i].fd,"r+"));
					}
				}
			}
		}
	}
}

int main()
{	
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);

	/*
	SHARED MEMORY CREATION FOR:
		1) SHARING PID OF S TO D
		2) KILL CALL COUNTS
	*/
	k_shmptr=ftok(".",64);
	shm_spid=shmget(k_shmptr,sizeof(int),IPC_CREAT|0666);
	shmptr=(int*)shmat(shm_spid,NULL,0);
	
	*shmptr=0;
	
	k_kill_call=ftok(".",65);
	shm_kill_call=shmget(k_kill_call,sizeof(int),IPC_CREAT|0666);
	kill_call_shmptr=(int*)shmat(shm_kill_call,NULL,0);
	
	int pid_s=getpid();
	*shmptr=pid_s;
	printf("Send: %d\n",pid_s);

	
	*kill_call_shmptr=0;

	/*FIFO CREATION FOR C1 AND C2*/
	mkfifo(c1,0666|IPC_CREAT);
	mkfifo(c2,0666|IPC_CREAT);
	
	/*FIFO CREATION FOR THE CHILDREN*/
	mkfifo(sc1,IPC_CREAT|0666);
	mkfifo(sc2,IPC_CREAT|0666);
	mkfifo(a,IPC_CREAT|0666);
	mkfifo(b,IPC_CREAT|0666);
    
	/*CREATION OF BACHCHE*/
	int pid=fork();
	if(pid==0)
	{
		signal(SIGUSR1,handler_sc1);
		printf("Child S`1 Started\n");

		while(1)
		{
			printf("Child S`1 running\n");
			sleep(2);
		}
	}
	else
	{
		int sc1_pid=pid;
		children[0]=sc1_pid;
		pid=fork();
		if(pid==0)
		{
			signal(SIGUSR1,handler_sc2);
			printf("Child S`2 Started\n");

			while(1)
			{
				printf("Child S`2 running\n");
				sleep(2);
			}
		}
		else
		{
			int sc2_pid=pid;
			children[1]=sc2_pid;
			pid=fork();
			if(pid==0)
			{
				signal(SIGUSR1,handler_a);
				printf("Child A Started\n");

				while(1)
				{
					printf("Child A running\n");
					sleep(2);
				}
			}
			else
			{
				int a_pid=pid;
				children[2]=a_pid;
				pid=fork();
				if(pid==0)
				{
					signal(SIGUSR1,handler_b);
					printf("Child B Started\n");

					while(1)
					{
						printf("Child B running\n");
						sleep(2);
					}
				}
				else
				{
				    int b_pid=pid;
					children[3]=b_pid;
					sleep(2);
					setpgid(children[3],children[3]); //B
					setpgid(children[2],children[3]); //A

					setpgid(children[0],children[0]); //S`1
					setpgid(children[1],children[0]); //S`2

					killpg(children[0],SIGSTOP);
					killpg(children[3],SIGSTOP);

					sigemptyset(&set);
					sigaddset(&set,SIGUSR1);
					sigaddset(&set,SIGUSR2);

					pthread_t t1;
					pthread_create(&t1,NULL,polling,NULL);
					pthread_join(t1,NULL);
				}
			}
		}
	}
	return 0;	
}

#include <bits/stdc++.h>
#include <sys/msg.h>
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
using namespace std;
struct msg_buf
{
	long mesg_type;
	char mesg_text[100];
};
struct shm_buff
{
	int pid[100];
	int server_pid;
};
struct shm_buff* pid_mem;
int main()
{
	int shmid;
	key_t key=ftok("X",65);
	shmid=shmget(key,sizeof(struct shm_buff),IPC_CREAT|0666);
	pid_mem=(struct shm_buff*)shmat(shmid,NULL,0);
	pid_mem->pid[0]=getpid();
	
	for(int i=1;i<100;i++)
		pid_mem->pid[i]=0;
	key_t key_2=ftok("file1",65);

    int msgid=msgget(key_2,0666|IPC_CREAT);
	cout<<"My pid is: "<<getpid()<<"\n";
	while(1)
	{
		struct msg_buf message; 
		if(msgrcv(msgid,&message,sizeof(message),pid_mem->server_pid,0)>0)
		{
			cout<<"Message received: "<<message.mesg_text<<"\n";
			continue;
		}
		else if(msgrcv(msgid,&message,sizeof(message),pid_mem->pid[1],0)>0)
		{
			message.mesg_type=pid_mem->pid[1];
			msgrcv(msgid,&message,sizeof(message),message.mesg_type,0);
			pid_mem->pid[1]=message.mesg_type;
			if(pid_mem->pid[1]!=0)
				kill(SIGINT,pid_mem->pid[1]);
		}
	}
	shmdt((void*)pid_mem);
	shmctl(shmid,IPC_RMID,NULL);
	msgctl(msgid,IPC_RMID,NULL);
	return 0;
}
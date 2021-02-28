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

	key_t key_2=ftok("file1",65);

    pid_mem->server_pid=getpid();
    int msgid=msgget(key_2,0666|IPC_CREAT);
    cout<<"My pid is: "<<getpid()<<"\n";
	while(1)
	{
		struct msg_buf message; 
        for(int i=0;i<100;i++)
        {
            if(pid_mem->pid[i]==0)
                break;
            if(msgrcv(msgid,&message,sizeof(message),pid_mem->pid[i],0)>0)
            {
                cout<<"Message received: "<<message.mesg_text<<"\n";
                for(int j=0;j<100;j++)
                {
                    if(pid_mem->pid[j]==0)
                        break;
                    else
                    {
                        message.mesg_type=getpid();
                        msgsnd(msgid,&message,sizeof(message),0);
                    }
                }
            }
        }
	}
    shmdt((void*)pid_mem);
	shmctl(shmid,IPC_RMID,NULL);
	msgctl(msgid,IPC_RMID,NULL);
	return 0;
}
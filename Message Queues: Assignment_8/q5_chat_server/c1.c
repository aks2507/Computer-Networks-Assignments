#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/unistd.h>

struct message_buf{
	long mesg_type;
	char mesg_text[100];
};

key_t write_key,read_key;
int read_msgqid,write_msgqid;
int pid,proj_id;

void sigint(int sig)
{
	printf("C1(%d):",pid);
	struct message_buf message;
	fgets(message.mesg_text,1024,stdin);
	message.mesg_type=pid;
	msgsnd(write_msgqid,&message,sizeof(message),0);
}

void first_time()
{
	printf("Requesting to join group\n");
	struct message_buf message,message_read;
	fgets(message.mesg_text,1024,stdin);
	message.mesg_type=(long)(pid);
	int a=msgsnd(write_msgqid,&message,sizeof(message),0);

	long temp_type=(long)(pid+1);
	if(msgrcv(write_msgqid,&message_read,sizeof(message_read),temp_type,0)!=-1)
    {
        proj_id=atoi(message_read.mesg_text);
    }
}

int main()
{
	write_key=ftok("server",102);
	write_msgqid=msgget(write_key,0666 | IPC_CREAT);
	pid=getpid();

	first_time();
	read_key=ftok("client",proj_id);
	read_msgqid=msgget(read_key,0666 | IPC_CREAT);

	while(1)
	{
		struct message_buf message_read;
		signal(SIGINT,sigint);
		if(msgrcv(read_msgqid,&message_read,sizeof(message_read),pid,0)!=-1)
			printf("received %s\n",message_read.mesg_text);
	}

}


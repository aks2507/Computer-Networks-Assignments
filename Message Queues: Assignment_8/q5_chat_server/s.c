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

key_t read_key,write_key;
int read_msgqid,write_msgqid;
int total_client=0;
long pids[100];

void is_there(long pid)
{
	for(int i=0;i<total_client;i++)
		if(pids[i]==pid)
			return;

	pids[total_client++]=pid;
	struct message_buf message_write;
	message_write.mesg_type=pid+1;
	strcpy(message_write.mesg_text,"101");
	msgsnd(read_msgqid,&message_write,sizeof(message_write),0);
}

void delete_server(int sig)
{
		msgctl(read_msgqid, IPC_RMID, NULL);
		msgctl(write_msgqid, IPC_RMID, NULL);
}

int main()
{
	read_key=ftok("server",102);
	write_key=ftok("client",101);
	read_msgqid=msgget(read_key,0666 | IPC_CREAT);
	write_msgqid=msgget(write_key,0666 | IPC_CREAT);

	while(1)
	{
		struct message_buf message,message_write;
		message.mesg_type=0;
		if(msgrcv(read_msgqid,&message,sizeof(message),message.mesg_type,0)!=-1)
		{
			is_there(message.mesg_type);
			for(int i=0;i<total_client;i++)
			{
				if(pids[i]==message.mesg_type)
					continue;
				printf("Receiving from: %ld %s\n",pids[i],message.mesg_text);
				struct message_buf message_write;
				message_write.mesg_type=pids[i];
				strcpy(message_write.mesg_text,message.mesg_text);
				msgsnd(write_msgqid,&message_write,sizeof(message_write),0);
			}
			signal(SIGINT,delete_server);
		}
	}

}


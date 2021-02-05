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
#include <sys/msg.h>

struct msg_buff
{
    long msg_type;
    char msg_text[1024];
};

struct msg_buff message;

int main()
{
    key_t key=ftok("file1",65);

    int msgid=msgget(key,0666|IPC_CREAT);

    while(1)
    {
        msgrcv(msgid,&message,sizeof(message),1,0);

        printf("P1: %s",message.msg_text);

        message.msg_type=2;

        printf("Me: ");
        fgets(message.msg_text,1024,stdin);

        msgsnd(msgid,&message,sizeof(message),0);
    }

    msgctl(msgid,IPC_RMID,NULL);
    return 0;
}
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
struct msqid_ds qbuf;
static const struct msqid_ds empty;
int get_queue_ds(int msgid)
{
    if(msgctl(msgid,IPC_STAT,&qbuf)==-1)
    {   
        printf("Error in IPC_STAT\n");
        return errno;
    }
    printf("Number of messages currently in queue: %lu\n",qbuf.msg_qnum);
    printf("Max number of bytes allowed in queue: %lu\n",qbuf.msg_qbytes);
    printf("Current number of bytes in queue: %lu\n",qbuf.__msg_cbytes);
    printf("PID of the last process that called msgsnd() for the queue: %d\n",qbuf.msg_lspid);
    printf("PID of the last process that called msgrcv() for the queue: %d\n",qbuf.msg_lrpid);
    printf("Owner's UID: %d\n",qbuf.msg_perm.uid);
    printf("Owner's GID: %d\n",qbuf.msg_perm.gid);
    printf("Creator's UID: %d\n",qbuf.msg_perm.cuid);
    printf("Creator's GID: %d\n",qbuf.msg_perm.cgid);
    printf("Access Modes: %d\n",qbuf.msg_perm.mode);
    return 1;
}

int main()
{
    key_t key=ftok("file1",65);

    int msgid=msgget(key,0666|IPC_CREAT);

    while(1)
    {
        message.msg_type=1;

        printf("Me: ");
        fgets(message.msg_text,1024,stdin);

        msgsnd(msgid,&message,sizeof(message),0);
        printf("DATA AFTER SEND:\n");
        get_queue_ds(msgid);
        qbuf=empty;

        msgrcv(msgid,&message,sizeof(message),2,0);
        printf("P2: %s",message.msg_text);
        printf("DATA AFTER RECEIVE:\n");
        get_queue_ds(msgid);
        qbuf=empty;
    }
    msgctl(msgid,IPC_RMID,NULL);
    return 0;
}
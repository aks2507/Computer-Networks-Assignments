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

char* c1 = "c1";

int main()
{
    mkfifo(c1,0666);
	while(1)
    {
        char buf[1024];
        
        fgets(buf,1024,stdin);

        int fd1 = open(c1,O_RDWR);
        write(fd1,buf,sizeof(buf));

        fflush(fdopen(fd1,"r+"));
        close(fd1);
    }
	return 0;
}

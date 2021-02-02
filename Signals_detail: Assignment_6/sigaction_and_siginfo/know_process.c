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

int main(){
	pid_t c = fork();
	if(c == 0){
		char *args[]={"./know_process_p2",NULL}; 
        execvp(args[0],args);
	}
	else{
		sleep(3);
		kill( c , SIGUSR1);
	}
	return 0;
}

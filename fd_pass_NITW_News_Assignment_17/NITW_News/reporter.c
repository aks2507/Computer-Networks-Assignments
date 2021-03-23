#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<errno.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define BUFFSIZE 1024

int main(int argc, char* argv[])
{
	int i=10;
	while(i--)
	{
		//printf("Reporter started...\n");
		char buff[BUFFSIZE];
		//printf("Enter the ground breaking report!!\n");
		fgets(buff,BUFFSIZE,stdin);
		write(1,&buff,sizeof(buff));
	}
	fflush(stdout);
	return 0;
}
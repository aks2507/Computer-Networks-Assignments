#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
 
int main()
{
	int fd=open("/dev/tickTock",O_RDWR);
	printf("%d",fd);
	char buf[100];
	int c=read(fd,buf,sizeof(100));
	printf("%d out read %s\n",c,buf);
	strcat(buf,"30");
	c=write(fd,buf,strlen(buf));
	printf("%d out write \n",c);
	return 0;
}

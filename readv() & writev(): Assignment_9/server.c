#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <string.h>

int main()
{
	char buff[3][30];
	struct iovec iov[3];
	ssize_t nr,nr2;
	int fd,i;

	fd=open("C1", O_RDONLY);
	mkfifo("Server",0666);

	for(int i=0;i<3;i++)
	{
		iov[i].iov_base=buff[i];
		iov[i].iov_len=sizeof(buff[i]);
	}
	//Reading from client
	nr=readv(fd,iov,3);

	for(i = 0; i < 3; i++)
		printf ("%s\n",(char *)iov[i].iov_base);

	close(fd);

	//Changing the text and writing back to client
	char buff2[3][30];
	for(int i=0;i<3;i++)
	{
		strcpy(buff2[i],(char*)iov[i].iov_base);
		buff2[i][0]='W';
		iov[i].iov_base=buff2[i];
		//printf("%s\n",(char*)iov[i].iov_base);
		iov[i].iov_len=sizeof(buff2[i]);
	}

	int fdw=open("Server",O_WRONLY);

	nr2=writev(fdw,iov,3);
	printf("wrote %ld bytes\n",nr2);

	
	close(fdw);
	return 0;
}
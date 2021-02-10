#include <stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/uio.h>

int main()
{
	struct iovec iov[3];
	ssize_t nr,nr2;
	int fd, i;

	char *buf[]= 
	{
		"This is line 1: in buffer 1.\n",
		"This is line 2: in buffer 2.\n",
		"This is line 3: in buffer 3.\n" 
	};
	mkfifo("C1",0666);

	fd = open ("C1", O_WRONLY);

	for (i=0;i<3;i++) 
	{
		iov[i].iov_base=buf[i];
		iov[i].iov_len=strlen(buf[i])+1;
	}

	//Writing to server
	nr=writev(fd,iov,3);
	printf("wrote %ld bytes\n",nr);
	close(fd);
	sleep(2);

	//Receiving the changed text from server
	char buff[3][30];
	int fdr=open("Server", O_RDONLY);
	
	for(int i=0;i<3;i++)
	{
		iov[i].iov_base=buff[i];
		iov[i].iov_len=sizeof(buff[i]);
	}
	nr2=readv(fdr,iov,3);
	for(i = 0; i < 3; i++)
		printf ("%s\n",(char *)iov[i].iov_base);

	
	close(fdr);
	return 0;
}
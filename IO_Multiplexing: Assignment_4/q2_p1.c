#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main(){

	int fd;
	char *myfifo="xyz";
	mkfifo(myfifo,0666);
	 
	char buf1[100],buf2[100];

	while(1)
	{
		fd=open(myfifo,O_WRONLY);

		fgets(buf1,100,stdin);

		write(fd,buf1,100);
		close(fd);

		fd=open(myfifo,O_RDONLY);

		read(fd,buf2,100);

		printf("User2: %s\n",buf2);
		close(fd);
	}

}

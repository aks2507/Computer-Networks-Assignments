#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include<stdlib.h>
#include <string.h> 

int main()
{
	while(1)
	{
		char buf[1024];
		strcpy(buf,"");
		read(0,buf,sizeof(buf));
		buf[strlen(buf)-1] = '\0';
		if(buf[0]=='@')
		{
			exit(0);
		}
		printf("%s\n",buf);
		fflush(stdout);
		printf("S2 processed the message!\n");
		fflush(stdout);
	}
	printf("S2 terminated!\n");
	return 0;
}
#include<stdio.h>
#include <pthread.h>
#include <unistd.h>
#include<poll.h>
struct pollfd fds[3];
int timeout_msecs = 5000;
int ret;

int main(){
  	int fd[3];
  	char buf[128];
  	fd[0]=fileno(popen("./pt1","r"));
  	fd[1]=fileno(popen("./pt2","r"));
  	fd[2]=fileno(popen("./pt3","r"));
  	int t=4;
  	while(t--)
	{
    	fds[0].fd=fd[0];
    	fds[1].fd=fd[1];
    	fds[2].fd=fd[2];
    	
    	fds[0].events=POLLIN;
    	fds[1].events=POLLIN;
    	fds[2].events=POLLIN;
  		
  		ret=poll(fds,3,-1);
  		
  		if (ret<=0) 
  		{
			printf ("%d seconds elapsed.\n", 0);
			return 0;
		}
    	for(int j=0;j<3;j++)
    	{
      		sleep(1);
      		if(fds[j].revents & POLLIN)
      		{
        		int i=read(fds[j].fd,buf,5);
        		write(1,buf,i);
        		printf("  ----->from P%d\n",j+2);
      		}
    	}
	}
  	return 0;
}

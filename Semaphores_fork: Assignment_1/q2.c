#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	for(int i=0;i<2;i++)
	{
	    int k1=getpid();
	    pid_t f_id=fork();
		if(f_id==0)
		{
		    k1=getpid();
			printf("Child process with pid: %d created from Parent with pid: %d\n",k1,getppid());
			sleep(1);
			printf("Child process with pid = %d exits now\n",k1);
			exit(0);
		}
		
		int status;
		waitpid(f_id,&status,0);
		if(WIFEXITED(status))
		{
		    int es=WEXITSTATUS(status);
		    printf("Message from parent, id = %d :=> P%d has exited with exit status: %d\n",getpid(),i+2,es);
		}
	}
	return 0;
}


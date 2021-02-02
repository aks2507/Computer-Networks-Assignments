#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	int pp1[2],pp2[2];
	pipe(pp1);
	pipe(pp2);
	
	int stdinsave=dup(STDIN_FILENO);
	int stdoutsave=dup(STDOUT_FILENO);
	//printf("%d %d\n",stdinsave,stdoutsave);
	
	dup2(pp1[0],0);
	dup2(pp2[1],1);
	
	int c=fork();
	
	if(c>0)
	{
		dup2(stdinsave,STDIN_FILENO);
		dup2(stdoutsave,STDOUT_FILENO);
		
		close(pp1[0]);
		close(pp2[1]);

		char buf1[128],buf2[128];
		
		printf("Writing to p2:");
		fgets(buf1,128,stdin);
		write(pp1[1],buf1,128);
		
		read(pp2[0],buf2,128);
		printf("InP1: %s \n",buf2);
	}
	else 
	{
		char *args[]={"./q4_p2",NULL}; 
		execvp(args[0],args); 
	}
	return 0;
}

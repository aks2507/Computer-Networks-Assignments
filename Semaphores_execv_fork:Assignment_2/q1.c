#include<stdio.h>  
#include<unistd.h>  
#include<fcntl.h> 
#include<stdlib.h> 
#include <semaphore.h>
const char* name1="s1";
const char* name2="s2";

void p1(int fd)
{
	sem_t* s1=sem_open(name1,O_EXCL);
	sem_t* s2=sem_open(name2,O_EXCL);
	char c;
	printf("p1: ");
	while(read(fd,&c,1)==1)
	{
		if(c=='\n')
		{
			printf("\n");
			sem_post(s1);
			sem_wait(s2);
			printf("p1: ");
		}
		else
		{
			printf("%c",c);
		}
	}
	
}
void p2(int fd)
{
	printf("p2: ");
	sem_t* s1=sem_open(name1,O_EXCL);
	sem_t* s2=sem_open(name2,O_EXCL);
	sem_wait(s1);
	char c;
	while(read(fd,&c,1)==1)
	{
		if(c=='\n')
		{
			printf("\n");
			sem_post(s2);
			sem_wait(s1);
			printf("p2: ");
		}
		else
		{
			printf("%c",c);
		}
	}
	sem_post(s2);
	
}
int main()
{
	sem_t *mutex1 = sem_open(name1,O_CREAT|O_EXCL,0666,0);
    sem_t *mutex2 = sem_open(name2,O_CREAT|O_EXCL,0666,0);
	int fd;
	
	fd=open("mytext.txt",O_RDONLY);
	
	int c=fork();
	if(c>0)
	{
		p1(fd);			
	}
	else
	{
		p2(fd);
	}
	sem_unlink(name1);
    sem_unlink(name2); 
	return 0;
}
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
using namespace std;

const char* name = "C2";

void* reader(void* args)
{
   while(1)
   {
   	 int rfd = open(name,O_RDONLY);
   	 char c,msg[1024];
   	 int j=0;
   	 while(read(rfd,&c,1))
   	 {
   	 	msg[j++]=c;
   	 }
   	 msg[j]='\0';
   	 cout<<msg<<"\n";
   	 close(rfd);
   }
}

void* writer(void* args)
{
  while(1)
  {
  	 char c,msg[1024],buff[1024];
  	 int j=0;
  	 read(STDIN_FILENO,&c,1);
  	 while(c!='\n')
  	 {
       msg[j++] = c;
       read(STDIN_FILENO,&c,1);
  	 }
  	 msg[j] = '\0';
  	 buff[0] = '\0';
  	 strcat(buff,name);
  	 strcat(buff,":");
  	 strcat(buff,msg);
     strcat(buff,"$\n");
     //mkfifo("server",0666);
  	 int wfd = open("serve",O_WRONLY);
  	 write(wfd,&buff,1024);

     close(wfd);
  }
}

int main()
{
	cout<<name<<" Client started!\n";
	mkfifo(name,0666);

	pthread_t write_th,read_th;
	pthread_create(&write_th,NULL,writer,NULL);
	pthread_create(&read_th,NULL,reader,NULL);

	pthread_join(write_th,NULL);
	pthread_join(read_th,NULL);
}

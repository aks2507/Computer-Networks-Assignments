#include <stdio.h>
#include <unistd.h>
#include <string>
#include<iostream>
#include<pthread.h>

#define MSG_SZ 500

using namespace std;

void* read_func(void* args)
{
	int* fd_a=(int*)args;
	int fd=*fd_a;
	char buff[MSG_SZ];
	read(fd,buff,MSG_SZ);
	cout<<"Returned value by parent to child: "<<buff<<"\n";
}

void* write_func(void* args)
{
	int* fd_a=(int*)args;
	int fd=*fd_a;
	char buff[MSG_SZ];
	cout<<"Enter your message: ";
	//cout<<fd<<" :";
	fgets(buff,128,stdin);
	write(fd,buff,MSG_SZ);
}

int main()
{
	int fd1=dup(STDIN_FILENO);
    int fd2=dup(STDOUT_FILENO);
    dup2(7,STDIN_FILENO);
    dup2(8,STDOUT_FILENO);
    pthread_t read_th,write_th;
    while(1)
		{
			//sleep(4);
			pthread_create(&read_th,NULL,&read_func,(void*)&fd1);
			sleep(5);
			pthread_create(&write_th,NULL,&write_func,(void*)&fd2);
			pthread_join(read_th,NULL);
			
			pthread_join(write_th,NULL);
		}
}

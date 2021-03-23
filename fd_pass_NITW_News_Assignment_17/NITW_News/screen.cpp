#include <iostream>
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

#define MAX 100
#define O_RDONLY         00
#define O_WRONLY         01
#define O_RDWR           02

int main()
{
	cout<<"Screen\n";
	mkfifo("screenfifo",0666);
	//Messages will be put in the fifo by the news reader
	while(1)
    {
		int rfd = open("screenfifo",O_RDONLY);
		char c,msg[MAX];
        //Accessing the message from fifo and printing it
		int j=0;
		while(read(rfd,&c,1))
			msg[j++]=c;
		msg[j]='\0';
		cout<<msg<<"\n";
		fflush(stdout);
	}
	
	return 0;
}
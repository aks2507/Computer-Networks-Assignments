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

int main()
{
	mkfifo("serve",0666);
	mkfifo("C1",0666);
    int fd = open("serve",O_RDONLY);
    cout<<"Serve Fifo is opened\n";

	char buff[1024];

	read(fd,&buff,1024);
	cout<<"Message read from group by server:\n";
	cout<<buff<<endl;

	int w_code_fd=open("code_file.cpp",O_CREAT|O_WRONLY|O_APPEND|O_TRUNC);
    
    for(int i=3;buff[i]!='$';i++)
    {
    	write(w_code_fd,&buff[i],1);
    }

    int inp = dup(0);
    int out = dup(1);
    int e = dup(2);

    int rfd = open("inp.txt",O_RDONLY);
    int wfd = open("returned_out.txt",O_WRONLY|O_TRUNC|O_CREAT,0666);

    dup2(rfd,0);
    dup2(wfd,1);
    dup2(wfd,2);

    string comm = "g++ code_file.cpp -o code_file";
    const char* command = comm.c_str();
    int val =system(command);

    if(val>0)
    {
    	char error[1024];
    	close(wfd);
    	int nfd = open("returned_out.txt",O_RDONLY);
    	read(nfd,error,1024);
    	//strcpy(error,"Compile error occured");
    	int client_msg = open("C1",O_WRONLY);
        write(client_msg,error,1024);
        close(client_msg);
        exit(1);
    }

    system("./code_file");

    dup2(inp,0);
    dup2(out,1);  //restored input and output
    dup2(e,2);

    close(wfd);
    close(rfd);


    int got_out = open("returned_out",O_RDONLY);
    int exp_out = open("req_out.txt",O_RDONLY);


    char c1,c2;
    bool cond=0;
    bool match=true;
    while(read(got_out,&c1,1) && read(exp_out,&c2,1))
    {  
    	printf("%c %c");
    	if(c1!=c2)
    	{
    		cout<<"pehla\n";
    		match = false;
    		cond=1;
    		break;
    	}

    }
	if(cond==0)
	{
		cout<<"here\n";
    	if(read(got_out,&c1,1) || read(exp_out,&c2,1))
    	{
    		cout<<c1<<" "<<c2<<"\n";
    		match = false;
		}
	}
    
    char verdict[1024];

    if(!match)
    {
    	cout<<"WRONG ANSWER!"<<endl; 
    	strcpy(verdict,"WRONG ANSWER");
    }
    else 
    {
    	cout<<"ACCEPTED!"<<endl; 
    	strcpy(verdict,"Accepted");
    }

    int client_msg = open("C1",O_WRONLY);
    write(client_msg,verdict,1024);

    close(got_out);
    //close(wfd);
    close(exp_out);
    close(w_code_fd);
    close(client_msg);
	close(fd);
	
	return 0;
}

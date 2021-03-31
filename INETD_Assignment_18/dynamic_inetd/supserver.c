#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include <string.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stddef.h>
#include <sys/stat.h>

#define NO_OF_REQUEST 10
#define MAXSERVICES 255

void encrypt(char* s)
{
	for(int i=0;i<strlen(s);i++)
	{
		s[i] = s[i]-'a'+'1';
	}
}

int ports[MAXSERVICES] = {0};
char* operations[MAXSERVICES] = {NULL};
char* type[MAXSERVICES] = {NULL};
struct sockaddr_in addr[MAXSERVICES];
int used = 0;
int sfd[MAXSERVICES];
FILE* fp;
fd_set readset;
int m = -1;

void sig_handler(int alrm)
{
	char buffer[255];
	while(fscanf(fp,"%[^\n]s",buffer)==1)
	{
		while(fgetc(fp)!='\n'&&fgetc(fp)!=EOF);
		printf("reading: %s\n",buffer);
		char* tok = strtok(buffer," ");
		tok = strtok(NULL," ");
		type[used] = strdup(tok);
		printf("Type: %s\n",type[used]);
		if(strcmp(type[used],"tcp")==0)
		{
			sfd[used] = socket(AF_INET,SOCK_STREAM,0);
			if(sfd[used]<0)
			{
				perror("socket not created");
				exit(0);
			}
		}
		else
		{
			sfd[used] = socket(AF_INET,SOCK_DGRAM,0);
			if(sfd[used]<0)
			{
				perror("socket not created");
				exit(0);
			}
		}
        int opt=1;
        if(setsockopt(sfd[used], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
        { 
            perror("setsockopt"); 
            exit(1); 
        }
		tok = strtok(NULL," ");
		tok = strtok(NULL," ");
		printf("Port No: %s\n",tok);
		addr[used].sin_family = AF_INET;
		addr[used].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		addr[used].sin_port = htons(atoi(tok));
		if(bind(sfd[used],(struct sockaddr*)&addr[used],sizeof(addr[used]))<0)
		{
			perror("bind!");
			exit(0);
		}
		if(strcmp(type[used],"tcp")==0)
		{
			listen(sfd[used],NO_OF_REQUEST);
		}
		m = (m<sfd[used])?sfd[used]:m;
		FD_SET(sfd[used],&readset);
		tok = strtok(NULL," ");
		operations[used] = strdup(tok);
		used++;
	}
	signal(SIGUSR1,sig_handler);
	printf("Updated successfully..\nm = %d\n",m);
}

int main()
{
	signal(SIGUSR1,sig_handler);
	char buffer[255];
	FD_ZERO(&readset);
	fp = fopen("config.txt","r");
	if(fp==NULL)
	{
		perror("Config_file");
		exit(1);
	}
	fscanf(fp,"%[^\n]s",buffer);
	while(fgetc(fp)!='\n'&&fgetc(fp)!=EOF);
	
    sig_handler(10);
	
    printf("m = %d\n",m);
	
    int r,nsfd;
	
    pid_t p;
	
    while(1)
	{
		printf("Waiting...\n");
		r = select(m+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			for(int i=0;i<used;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					if(strcmp(type[i],"tcp")==0)
					{
                        printf("TCP Service!\n");
						nsfd = accept(sfd[i],NULL,NULL);
						if(nsfd<0)
						{
							perror("Accept");
						}
						else
						{
							p = fork();
							if(p<0)
							{
								perror("Fork");
							}
							else if(p>0)
							{
								close(nsfd);
								printf("Operation: %s\n",operations[i]);
							}
							else
							{
								close(sfd[i]);
								dup2(nsfd,0);
								dup2(nsfd,1);
								char* arg[1]= {NULL};
								execv(operations[i],arg);
                                exit(0);
							}
						}
					}
					else
					{
						printf("UDP Service!\n");
						struct sockaddr_in cl_addr;
						int len = sizeof(cl_addr),sz;
						char mesg[1024];
						if((sz = recvfrom(sfd[i],mesg,1024,0,(struct sockaddr*)&cl_addr,&len))<0)
						{
							perror("Receive");
						}
						else
						{
							mesg[sz] = '\0';
							printf("Received: %s\nProcessing...\n",mesg);
							encrypt(mesg);
							printf("Sending...\n");
							if(sendto(sfd[i],mesg,strlen(mesg),0,(struct sockaddr*)&cl_addr,len)<0)
							{
								perror("Send");
							}
							else
							{
								printf("Sent successfully....\n");
							}
						}
					}
				}
				else
				{
					FD_SET(sfd[i],&readset);
				}
			}
		}
	}
	return 0;
}
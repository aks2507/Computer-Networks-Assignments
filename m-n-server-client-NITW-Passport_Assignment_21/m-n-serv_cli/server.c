#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <pthread.h>

int nsfd[255],ind = 0;

void* func1(void* arg) // Uppercase
{
	int fd = *(int*)arg,sz;
	char buffer[50];
	while(1)
	{
		if((sz = recv(fd,buffer,50,0))<0)
			perror("Could not read");
		else
		{
			if(sz==0)
				pthread_exit(0);
			buffer[sz] = '\0';
			for(int i=0;i<sz;i++)
			{
				if(buffer[i]>='a'&&buffer[i]<='z')
					buffer[i] = buffer[i]-'a'+'A';
			}
			if(send(fd,buffer,sz,0)<0)
				perror("Could not send");
		}
	}
}

void* func2(void* arg) //Lowercase
{
	int fd = *(int*)arg,sz;
	char buffer[50];
	while(1)
	{
		if((sz = recv(fd,buffer,50,0))<0)
			perror("Could not read");
		else
		{
			if(sz==0)
				pthread_exit(0);
			buffer[sz] = '\0';
			for(int i=0;i<sz;i++)
			{
				if(buffer[i]>='A'&&buffer[i]<='Z')
					buffer[i] = buffer[i]-'A'+'a';
			}
			if(send(fd,buffer,sz,0)<0)
				perror("Could not send");
		}
	}
}

void* func3(void* arg) // Number
{
	int fd = *(int*)arg,sz;
	char buffer[50];
	while(1)
	{
		if((sz = recv(fd,buffer,50,0))<0)
			perror("Could not read");
		else
		{
			if(sz==0)
				pthread_exit(0);
			buffer[sz] = '\0';
			for(int i=0;i<sz;i++)
			{
				if(buffer[i]>='a'&&buffer[i]<='z')
					buffer[i] = buffer[i]-'a'+'1';
			}
			if(send(fd,buffer,sz,0)<0)
				perror("Could not send");
		}
	}
}

char reply[100];
fd_set readset;

int main(int argc, char const *argv[])
{
	if(argc<4)
	{
		printf("Usage: %s [SERVER NUMBER] [STARTING PORT NUMBER] [ENDING PORT NUMBER]\n",argv[0]);
		exit(0);
	}
	int port_start = atoi(argv[2]),port_end = atoi(argv[3]);
	int n = port_end-port_start+1,j = 0,temp = 1;
	int sfd[n];
	struct sockaddr_in addr[n];
	FD_ZERO(&readset);
	for(int i=port_start;i<=port_end;i++)
	{
		sfd[j] = socket(AF_INET,SOCK_STREAM,0);
		if(sfd[j]<0)
		{
			perror("socket");
			continue;
		}
		if(setsockopt(sfd[j],SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&temp,sizeof(temp))<0)
		{
			perror("setsockopt");
			exit(1);
		}
		addr[j].sin_family = AF_INET;
		addr[j].sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		addr[j].sin_port = htons(i);
		if(bind(sfd[j],(struct sockaddr*)&addr[j],sizeof(addr[j]))<0)
		{
			perror("bind");
			exit(1);
		}
		else if(listen(sfd[j],10)<0)
		{
			perror("listen");
			exit(1);
		}
		else
		{
			FD_SET(sfd[j],&readset);
			j++;
		}

	}
	

	int rsfd = socket(AF_INET,SOCK_RAW,253),r,optval = 1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));
	struct sockaddr_in rawaddr,cl_addr;int len = sizeof(cl_addr);
	memset(&rawaddr,0,sizeof(rawaddr));
	rawaddr.sin_family = AF_INET;
	rawaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(bind(rsfd,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
	{
		perror("bind");
	}
	else
	{
		FD_SET(rsfd,&readset);
	}
	char buffer[100];
	while(1)
	{
		r = select(FD_SETSIZE+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			if(FD_ISSET(rsfd,&readset))
			{
				if(recvfrom(rsfd,buffer,100,0,(struct sockaddr*)&cl_addr,&len)<0)
				{
					perror("receive");
				}
				struct iphdr *ip;
				ip = (struct iphdr*)buffer;
				char ad[INET_ADDRSTRLEN];
				printf("Remote IP: %s\n",inet_ntop(AF_INET,&ip->saddr,ad,INET_ADDRSTRLEN));
				sprintf(reply,"Ports available in Server-%s : %s - %s",argv[1],argv[2],argv[3]);
				if(sendto(rsfd,reply,strlen(reply)+1,0,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
				{
					perror("send");
				}
			}
			else
				FD_SET(rsfd,&readset);
			for(int i=0;i<n;i++)
			{
				if(FD_ISSET(sfd[i],&readset))
				{
					nsfd[ind] = accept(sfd[i],NULL,NULL);
					if(nsfd[ind]<0)
					{
						perror("Could not accept");continue;
					}
					pthread_t p;
					if(argv[1][0]=='1')
					{
						pthread_create(&p,NULL,func1,&nsfd[ind]);
					}
					else if(argv[1][0]=='2')
					{
						pthread_create(&p,NULL,func2,&nsfd[ind]);
					}
					else
						pthread_create(&p,NULL,func3,&nsfd[ind]);
					ind++;
				}
				else
					FD_SET(sfd[i],&readset);
			}
		}
	}
	return 0;
}
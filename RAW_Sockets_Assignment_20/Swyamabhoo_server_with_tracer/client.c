#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>
#define KEYPATH "."
#define CONTROLLEN CMSG_LEN(sizeof(int))
#define PATH "/tmp/file.socket"

void m(char* s)
{
	printf("%s success!!\n",s);
}

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: %s [PORT_NO]\n",argv[0]);
		exit(0);
	}
	else
		m("user_execution");

	int sfd;
	if((sfd = socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		perror("udp_socket");
		exit(1);
	}
	else
		m("udp_socket");
	struct sockaddr_in t_addr;
	t_addr.sin_family = AF_INET;
	t_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	t_addr.sin_port = htons(8003);
	char c = '?';
	int rpl;
	if(sendto(sfd,&c,sizeof(c),0,(struct sockaddr*)&t_addr,sizeof(t_addr))<0)
	{
		perror("sendto");
		exit(1);
	}
	else
		m("sendto");
	printf("Available ports are:\n");
	while(1)
	{
		if(recvfrom(sfd,&rpl,sizeof(rpl),0,NULL,NULL)<0)
		{
			perror("recvfrom");
			exit(1);
		}
		else
			m("recvfrom");
		if(rpl==-1)
			break;
		printf("%d ,",rpl);
	}
	
	if((sfd = socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("tcp_socket");
		exit(1);
	}
	else
		m("tcp_socket");
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(atoi(argv[1]));
	if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("connect");
		exit(1);
	}
	else
		printf("Connected\n");
	char buffer[50];
	while(1)
	{
		printf("Enter a string\n");
		scanf("%[^\n]s",buffer);
		while(getchar()!='\n');
		send(sfd,buffer,strlen(buffer),0);
		int sz = read(sfd,buffer,50);
		buffer[sz] = '\0';
		printf("Reading: %s\n",buffer);
	}
	return 0;
}
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

struct sockaddr_in rawaddr,cl_addr;

int main(int argc, char const *argv[])
{
	int rsfd = socket(AF_INET,SOCK_RAW,253),sz,optval = 1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));
	memset(&rawaddr,0,sizeof(rawaddr));
	rawaddr.sin_family = AF_INET;
	rawaddr.sin_addr.s_addr = inet_addr("127.0.0.2");
	cl_addr.sin_family = AF_INET;
	cl_addr.sin_addr.s_addr = inet_addr("127.0.0.3");
	if(bind(rsfd,(struct sockaddr*)&cl_addr,sizeof(cl_addr))<0)
		perror("bind");
	int n = atoi(argv[0]);
	char buffer[100];
	strcpy(buffer,"?");
	if(sendto(rsfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&rawaddr,sizeof(rawaddr))<0)
	{
		perror("send");
	}
	else
	{
		for(int i=0;i<n;i++)
		{
			if(recvfrom(rsfd,buffer,100,0,NULL,NULL)<0)
			{
				perror("recvfrom");
			}
			else
			{
				struct iphdr *ip;
				ip = (struct iphdr*)buffer;
				printf("Receiving info from server: %s\n",buffer+(ip->ihl*4));
			}
		}
		printf("Enter the port needed for connection:\n");
		int portno;
		scanf("%d",&portno);while(getchar()!='\n');
		int sfd = socket(AF_INET,SOCK_STREAM,0);
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		addr.sin_port = htons(portno);
		if(connect(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
		{
			perror("Could not connect");
		}

		while(1)
		{
			printf("Enter input\n");
			scanf("%[^\n]s",buffer);
			while(getchar()!='\n');
			send(sfd,buffer,strlen(buffer),0);
			sz = recv(sfd,buffer,100,0);
			buffer[sz] = '\0';
			printf("Receiving: %s\n",buffer);
		}
	}
	return 0;
}
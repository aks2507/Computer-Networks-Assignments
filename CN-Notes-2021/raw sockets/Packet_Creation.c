#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in_systm.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include<string.h>
#define MSG_LEN 10
#if !defined(IPVERSION)
#define IPVERSION 4
#endif 
int main(int argc,char *argv[])
{
	unsigned char packet[
	sizeof(struct iphdr)+
	sizeof(struct udphdr)+MSG_LEN];
	unsigned char *k;
	struct in_addr saddr,daddr;
	struct sockaddr_in clin;
	struct udphdr *udp;
	struct iphdr *iphdr;
	unsigned short sport,dport;
	int sfd,on=1,n;
	char *p="Message";
	
  	if(argc < 5)  {
  	  fprintf(stderr,"usage: %s source_port source_address dest_port dest_address\n",
		    argv[0]);
  	  exit(1);
  	}
	
	sport=(unsigned short)atoi(argv[1]);
	saddr.s_addr=inet_addr(argv[2]);

	dport=(unsigned short)atoi(argv[3]);
	daddr.s_addr=inet_addr(argv[4]);

	sfd=socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
	setsockopt(sfd,IPPROTO_IP,IP_HDRINCL,(char *)&on,sizeof(on));

	
	iphdr=(struct iphdr *)packet;
	memset((char *)iphdr,'\0',sizeof(struct iphdr));
	iphdr->ihl=5;
	iphdr->ttl=60;
	iphdr->version=IPVERSION;
	iphdr->tot_len=htons(sizeof(packet));
	iphdr->id=htons(getpid());
	iphdr->protocol=IPPROTO_UDP;
	iphdr->saddr=saddr.s_addr;
	iphdr->daddr=daddr.s_addr;
	iphdr->check=0;

	udp = (struct udphdr *)(packet+sizeof(struct iphdr));
	memset((packet+sizeof(struct udphdr)+sizeof(struct iphdr)),'0',MSG_LEN);
	udp->source = htons(sport);
  	udp->dest = htons(dport);
  	udp->len = htons(sizeof(struct udphdr)+MSG_LEN);
  	udp->check = 0;
	memcpy((packet+sizeof(struct iphdr)+sizeof(struct udphdr)),p,sizeof(p)+1);	

	//bzero(&clin,sizeof(clin));
	 memset(&clin,'\0',sizeof(clin));
	clin.sin_family=AF_INET;
	clin.sin_port=htons(dport);
	clin.sin_addr=daddr;
	
if((n=sendto(sfd,&packet,sizeof(packet),0x0,(struct sockaddr*)&clin,sizeof(clin)))!=sizeof(packet))
{
	printf("Error occurred n:%d\n",n);	
	exit(1);
}
return 0;
}
	

	

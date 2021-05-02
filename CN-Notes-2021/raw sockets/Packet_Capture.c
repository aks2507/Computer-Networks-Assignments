#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/in_systm.h>
//#include <linux/ip.h>
//#include <linux/udp.h>
#include<string.h>
#include <netinet/if_ether.h> 
#include<net/ethernet.h>
/*for ethernet*/
#define SNP_LEN 1024
#define CMD "udp and src host %s and src port %d"
void my_callback(u_char *args,const struct pcap_pkthdr *header,const u_char *packet);
int main(int argc,char *arv[])
{
	char *dev=NULL;
	char errbuf[PCAP_ERRBUF_SIZE];		
	pcap_t *handle;	
	char filter[]="ip";
	struct bpf_program fp;			
	uint32_t net,mask;
	int np=10;
	
	dev=pcap_lookupdev(errbuf);
	pcap_lookupnet(dev,&net,&mask,errbuf);
	
	printf("Device: %s\n", dev);
	printf("Number of packets: %d\n", np);
	printf("Filter expression: %s\n", filter);	

	handle=pcap_open_live(dev,SNP_LEN,1,1000,errbuf);
	
	if (pcap_datalink(handle) != DLT_EN10MB) 
	{
	 	fprintf(stderr, "%s is not an Ethernet\n", dev);
		exit(1);
	}
	pcap_compile(handle,&fp,filter,0,net);
	pcap_setfilter(handle,&fp);
	pcap_loop(handle,np,my_callback,NULL);
	
	pcap_freecode(&fp);
	pcap_close(handle);
		
	printf("Capture Complete\n");
	return 0;
}
void my_callback(u_char *args,const struct pcap_pkthdr *header,const u_char *packet)
{
	const struct ether_header *ehdr;
	struct ip *iphdr;
	const struct udphdr *udp;
	
	ehdr=(struct ether_header *)packet;
	iphdr=(struct ip *)(packet+14);
	
	printf("       From: %s\n", inet_ntoa(iphdr->ip_src));
	printf("         To: %s\n", inet_ntoa(iphdr->ip_dst));
	
	if(iphdr->ip_p==IPPROTO_UDP)
	{
		printf("UDP\n");
 		printf("Message:%s\n",(packet+sizeof(struct ether_header)+sizeof(struct ip)
		+sizeof(struct udphdr)));	
	}
	return;
}

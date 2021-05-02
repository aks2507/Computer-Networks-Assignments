
#define __USE_BSD	/* use bsd'ish ip header */
#include <sys/socket.h>	
#include <netinet/in.h>	
#include <netinet/ip.h>
#define __FAVOR_BSD	/* use bsd'ish tcp header */
#include <netinet/tcp.h>
#include <unistd.h>

#define P 9013	

int 
main (void)
{
 
 int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);	


int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Warning: Cannot set HDRINCL!\n");

  char datagram[4096];	

  struct ip *iph = (struct ip *) datagram;

  struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);

  struct sockaddr_in sin,sin2; socklen_t len = sizeof(sin2) ;

memset(&sin,0,sizeof(sin)) ;

  sin.sin_family = AF_INET;
  sin.sin_port = htons (P);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(s,(struct sockaddr *) &sin,sizeof(sin)) ;
 
   
      if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	printf ("error\n");
      
	 datagram[n] = 0 ; printf("\n\n\t Headers have ben received, as : ");
          printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d  %d %d %d %d %d  %d    ",iph->ip_hl,iph->ip_v ,  iph->ip_tos,  iph->ip_len,  iph->ip_id ,  iph->ip_off,  iph->ip_ttl,  iph->ip_p,  iph->ip_sum,  iph->ip_src.s_addr,  iph->ip_dst.s_addr,  tcph->th_sport,  tcph->th_dport,  tcph->th_seq,  tcph->th_ack,  tcph->th_x2 ,  tcph->th_off,  tcph->th_flags,  tcph->th_win,  tcph->th_sum,  tcph->th_urp) ;
        


  return 0;
}


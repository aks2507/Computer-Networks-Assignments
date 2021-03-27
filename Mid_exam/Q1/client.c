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

#define PORT1 5000
#define PORT2 6000
#define PORT3 7000
#define PORT4 8000

int cli_connect(int port)
{
    int sfd;
	  
    struct sockaddr_in servaddr; 

    int n, len; 
    // Creating socket file descriptor 
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("socket creation failed"); 
        exit(0); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
  
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    { 
        printf("\n Error : Connect Failed \n"); 
    }  
    return sfd;
}
int main()
{
    int inp;
    printf("Select the server(1/2/3/4)??: ");
    scanf("%d",&inp);
    int sfd;
    if(inp==1)
    {
        sfd = cli_connect(PORT1);
    }
    else if(inp==2)
    {
        sfd = cli_connect(PORT2);
    }
    else if(inp==3)
    {
        sfd = cli_connect(PORT3);
    }
    else
    {
        sfd = cli_connect(PORT4);
    }
    
    char buffer[1024];
    while(1)
    {
        memset(buffer, 0, sizeof(buffer)); 
        fgets(buffer,sizeof(buffer),stdin);
        //strcpy(buffer, "Hello Server"); 
        write(sfd, buffer, sizeof(buffer)); 
        printf("Message from server: "); 
        read(sfd, buffer, sizeof(buffer));
        printf("%s\n",buffer); 
        //puts(buffer);
    }
    close(sfd); 
}
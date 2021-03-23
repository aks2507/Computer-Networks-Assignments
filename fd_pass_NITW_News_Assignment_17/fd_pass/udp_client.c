#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#define PORT 7000 

int main()
{
	int usfd; 
    char buffer[1024]; 
    //char* message = "Hello Server";
    char message[1024]; 
    struct sockaddr_in servaddr,recvaddr;

    int n, len; 
    // Creating socket file descriptor 
    if ((usfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        printf("socket creation failed"); 
        exit(0); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 

    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    fgets(message,sizeof(message),stdin);

    sendto(usfd, (const char*)message, strlen(message), 
           0, (const struct sockaddr*)&servaddr, 
           sizeof(servaddr)); 
  
    // receive server's response 
    printf("Message from server: "); 
    n = recvfrom(usfd, (char*)buffer, 1024, 
                 0, (struct sockaddr*)&recvaddr, 
                 &len);
    char buf[len];
	inet_ntop( AF_INET, &recvaddr.sin_addr, buf, sizeof( buf ));
	printf( "%s\n", buf );
    printf("%s\n",buffer); 
    //puts(buffer); 
    close(usfd);  
    return 0;
}
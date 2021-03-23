#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h> 
#define PORT 6000 

int main()
{
	int sfd;
	char buffer[1024];  
    struct sockaddr_in servaddr; 

    int n, len; 
    // Creating socket file descriptor 
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("socket creation failed"); 
        exit(0); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
  
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sfd, (struct sockaddr*)&servaddr,  
                             sizeof(servaddr)) < 0) { 
        printf("\n Error : Connect Failed \n"); 
    }  

    memset(buffer, 0, sizeof(buffer)); 
    fgets(buffer,sizeof(buffer),stdin);
    //strcpy(buffer, "Hello Server"); 
    write(sfd, buffer, sizeof(buffer)); 
    printf("Message from server: "); 
    read(sfd, buffer, sizeof(buffer));
    printf("%s\n",buffer); 
    //puts(buffer); 
    close(sfd); 
}
#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#define PORT 5000
#define PORT2 6000
#define PORT3 7000 

int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 

int main()
{
	int sfd, nsfd, nsfd2 ,usfd, nready, maxfdp1; 
    char buffer[1024]; 
    pid_t childpid; 
    fd_set rset; 
    ssize_t n; 
    socklen_t len; 
    const int on = 1; 
    struct sockaddr_in cliaddr, servaddr; 
    char* message = "Hello Client"; 
    
    /* create listening TCP socket */
    sfd = socket(AF_INET, SOCK_STREAM, 0); 
    int sfd2 = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    bind(sfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 


    servaddr.sin_port = htons(PORT2); 
    bind(sfd2, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    listen(sfd, 10); 
    listen(sfd2,10);

    /* create UDP socket */
    usfd = socket(AF_INET, SOCK_DGRAM, 0); 
    servaddr.sin_port = htons(PORT3);
    bind(usfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

    FD_ZERO(&rset);  //clear reading set
    int maxfd = max(sfd, usfd); 
    maxfdp1 = max(maxfd,sfd2)+1;

    while(1)
    {
    	FD_SET(sfd, &rset); 
        FD_SET(usfd, &rset);
        FD_SET(sfd2,&rset);
        
        nready = select(maxfdp1, &rset, NULL, NULL, NULL); 

        if (FD_ISSET(sfd, &rset)) 
        { 
            len = sizeof(cliaddr); 
            nsfd = accept(sfd, (struct sockaddr*)&cliaddr, &len); 
            if ((childpid = fork()) == 0) 
            { 
                close(sfd); 
                bzero(buffer, sizeof(buffer)); 
                printf("Message From TCP client: "); 
                read(nsfd, buffer, sizeof(buffer)); 
                //puts(buffer); 
                printf("%s\n",buffer);
                write(nsfd, (const char*)message, sizeof(buffer)); 
                close(nsfd); 
                exit(0); 
            } 
            close(nsfd); 
        } 

        if (FD_ISSET(sfd2, &rset)) 
        { 
            len = sizeof(cliaddr); 
            nsfd2 = accept(sfd2, (struct sockaddr*)&cliaddr, &len); 
            if ((childpid = fork()) == 0) { 
                close(sfd2); 
                bzero(buffer, sizeof(buffer)); 
                printf("Message From TCP client2: "); 
                read(nsfd2, buffer, sizeof(buffer)); 
                //puts(buffer); 
                printf("%s\n",buffer);
                write(nsfd2, (const char*)message, sizeof(buffer)); 
                close(nsfd2); 
                exit(0); 
            } 
            close(nsfd2); 
        } 

        // if udp socket is readable receive the message. 
        if (FD_ISSET(usfd, &rset)) 
        { 
            len = sizeof(cliaddr); 
            bzero(buffer, sizeof(buffer)); 
            printf("\nMessage from UDP client: "); 
            n = recvfrom(usfd, buffer, sizeof(buffer), 0, 
                         (struct sockaddr*)&cliaddr, &len); 
            //puts(buffer);
            printf("%s\n",buffer); 
            sendto(usfd, (const char*)message, sizeof(buffer), 0, 
                   (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
        } 
    }
}
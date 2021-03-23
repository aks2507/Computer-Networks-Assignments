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
#define MAXLINE 1024 
int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    return y; 
} 
int main() 
{ 
    int tcpfd, tcp_nsfd, udpfd, nready, maxfdp1;
    char buffer[MAXLINE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    char* message = "Hello Client";
    void sig_chld(int);
  
    /* create listening TCP socket */
    tcpfd = socket(AF_INET, SOCK_STREAM, 0); 

    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // binding server addr structure to tcpfd 
    bind(tcpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    listen(tcpfd, 10); 
  
    /* create UDP socket */
    udpfd = socket(AF_INET, SOCK_DGRAM, 0); 
    // binding server addr structure to udp sockfd 
    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
  
    // clear the descriptor set 
    FD_ZERO(&rset); 
  
    // get maxfd 
    maxfdp1 = max(tcpfd, udpfd) + 1; 
    while(1) 
    { 
  
        // set tcpfd and udpfd in readset 
        FD_SET(tcpfd, &rset); 
        FD_SET(udpfd, &rset); 
  
        // select the ready descriptor 
        nready = select(maxfdp1, &rset, NULL, NULL, NULL); 
  
        // if tcp socket is readable then handle 
        // it by accepting the connection 
        if (FD_ISSET(tcpfd, &rset)) 
        { 
            len = sizeof(cliaddr); 
            tcp_nsfd = accept(tcpfd, (struct sockaddr*)&cliaddr, &len); 
            if ((childpid = fork()) == 0) 
            { 
                close(tcpfd); 
                bzero(buffer, sizeof(buffer)); 
                printf("Message From TCP client: "); 
                read(tcp_nsfd, buffer, sizeof(buffer)); 
                puts(buffer); 
                write(tcp_nsfd, (const char*)message, sizeof(buffer)); 
                close(tcp_nsfd); 
                exit(0); 
            } 
            close(tcp_nsfd); 
        } 
        // if udp socket is readable receive the message. 
        if (FD_ISSET(udpfd, &rset)) 
        { 
            len = sizeof(cliaddr); 
            bzero(buffer, sizeof(buffer)); 
            printf("\nMessage from UDP client: "); 
            n = recvfrom(udpfd, buffer, sizeof(buffer), 0, 
                         (struct sockaddr*)&cliaddr, &len);
            puts(buffer);
            sendto(udpfd, (const char*)message, sizeof(buffer), 0, 
                   (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        }
    }
}
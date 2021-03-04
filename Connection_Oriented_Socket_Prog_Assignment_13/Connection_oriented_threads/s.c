#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h> 
#include <poll.h>
#include <pthread.h>
#define PORT1 37778
#define PORT2 37779

struct sockaddr_in serv_addr, cli_addr;
char buffer2[1024]={0};

int* socket_creation(int sfd[2])
{
    if((sfd[0] = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket");
        exit(1);
    }
	if((sfd[1] = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket");
        exit(1);
    }
    return sfd;
} 

void set_socket_options(int sfd1, int sfd2,int opt)
{
    if((setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) 
    {
        perror("setsockopt");
        exit(1);
    }
	if((setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) 
    {
        perror("setsockopt");
        exit(1);
    }
}

void binding(int sfd1,int sfd2)
{
    serv_addr.sin_port = htons(PORT1);
    if(bind(sfd1, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("bind");
        exit(1);
    }
	serv_addr.sin_port = htons(PORT2);
	if(bind(sfd2, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("bind");
        exit(1);
    }
}

void listening_requests(int sfd1,int sfd2)
{
    if(listen(sfd1, 5) < 0) 
    {
        perror("listen");
        exit(1);
    }
    if(listen(sfd2, 5) < 0) 
    {
        perror("listen");
        exit(1);
    }
}

void *thread_func(void *i)
{
    int nsfd = *((int *) i);
    printf("Started thread\n");
    char buffer [1024] = "1:";
    int read = recv( nsfd , buffer2, 1024, 0);
    buffer2[read] = '\0';
    printf("message received : %s\n",buffer2);
    fgets(buffer2,1024,stdin);
    strcat(buffer,buffer2);
    send(nsfd , buffer , strlen(buffer), 0 );
    buffer[strlen(buffer)] = '\0';
    printf("message sent : %s\n",buffer);
}

int main()
{
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    
    int sfd1,sfd2;
    int sfd[2]={0};

    socket_creation(sfd);
    sfd1=sfd[0];
    sfd2=sfd[1];

    int addrlen = sizeof(cli_addr); 
    int opt = 1;
    
    set_socket_options(sfd1,sfd2,opt);

    binding(sfd1,sfd2);

    listening_requests(sfd1,sfd2);

    printf("Server Started....\n");
    while(1)
    {
    	struct pollfd fds[2];
    	fds[0].fd = sfd1;
        fds[0].events = POLLIN;
		fds[0].revents = 0;
		fds[1].fd = sfd2;
        fds[1].events = POLLIN;
		fds[1].revents = 0;
        
        poll(fds, 2, -1);

        if(fds[0].revents & POLLIN)
        {
            int nsfd;
	    	if((nsfd = accept(sfd1, (struct sockaddr *)&cli_addr, (socklen_t*)&addrlen))<0) 
		    { 
		        perror("accept"); 
		        exit(1); 
		    } 
		    printf("New Client connected to S1\n");
            pthread_t t1;
            void* ns=&nsfd;
            pthread_create(&t1,NULL,thread_func,ns);
		    //pthread_join(t1,NULL);
        }
        if(fds[1].revents & POLLIN)
        {
        	int nsfd;
	    	if((nsfd = accept(sfd2, (struct sockaddr *)&cli_addr, (socklen_t*)&addrlen))<0) 
		    { 
		        perror("accept"); 
		        exit(1); 
		    } 
		    printf("New Client connected to S2\n");
		    pthread_t t2;
            void* ns=&nsfd;
            pthread_create(&t2,NULL,thread_func,ns);
		    //pthread_join(t2,NULL);
        }
    }
}
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include<stdlib.h>
#include <string.h> 
#define PORT 8080
struct sockaddr_in serv_addr, cli_addr;
int main()
{
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    int sfd;
    int addrlen = sizeof(cli_addr); 
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("socket");
        exit(1);
    }

    if(bind(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("bind");
        exit(1);
    }

    if(listen(sfd, 3) < 0) 
    {
        perror("listen");
        exit(1);
    }
    printf("Server is operational!!\n");	
    while(1)
    {
    	int nsfd;
    	if((nsfd = accept(sfd,(struct sockaddr *)&cli_addr,(socklen_t*)&addrlen))<0) 
	    { 
	        perror("accept"); 
	        exit(1); 
	    } 
	    int c = fork();
	    if(c==0)
	    {
            printf("New Client connection established!!\n");
            char buf[1024];
            while(1)
            {
                read(nsfd,buf,sizeof(buf));
                printf("%s\n",buf);
            }
	    }
    }
    return 0;

}

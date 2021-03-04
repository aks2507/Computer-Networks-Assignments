#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT1 37778
#define PORT2 37779
struct sockaddr_in serv_addr;
int main()
{
	int csfd;
	if ((csfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    int num;
    printf("Enter service no:\n");
    fflush(stdout);
    scanf("%d",&num);
    char ch; 
    scanf("%c",&ch); //extra \n character removal

    serv_addr.sin_family = AF_INET; 
    if(num==1) 
        serv_addr.sin_port = htons(PORT1); 
    else 
        serv_addr.sin_port = htons(PORT2);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
     if (connect(csfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    
    while(1)
    {
        char buf[1024]; 
        printf("Enter client input:\n");
        fflush(stdout);
        fgets(buf,sizeof(buf),stdin);
        if(buf[0]=='@')
            break;
        send(csfd,buf,strlen(buf)+1,0);
        printf("Message sent!\n");
        fflush(stdout);
    } 
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
    char buf1[128],buf2[128];
    //char* x1,*x2;
    int fd1=dup(STDIN_FILENO);
    int fd2=dup(STDOUT_FILENO);
    
    dup2(7,STDIN_FILENO);
    dup2(8,STDOUT_FILENO);
    
    //printf("\n And in file 2\n");
    read(fd1,buf2,128);
    
    printf("Inp2: %s \n",buf2);
    printf("Writing to p1:");
    fgets(buf1,128,stdin);
    
    write(fd2,buf1,128);
    
    close(fd1);
    close(fd2);
    return 0;
}

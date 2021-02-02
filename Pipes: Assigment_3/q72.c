#include <stdio.h>
#include <unistd.h>
#include <string.h> 
int main()
{
    fputs("p2 ",stdout);
    FILE* fd1=popen("./q73","r");
	char buf[1024];
	read(fileno(fd1),buf,1024);
	fputs("From pipe3: ",stdout);
	fputs(buf,stdout);
	pclose(fd1);
    return 0;
}

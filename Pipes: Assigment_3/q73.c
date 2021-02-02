#include <stdio.h>
#include <unistd.h>
#include <string.h> 
int main()
{
    fputs("p3 ",stdout);
    FILE* fd2=popen("./q74","r");
	char buf[1024];
	read(fileno(fd2),buf,1024);
	fputs("From pipe4: ",stdout);
	fputs(buf,stdout);
	pclose(fd2);
    return 0;
}

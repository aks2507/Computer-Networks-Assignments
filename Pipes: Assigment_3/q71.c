#include <stdio.h>
#include <unistd.h>
#include <string.h> 
int main()
{
    printf("p1 ");
    FILE* fd=popen("./q72","r");
	char buf[1024];
	read(fileno(fd),buf,1024);
	printf("From pipe2: %s",buf);
	pclose(fd);
    return 0;
}

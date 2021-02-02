#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	write(1,"From P2: Hello World!!\n",1024);
	return 0;
}

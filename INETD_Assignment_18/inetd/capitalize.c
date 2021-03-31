#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<unistd.h>

int main()
{
    char buff[1024];
    read(0,buff,sizeof(buff));
    int i=0;
    while(buff[i]!='\0')
    {
        if(buff[i]>='a' && buff[i]<='z')
        {
            buff[i]=toupper(buff[i]);
        }
        i++;
    }
    printf("%s",buff);
    return 0;
}
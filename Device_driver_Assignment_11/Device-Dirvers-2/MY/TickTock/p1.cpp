#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
char arr[100];
int main()
{
	int fd=open("/dev/TickTock",O_RDWR);
	int i=read(fd,arr,30);arr[i]='\0';
	cout<<arr<<endl;
	arr[5]='7';
	i=write(fd,arr,30);
	cout<<arr<<" "<<i<<endl;
	return 0;
}
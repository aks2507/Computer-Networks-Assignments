#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<string>
using namespace std;
int main()
{
	int a[2],b[2];
	pipe(a);
	pipe(b);
	
	int c=fork();
	if(c>0)
	{
		close(a[0]);
		close(b[1]);
		int i=1;
		while(i<5)
		{
			char buff1[128];
			cout<<"write message to be sent by parent:\n";
			fgets(buff1,128,stdin);
			write(a[1],buff1,128);
			char buff3[128];
			cout<<"Read from pipe 2: ";
			read(b[0],buff3,128);
			cout<<buff3<<"\n";
			i++;
		}
	}
	else
	{
		close(a[1]);
		close(b[0]);
		int i=1;
		while(i<5)
		{
			char buff2[128];
			read(a[0],buff2,128);
			cout<<"Child receives: "<<buff2<<"\n";
			buff2[0]='W';
			write(b[1],buff2,128);
		}
	}
	return 0;
}


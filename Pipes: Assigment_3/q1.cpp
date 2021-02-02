#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<string>
#define n 128
using namespace std;

int main()
{
	int pp[2];
	pipe(pp);
	char str_w[n],str_r[n];
	fgets(str_w,n,stdin);
	write(pp[1],str_w,n);
	read(pp[0],str_r,n);
	cout<<str_r<<"\n";
	return 0;
}


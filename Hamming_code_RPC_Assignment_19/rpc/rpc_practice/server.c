#include<stdio.h>
#include<rpc/rpc.h>
#include "add.h"
addproc_1_svc(in *inp,struct svc_req *rqstp)
{
	static out fin;
	
	long int n1=inp->num1;
	long int n2=inp->num2
	long int sm1=0;
	long int sm2=0;
	while(n1)
	{
		sm1+=n1%10;
		n1/=10;
	}
	while(n2)
	{
		sm2+=n2%10;
		n2/=10;
	}
	fin->res=sm1+sm2;
	return (&out);
}

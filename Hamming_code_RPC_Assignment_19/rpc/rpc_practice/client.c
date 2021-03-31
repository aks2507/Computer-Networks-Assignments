#include<stdio.h>
#include<rpc/rpc.h>
#include "square.h"
int main()
{
	CLIENT *c1;
	in inp;
	out *outp;
	c1=clnt_create("127.0.0.1",ADD_PROG,ADD_VERS,"tcp");
	printf("Enter the number\n");
	scanf("%ld %ld",&inp.num1,&inp.num2);
	outp=addproc_1(&inp,c1);
	if(outp!=NULL)
		printf("The digit sum value is %d\n",outp->res);
	exit(0);
	return 0;
}

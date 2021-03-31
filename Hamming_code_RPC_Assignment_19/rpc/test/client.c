#include<stdio.h>
#include<rpc/rpc.h>
#include "square.h"
int main()
{
	CLIENT *c1;
	square_in in;
	square_out *outp;
	c1=clnt_create("127.0.0.1",SQUARE_PROG,SQUARE_VERS,"tcp");
	printf("Enter the number\n");
	scanf("%ld",&in.arg1);
	outp=squareproc_1(&in,c1);
	if(outp!=NULL)
		printf("The square value is %d\n",outp->res1);
	exit(0);
	return 0;
}

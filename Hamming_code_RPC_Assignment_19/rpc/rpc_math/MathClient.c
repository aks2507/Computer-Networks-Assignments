#include <stdio.h>
#include "MathSpec.h"

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage: ./mathclient [HOST_NAME]\n");
		exit(0);
	}
	CLIENT* cl;
	operations ops,*rtn;
	cl = clnt_create(argv[1],MATHPROG,INITIAL,"tcp");
	while(1)
	{
		printf("Enter the operations\n");
		scanf("%d",&(ops.operation));
		switch(ops.operation)
		{
			case 1:
			case 2:
			case 3:
			case 4:
				printf("Enter two operands\n");
				scanf("%d%d",&(ops.op1),&(ops.op2));
				break;
			case 5:
				printf("Enter the length");
				scanf("%d",&(ops.len));
				printf("Enter the elements\n");
				for(int i=0;i<ops.len;i++)
					scanf("%d",&(ops.arr[i]));
				break;
		}
		if((rtn=mathproc_1(&ops,cl))==NULL)
		{
			perror("Could not connect");
			exit(0);
		}
		else
		{
			switch(rtn->operation){
				case 1:
				case 2:
				case 3:
				case 4:
				printf("Result: %d\n",rtn->result);
				break;
				case 5:
				for(int i=0;i<rtn->len;i++)
					printf("%d,", rtn->arr[i]);
				printf("\n");
				break;
			}
		}
		// mathproc_1(operations *, CLIENT *);
	}
	return 0;
}
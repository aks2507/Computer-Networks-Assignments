#include <stdio.h>
#include "MathSpec.h"

operations * mathproc_1_svc(operations *inp, struct svc_req *svc)
{
	static operations rtn;
	rtn = *inp;
	switch(rtn.operation)
	{
		case 1:
		rtn.result = rtn.op1+rtn.op2;
		break;
		case 2:
		rtn.result = rtn.op1-rtn.op2;
		break;
		case 3:
		rtn.result = rtn.op1*rtn.op2;
		break;
		case 4:
		rtn.result = rtn.op1/rtn.op2;
		case 5:
		for(int i=1;i<rtn.len;i++)
		{
			for(int j=i;j>0;j--)
			{
				if(rtn.arr[j]<rtn.arr[j-1])
				{
					rtn.arr[j] = rtn.arr[j]+rtn.arr[j-1];
					rtn.arr[j-1] = rtn.arr[j]-rtn.arr[j-1];
					rtn.arr[j] = rtn.arr[j]-rtn.arr[j-1];
				}
			}
		}
		break;
	}
	return &rtn;
}
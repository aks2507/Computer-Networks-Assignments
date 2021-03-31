struct operations
{
	int op1;
	int op2;
	int arr[20];
	int len;
	int result;
	int operation;	
};

program MATHPROG
{
	version INITIAL
	{
		operations MATHPROC(operations) = 1;
	} = 1;
} = 0x21111111;
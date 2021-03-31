struct in{
	long int num1;
	long int num2;
};
struct out{
	long int res;
};
program ADD_PROG
{
	version ADD_VERS
	{
		out ADDPROC(in)=1;
	} =1;
}= 0x3123000;

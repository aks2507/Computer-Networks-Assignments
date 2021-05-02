#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

static char* whom="world";
static int times=1,i;
module_param(whom,charp, S_IRUGO);
module_param(times,int, S_IRUGO);

static int hello_init(void)
{
	for(i=0;i<times;i++) printk(KERN_ALERT "hello %s \n",whom);
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT " good bye \n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KRSir");

//insmos hello.ko whom="bharath" times=10
// module_param_array(name,type,num,perm);//for accepting array in command line seperated by commas
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

#define KEY_STAT 0x60
#define KEY_DAT  0x64

u8 bit[7]={1,2,4,8,16,32,64};

static int hello_init(void)
{
	printk(KERN_ALERT "hello world \n");
	u8 val_stat,val_data;
	val_stat = inb(KEY_STAT);
	val_data = inb(KEY_DAT);
		val_stat|=bit[0];
		outb(val_stat,KEY_STAT);
 	printk(KERN_ALERT "%d %d\n",val_stat,val_data);
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
/*
 cat /proc/ioports
 cat /proc/iomem
*/
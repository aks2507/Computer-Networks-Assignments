#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

#define KEY_STAT 0x60
#define KEY_DAT  0x64

u8 bit[8]={1,2,4,8,16,32,64,128};

static int hello_init(void)
{
	printk(KERN_ALERT "hello world \n");
	u8 val_stat,val_data;
	/*val_stat = inb(KEY_STAT);
	val_data = inb(KEY_DAT);
		val_stat|=bit[0];
		outb(val_stat,KEY_STAT);
 	printk(KERN_ALERT "%d %d\n",val_stat,val_data);*/
 	while(1)
 	{
 		val_stat=inb(KEY_STAT);
 		if(!(val_stat&bit[1])) break;
 	}
 	val_data=167;
 	outb(val_data,KEY_DAT);
 	printk(KERN_ALERT " hello world2 \n");
	return 0;
}

static void hello_exit(void)
{
	u8 val_stat,val_data;
	while(1)
 	{
 		val_stat=inb(KEY_STAT);
 		if(!(val_stat&bit[1])) break;
 	}
 	val_data=168;
 	outb(val_data,KEY_DAT);
	printk(KERN_ALERT " good bye \n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KRSir");
/*
 cat /proc/ioports
 cat /proc/iomem
 rmmod key
*/
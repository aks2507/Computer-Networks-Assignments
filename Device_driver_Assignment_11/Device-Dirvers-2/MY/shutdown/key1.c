#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

#define KEY_STAT 0x60
#define KEY_DAT  0x64

u8 bit[8]={1,2,4,8,16,32,64,128};
/*
static void inp_empty()
{
	u8 val_stat;
	while(1){val_stat=inb(KEY_STAT);if(!(val_stat&bit[1])) break;}
}*/
static int hello_init(void)
{
	printk(KERN_ALERT "hello world \n");
	u8 val_stat,val_data;

 	while(1){val_stat=inb(KEY_STAT);if(!(val_stat&bit[1])) break;}
 	val_data=212;outb(val_data,KEY_DAT);//change to PS2
 	while(1){val_stat=inb(KEY_STAT);if(!(val_stat&bit[1])) break;}
 	val_data=244;outb(val_data,KEY_DAT);//to send back movements
 	while(1){val_stat=inb(KEY_STAT);if(!(val_stat&bit[1])) break;}

 	printk(KERN_ALERT " hello world2 \n");
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
 rmmod key
*/
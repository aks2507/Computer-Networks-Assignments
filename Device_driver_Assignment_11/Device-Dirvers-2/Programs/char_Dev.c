#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>

ssize_t (dev_read) (struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t (dev_write) (struct file *pfile, const char __user *buffer, size_t, loff_t *offset);
int (dev_open) (struct inode *pinode, struct file *pfile);
int (dev_close) (struct inode *pinode, struct file *pfile);



struct file_operations char_Dev_file_operations={
		.owner = THIS_MODULE,
		.open = dev_open,
		.read = dev_read,
		.write = dev_write,
		.release = dev_close,
};


ssize_t (dev_read) (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_ALERT " inside the read  function \n"); return 0;
}
ssize_t (dev_write) (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
        printk(KERN_ALERT " inside the write function \n"); return length;
}

int (dev_open) (struct inode *pinode, struct file *pfile)
{
        printk(KERN_ALERT " inside the open function \n"); return 0;
}

int (dev_close) (struct inode *pinode, struct file *pfile)
{
        printk(KERN_ALERT " inside the close function \n"); return 0;
}




int char_Dev_init(void)
{
 	printk(KERN_ALERT "Inside init function \n");
	register_chrdev(240,"Simple_dev_Drv",&char_Dev_file_operations);
	
	return 0;
}

void char_Dev_exit(void)
{
	printk(KERN_ALERT "inside exit function \n");
	unregister_chrdev(240,"Simple_dev_Drv");
}

module_init(char_Dev_init);
module_exit(char_Dev_exit);

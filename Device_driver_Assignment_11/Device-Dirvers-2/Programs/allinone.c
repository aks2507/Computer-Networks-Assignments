		//INITIAL PART OF DRIVER
		//----------------------

/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
MODULE_LICENSE("Dual BSD/GPL");

/* Declaration of memory.c functions */
int memory_open(struct inode *inode, struct file *filp);
int memory_release(struct inode *inode, struct file *filp);
ssize_t memory_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
ssize_t memory_write(struct file *filp, char *buf, size_t count, loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

/* Structure that declares the usual file */
/* access functions */
struct file_operations memory_fops = {
  read: memory_read,
  write: memory_write,
  open: memory_open,
  release: memory_release
};

/* Declaration of the init and exit functions */
module_init(memory_init);
module_exit(memory_exit);

/* Global variables of the driver */
int memory_major = 60;/* Major number */
char *memory_buffer;/* Buffer to store data */


		//CONNECTING THE DEVICE WITH ITS FILES
		//------------------------------------

//# mknod /dev/memory c 60 0//create a file 
int memory_init(void) 
{
  int result;
  result = register_chrdev(memory_major, "memory", &memory_fops);
  /* Registering char device driver with a major number in the kernal */
  /* If you have any device with the same major number ,
     this is the the driver that is going to serve the access to device*/
  if (result < 0) 
  {
    printk("<1>memory: cannot obtain major number %d\n", memory_major);
    return result;
  }
  /* Allocating memory for the buffer */
  memory_buffer = kmalloc(1, GFP_KERNEL); 
  if (!memory_buffer) { 
    result = -ENOMEM;
    goto fail; 
  } 
  memset(memory_buffer, 0, 1);
  printk("<1>Inserting memory module\n"); 
  return 0;

  fail: 
    memory_exit(); 
    return result;
}

		//REMOVING THE DRIVER
		//-------------------

void memory_exit(void) {
  unregister_chrdev(memory_major, "memory"); /* Freeing the major number */
  if (memory_buffer) 
    kfree(memory_buffer);/* Freeing buffer memory */
  printk("<1>Removing memory module\n");
}

		//OPENING,CLOSING THE DEVICE AS FILE
		//----------------------------------

int memory_open(struct inode *inode, struct file *filp) 
{
  /* Success */
  return 0;
}
int memory_release(struct inode *inode, struct file *filp) 
{
  /* Success */
  return 0;
}

		//READING,WRITING THE DEVICE
		//--------------------------

ssize_t memory_read(struct file *filp, char *buf,size_t count,loff_t *f_pos) 
{ 
  copy_to_user(buf,memory_buffer,1);/* Transfering data to user space */ 
  
  if (*f_pos == 0) /* Changing reading position as best suits */ 
  { 
    *f_pos+=1; 
    return 1; 
  } 
  else 
  { 
    return 0; 
  }

}
ssize_t memory_write( struct file *filp, char *buf,size_t count,loff_t *f_pos) 
{
  char *tmp;
  tmp=buf+count-1;
  copy_from_user(memory_buffer,tmp,1);
  return 1;
}
/*
		Events			User functions		Kernel functions
		Load module		insmod				module_init()
		Open device		fopen				file_operations: open
		Close device	fread				file_operations: read
		Write device	fwrite				file_operations: write
		Close device	fclose				file_operations: release
		Remove module	rmmod				module_exit()
*/
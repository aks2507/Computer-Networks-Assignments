#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>
 
#define VRAM_BASE 0x000A0000
#define VRAM_SIZE 0x00020000
 
static void __iomem *vram;
static dev_t first;
static struct cdev c_dev;
static struct class *cl;
 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    int i;
    u8 byte;
 
    if (*off >= VRAM_SIZE)
    {
        return 0;
    }
    if (*off + len > VRAM_SIZE)
    {
        len = VRAM_SIZE - *off;
    }
    for (i = 0; i < len; i++)
    {
        byte = ioread8((u8 *)vram + *off + i);
        if (copy_to_user(buf + i, &byte, 1))
        {
            return -EFAULT;
        }
    }
    *off += len;
 
    return len;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    int i;
    u8 byte;
 
    if (*off >= VRAM_SIZE)
    {
        return 0;
    }
    if (*off + len > VRAM_SIZE)
    {
        len = VRAM_SIZE - *off;
    }
    for (i = 0; i < len; i++)
    {
        if (copy_from_user(&byte, buf + i, 1))
        {
            return -EFAULT;
        }
        iowrite8(byte, (u8 *)vram + *off + i);
    }
    *off += len;
 
    return len;
}
 
static struct file_operations vram_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .read = my_read,
    .write = my_write
};
 
static int __init vram_init(void) /* Constructor */
{
    if ((vram = ioremap(VRAM_BASE, VRAM_SIZE)) == NULL)
    {
        printk(KERN_ERR "Mapping video RAM failed\n");
        return -1;
    }
    if (alloc_chrdev_region(&first, 0, 1, "vram") < 0)
    {
        return -1;
    }
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        unregister_chrdev_region(first, 1);
        return -1;
    }
    if (device_create(cl, NULL, first, NULL, "vram") == NULL)
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
 
    cdev_init(&c_dev, &vram_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }
    return 0;
}
 
static void __exit vram_exit(void) /* Destructor */
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    iounmap(vram);
}
 
module_init(vram_init);
module_exit(vram_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email_at_sarika-pugs_dot_com>");
MODULE_DESCRIPTION("Video RAM Driver");
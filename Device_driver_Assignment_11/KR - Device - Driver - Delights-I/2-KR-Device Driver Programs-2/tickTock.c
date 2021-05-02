#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

int opendev(struct inode *i, struct file *f);
 int closedev(struct inode *i, struct file *f);
 ssize_t readme(struct file *f, char *c, size_t t, loff_t *o);
ssize_t writeme(struct file *f, const char *c, size_t t, loff_t *o);
 int Major;            
#define SUCCESS 0
#define DEVICE_NAME "TickTock"           
static struct file_operations fops = {
  .read = readme, 
  .write = writeme,
  .open = opendev,
  .release = closedev
};

int Tick_init(void){
  	Major = register_chrdev(0, DEVICE_NAME, &fops);
 		if(Major>0){
 			printk("TickTock is Ready For Requests!: 		  %d\n",Major);
 			return 0;
 		}else{
 			printk("Something Bad happened OhOhhh: %d",Major); return -1;
 		}
}
module_init(Tick_init);


 void Tock_exit(void)
{
 	unregister_chrdev(Major, DEVICE_NAME);
 	printk("Goodbye Crewl World!\n");
}
module_exit(Tock_exit);




MODULE_LICENSE("GPL");


int opendev(struct inode * a, struct file * b){
printk("Someone opened me!\n");
//nothing to do!!!
return SUCCESS;
}
int closedev(struct inode * a, struct file * b){
//nothing to do!!
printk("Someone Closed me!\n");
return SUCCESS;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}
char *b;
// int pow(int a, int b);
int itoa(int num, char* str)
{
	int base=10;
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
       str[i++] = '0';
       str[i] = '\0';
        int x=strlen(str); strcpy(b,str); return x;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    int y= strlen(str); strcpy(b,str); return y;
}
int atoi(const char * str)
{
    int res = 0,i=0; // Initialize result
  
    // Iterate through all characters of input string and
    // update result
    for (i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
  
    // return result.
    return res;
}

ssize_t readme(struct file *filp,char *buffer,size_t length,loff_t *offset)  {
int len;
   struct timeval time;
   do_gettimeofday(&time);
  // char *b=NULL;
   b=kmalloc(sizeof(char)*40,GFP_KERNEL);
   len=itoa(time.tv_sec,b);
   copy_to_user(buffer,b,len);
   printk("Return val:%s\n",b);
   return length;
}


ssize_t writeme(struct file *filp,const char *buff,size_t len, loff_t *off)
{
  int i=atoi(buff);
  struct timespec ab;
  ab.tv_sec=i;
  ab.tv_nsec=0;
  do_settimeofday(&ab);
  return len;
}




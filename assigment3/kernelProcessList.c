#include "processList.h"
#include <linux/fs.h> 	 
#include <linux/kernel.h>    
#include <linux/errno.h>     
#include <linux/module.h>  
#include <linux/miscdevice.h>


//Pointer to interate through each process
struct task_struct *i;

//buffer to store processes data
struct my_buf mybuf[MAX_BUF_SIZE];

//Number of processes
int count;

static ssize_t sample_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    int error_count =0;

    for_each_process(i)
    {
     
       mybuf[count].pid=i->pid;
       mybuf[count].ppid=i->parent->pid;
       mybuf[count].state = i->state;
       mybuf[count].cpu = task_cpu(i);

       count++;
    }
      //unsigned long __copy_to_user (	void __user * to,const void * from,unsigned long n);
      error_count = copy_to_user((struct my_buf*)buf, mybuf, MAX_BUF_SIZE);

      if (error_count != 0){      
         printk(KERN_INFO "Failed to send %d characters to the user\n", error_count);
         return -EFAULT;              
      }

   return count;
}

static ssize_t sample_write(struct file *file, const char __user *buf,
		       size_t len, loff_t *ppos)
{
    pr_info("Yummy - I just ate %d bytes\n", len);
    return len; /* But we don't actually do anything with the data */
}

static int sample_open(struct inode *inodep, struct file *file) { 
   printk(KERN_INFO "Device has been successfully opened\n");
   return 0;
}

static int sample_close(struct inode *inodep, struct file *file){
     
   printk(KERN_INFO "Device has been successfully closed\n");
   return 0;
}

// Declaration of the file operations
static struct file_operations sample_fops = 
{
    .owner			= THIS_MODULE,
    .write			= sample_write,
    .open			= sample_open,
    .read         = sample_read,
    .release		= sample_close,
    .llseek 		= no_llseek,
};

//device struct
static struct miscdevice sample_device = { 
    .minor = MISC_DYNAMIC_MINOR, 
    .name = "process_list",
    .fops = &sample_fops
};


//register the device
static int __init processList_init(void)
{
    int error;

    error = misc_register(&sample_device);
    if (error) {
        pr_err("can't misc_register :(\n");
        return error;
    }

    pr_info("register device: %d \n",sample_device.minor);
    return 0;
}

//unregister the device
static void __exit processList_exit(void)
{
    misc_deregister(&sample_device);
    pr_info("I'm out\n");
}

module_init(processList_init);
module_exit(processList_exit);
MODULE_DESCRIPTION("module for process management");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xin Hong");
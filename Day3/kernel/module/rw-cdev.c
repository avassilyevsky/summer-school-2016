/* Dummy character device driver from Free Electrons
 * Referenced in http://free-electrons.com/training/drivers
 *
 * Copyright 2006, Free Electrons <http://free-electrons.com>
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/ioport.h>
#include <asm/uaccess.h>

//#define DEBUG
#define BUF_SIZE	1024

static int rwdev_count = 1;
static dev_t	rw_dev=MKDEV(209,1);
static ulong	start_addr=0,block_len=0;
static struct resource *rsrc;
void *base_ptr;
module_param(start_addr, ulong, S_IRUGO);
module_param(block_len, ulong, S_IRUGO);

static struct cdev rwdev_cdev;


static ssize_t
rwdev_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t
rwdev_read(struct file *file, char __user * buf, size_t count, loff_t * ppos)
{
    int i;
    if (count>block_len)
	count=block_len;
    for (i=0;i<count;i++)
	buf[i]=readb(base_ptr+i);
    return count;
}

static ssize_t
rwdev_write(struct file *file, const char __user *buf, size_t count,
	   loff_t *ppos)
{
	return count;
}

static const struct file_operations rwdev_fops = {
	.owner = THIS_MODULE,
	.open = rwdev_open,
	.read = rwdev_read,
	.write = rwdev_write,
};

static int __init rwdev_init(void)
{
	int err;
	if (register_chrdev_region(rw_dev, rwdev_count, "rwdev")) {
	    err = -ENODEV;
	    goto err_exit_and_free;
	}
	cdev_init(&rwdev_cdev, &rwdev_fops);
	rsrc=request_mem_region(start_addr,block_len,"res_mem");
	base_ptr=ioremap(start_addr,block_len);
	printk("rwdev initialized\n");
	printk("start_addr=%x block_len=%x\n",start_addr,block_len);
	return 0;

 err_dev_unregister:
	unregister_chrdev_region(rw_dev, rwdev_count);
 err_exit_and_free:
 err_exit:
	return err;
}

static void __exit rwdev_exit(void)
{
	iounmap(base_ptr);
	release_mem_region(start_addr,block_len);
	cdev_del(&rwdev_cdev);
	printk("cdev removed\n");
	unregister_chrdev_region(rw_dev, rwdev_count);
	printk("rwdev deinitialized\n");
}

module_init(rwdev_init);
module_exit(rwdev_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example character driver");
MODULE_AUTHOR("Free Electrons");

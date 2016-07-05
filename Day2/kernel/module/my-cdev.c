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
#include <asm/uaccess.h>

#define BUF_SIZE	8192

static char *bboard_buf;

static int bboard_count = 1;
static size_t	cur_size=0;
static dev_t bboard_dev = MKDEV(208, 1);

static struct cdev bboard_cdev;


static ssize_t
bboard_open(struct inode *inode, struct file *file)
{
	return 0;
}


static ssize_t
bboard_read(struct file *file, char __user * buf, size_t count, loff_t * ppos)
{
	memcpy(buf,bboard_buf,count);
	return cur_size;
}

static ssize_t
bboard_write(struct file *file, const char __user *buf, size_t count,
	   loff_t *ppos)
{
	
	if (!count)
	    return 0;
	if (count >= BUF_SIZE)
	    return 0;
	memcpy(bboard_buf,buf,count);
	cur_size=count;
	bboard_buf=buf;
	printk("%d bytes received\n",count);
	return count;
}

static const struct file_operations bboard_fops = {
	.owner = THIS_MODULE,
	.open = bboard_open,
	.read = bboard_read,
	.write = bboard_write,
};

static int __init bboard_init(void)
{
	int err;

	bboard_buf=kmalloc(BUF_SIZE, GFP_KERNEL);
	if (!bboard_buf)
	{
		err = -ENOMEM;
		goto err_exit;
	}

	if (register_chrdev_region(bboard_dev, bboard_count, "bboard")) {
		err = -ENODEV;
		goto err_exit_and_free;
	}

	cdev_init(&bboard_cdev, &bboard_fops);

	if (cdev_add(&bboard_cdev, bboard_dev, bboard_count)) {
		err = -ENODEV;
		goto err_dev_unregister;
	}
	printk("bboard initialized\n");
	return 0;

 err_dev_unregister:
	unregister_chrdev_region(bboard_dev, bboard_count);
 err_exit_and_free:
	kfree(bboard_buf);
 err_exit:
	return err;
}

static void __exit bboard_exit(void)
{
	cdev_del(&bboard_cdev);
	unregister_chrdev_region(bboard_dev, bboard_count);
	kfree(bboard_buf);
	printk("bboard deinitialized");
	//iounmap(bboard_buf);
}

module_init(bboard_init);
module_exit(bboard_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example character driver");
MODULE_AUTHOR("Free Electrons");

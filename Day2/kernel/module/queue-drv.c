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
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
//#define DEBUG
typedef struct TQueue
{
    size_t size;
    int r_idx,w_idx;
    int count;
    char *buf;
} TQueue,*PQueue;

static ssize_t q_init(PQueue q, size_t count)
{
    q->buf=kmalloc(count,GFP_KERNEL);
    if (!q->buf)
	return 0;
    q->size=count;
    q->r_idx=0;
    q->w_idx=0;
    q->count=0;
    return count;
#ifdef DEBUG
    printk("Queue initialized\n");
#endif
}

static void q_free(PQueue q)
{
    kfree(q->buf);
#ifdef DEBUG
    printk("Queue freed\n");
#endif
}

static int q_read(PQueue q, char *value)
{
    char v;
    if (q->count==0)
	return 0;
    v=(q->buf)[q->r_idx++];
    put_user(v,value);
    if (q->r_idx>=q->size)
	q->r_idx=0;
    q->count--;
    return 1;
}

static int q_read_block(PQueue q, char *buf, int len)
{
    int buf1_len;
//если запрошено больше,чем есть в очереди, отдаём сколько есть
    if (len>q->count)
	len=q->count;
//если запрошено меньше,чем осталось от указателя чтения до 
//края буфера, копируем в один приём
    if (q->r_idx+len<q->size)
    {
	copy_to_user(buf,q->buf+q->r_idx,len);
	q->r_idx+=len;
	q->count-=len;
	return len;
    }
//копируем в 2 приёма
//1 - от текущего индекса до края
    buf1_len=q->size-q->r_idx;
    copy_to_user(buf,q->buf+q->r_idx,buf1_len);
//2 - от 0 до сколько_останется
    copy_to_user(buf+buf1_len,q->buf,q->count-buf1_len);
    q->r_idx=q->count-buf1_len;
    q->count-=len;
    return len;
}

static ssize_t q_write(PQueue q, char value)
{
    if (q->count==q->size)
	return 0;
    q->buf[q->w_idx++]=value;
    if (q->w_idx>=q->size)
	q->w_idx=0;
    if (q->count<q->size)
        q->count++;
    return 1;
}

#define BUF_SIZE	30


static int drv_count = 1;
static TQueue queue;
static dev_t drv_dev = MKDEV(208, 1);
DECLARE_WAIT_QUEUE_HEAD(ev_rw);
static struct cdev drv_cdev;


static ssize_t
drv_read(struct file *file, char __user * buf, size_t count, loff_t * ppos)
{
	int i;
#ifdef	DEBUG
	printk("[read] -> count:%d, queue.count:%d\n",count,queue.count);
#endif
        if (wait_event_interruptible(ev_rw,queue.count!=0))
	  return -ERESTARTSYS;

	{
	    if (count>queue.count)
	    {
		count=queue.count;
	    }
	    q_read_block(&queue,buf,count);
	}
#ifdef	DEBUG
	printk("[read] -> %d bytes read from queue\n",count);
	printk("[read] -> r_idx:%d w_idx:%d q.count:%d q.size:%d\n",
		queue.r_idx,
		queue.w_idx,
		queue.count,
		queue.size);
#endif
	wake_up_interruptible(&ev_rw);
	return count;
}
static ssize_t
drv_write(struct file *file, const char __user *buf, size_t count,
       loff_t *ppos)
{
	int i;
	char v;
#ifdef	DEBUG
	printk("[write] -> count:%d, queue.count:%d\n",count,queue.count);
#endif
	if (wait_event_interruptible(ev_rw,queue.count<queue.size))
	    return -ERESTARTSYS;

        if (count>queue.size-queue.count)
	{
	    count=queue.size-queue.count;
	}

	for (i=0;i<count;i++)
	{
	    get_user(v,buf+i);
	    q_write(&queue,v);
	}
#ifdef DEBUG
	printk("[write] -> %d bytes written to queue\n",count);
#endif
        wake_up_interruptible(&ev_rw);
	return count;


}

static const struct file_operations drv_fops = {
	.owner = THIS_MODULE,
	.read = drv_read,
	.write = drv_write
};

static int __init drv_init(void)
{
	int err;

	if (!q_init(&queue,BUF_SIZE))
	{
		err = -ENOMEM;
		goto err_exit;
	}

	if (register_chrdev_region(drv_dev, drv_count, "pipe_buf")) {
		err = -ENODEV;
		goto err_exit_and_free;
	}

	cdev_init(&drv_cdev, &drv_fops);

	if (cdev_add(&drv_cdev, drv_dev, drv_count)) {
		err = -ENODEV;
		goto err_dev_unregister;
	}

	printk("driver initialized\n");
	return 0;
	
 err_dev_unregister:
	unregister_chrdev_region(drv_dev, drv_count);
 err_exit_and_free:
	q_free(&queue);
 err_exit:
	return err;
}

static void __exit drv_exit(void)
{
	q_free(&queue);
	cdev_del(&drv_cdev);
	unregister_chrdev_region(drv_dev, drv_count);
	printk("driver deinitialized\n");
}

module_init(drv_init);
module_exit(drv_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Pipe character driver");
MODULE_AUTHOR("");

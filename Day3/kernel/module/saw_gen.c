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
	return -1;
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

    q->buf[q->w_idx++]=value;
    if (q->w_idx>=q->size)
	q->w_idx=0;
    if (q->count<q->size)
        q->count++;
    return 0;
}


#define BUF_SIZE	1024


static int drv_count = 1;
static TQueue queue;
static int term_kthread=0,Terminated=0;
static dev_t drv_dev = MKDEV(208, 1);
DECLARE_WAIT_QUEUE_HEAD(ev_rw);
static struct cdev drv_cdev;

static int thread_func(void *q)
{
	
    PQueue AQueue=&queue;
    int i=0;
    for(;;i++)
    {
	if (term_kthread)
	{
	    Terminated=1;
	    return 1;
	}
	q_write(AQueue,(i % 30)+0x30);
	wake_up_interruptible(&ev_rw);
#ifdef DEBUG
	printk("r_idx=%d,w_idx=%d,count=%d,size=%d\n",
		AQueue->r_idx,
		AQueue->w_idx,
		AQueue->count, 
		AQueue->size);
#endif
	msleep(50);
    }
    return 0;
};

static ssize_t
drv_read(struct file *file, char __user * buf, size_t count, loff_t * ppos)
{
	    int rd_count;
	    if (wait_event_interruptible(ev_rw,queue.count!=0))
		return -ERESTARTSYS;
	    if (count>queue.count)
		count=queue.count;
#ifdef DEBUG
	    printk("Count=%d\n",count);
#endif	
	    if (count==1)
		rd_count=q_read(&queue,buf);
	    else
		rd_count=q_read_block(&queue,buf,count);
	
	
	return rd_count;
}

static const struct file_operations drv_fops = {
	.owner = THIS_MODULE,
	.read = drv_read,
};




static int __init drv_init(void)
{
	int err;

	if (!q_init(&queue,BUF_SIZE))
	{
		err = -ENOMEM;
		goto err_exit;
	}

	if (register_chrdev_region(drv_dev, drv_count, "saw_gen")) {
		err = -ENODEV;
		goto err_exit_and_free;
	}

	cdev_init(&drv_cdev, &drv_fops);

	if (cdev_add(&drv_cdev, drv_dev, drv_count)) {
		err = -ENODEV;
		goto err_dev_unregister;
	}

	kthread_run(thread_func,NULL,"kthread_saw");
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
	term_kthread=1;
	while (!Terminated) msleep(10);
	q_free(&queue);
	cdev_del(&drv_cdev);
	unregister_chrdev_region(drv_dev, drv_count);
	printk("driver deinitialized\n");
}

module_init(drv_init);
module_exit(drv_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Saw waveform generator");
MODULE_AUTHOR("");

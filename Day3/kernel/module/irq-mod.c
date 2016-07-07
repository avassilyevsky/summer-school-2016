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
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

#define GPIO_NUM	67
#define GPIO_NAME	"Interrupe Line"
#define IRQ_NAME	"GPIO_IRQ"

static int gpio_irq_num = 0;

irqreturn_t irq_handler(int irq, void *notused)
{
    printk("IRQ Handled\n");
    return IRQ_HANDLED;
}


static int __init gpio_irq_test_init(void)
{
    if(gpio_request(GPIO_NUM,GPIO_NAME)) 
    {
	return 0;
    }
    if((gpio_irq_num=gpio_to_irq(GPIO_NUM))<0)
    {
	return 0;
    }
    if(request_irq(gpio_irq_num, irq_handler, 0, IRQ_NAME,NULL))
    {
	return 0;
    }
    irq_set_irq_type(gpio_irq_num, IRQ_TYPE_EDGE_RISING);
    return 0;
}

static void __exit gpio_irq_test_exit(void)
{
    free_irq(gpio_irq_num,NULL);
    gpio_free(GPIO_NUM);
}
module_init(gpio_irq_test_init);
module_exit(gpio_irq_test_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example character driver");
MODULE_AUTHOR("Free Electrons");

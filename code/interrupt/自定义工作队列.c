#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

int irq; 
struct work_struct test_workqueue_work;
struct workqueue_struct *test_workqueue;

void test_work(struct work_struct* work)
{
    msleep(1000);
    printk("This is test_work\n");
}

irqreturn_t test_interrupt(int irq, void* args)
{
    printk("This is test_interrupt\n");
    queue_work(test_workqueue,&test_workqueue_work);
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int interrupt_irq_init(void)
{
    int ret;
    
    irq = gpio_to_irq(10);
    if (irq < 0) {
        printk("gpio_to_irq failed: %d\n", irq);
        return irq;
    }
    printk("irq is %d\n",irq);
    
    ret = request_irq(irq,test_interrupt,IRQF_TRIGGER_RISING,"test",NULL);
    if(ret < 0)
    {
        printk("request_irq failed\n");
        return -1;
    }
    test_workqueue=create_workqueue("test_workqueue");
    INIT_WORK(&test_workqueue_work,test_work);

    return 0;
}

static void interrupt_irq_exit(void)
{
    free_irq(irq,NULL);
    destroy_workqueue(test_workqueue);
    printk("bye\n");;
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zzz");
MODULE_VERSION("V1.0");
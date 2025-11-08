#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>


int irq;

irqreturn_t test_work(int irq,void* args)
{
    msleep(1000);
    printk("This is test_work\n");
    return IRQ_RETVAL(IRQ_HANDLED);
}

irqreturn_t test_interrupt(int irq, void* args)
{
    printk("This is test_interrupt\n");
    return IRQ_WAKE_THREAD;
}

static int interrupt_irq_init(void)
{
    int ret;

   
    
    irq = gpio_to_irq(10);//目前gpio的编号没有算出
    printk("irq is %d\n",irq);
    
    ret = request_threaded_irq(irq,test_interrupt,test_work,IRQF_TRIGGER_RISING,"test",NULL);
    if(ret < 0)
    {
        printk("request_irq failed\n");
        return -1;
    }
    
    return 0;
}

static void interrupt_irq_exit(void)
{
    free_irq(irq,NULL);
    printk("bye\n");;
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zzz");
MODULE_VERSION("V1.0");
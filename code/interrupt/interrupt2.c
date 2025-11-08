#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

int irq;

void testsoft_func(struct softirq_action *softirq_action)
{
    printk("This is testsoft_func\n");
}
irqreturn_t test_interrupt(int irq, void* args)
{
    printk("This is test_interrupt\n");
    raise_softing(TEST_SOFTIRQ);
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int interrupt_irq_init(void)
{
    int ret;

    open_softirq(TEST_SOFTIRQ,testsoft_func);
    
    irq = gpio_to_irq(10);//目前gpio的编号没有算出
    printk("irq is %d\n",irq);
    
    ret = request_irq(irq,test_interrupt,IRQF_TRIGGER_RISING,"test",NULL);
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
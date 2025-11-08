#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

int irq;
struct tasklet_struct mytasklet;

void mytasklet_func(unsigned long data)
{
    printk("data is %ld\n",data);
}
irqreturn_t test_interrupt(int irq, void* args)
{
    printk("This is test_interrupt\n");
    tasklet_schedule(&mytasklet);
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int interrupt_irq_init(void)
{
    int ret;

    tasklet_init(&mytasklet,mytasklet_func,1);
    
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
    tasklet_kill(&mytasklet);
    printk("bye\n");;
}

module_init(interrupt_irq_init);
module_exit(interrupt_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zzz");
MODULE_VERSION("V1.0");
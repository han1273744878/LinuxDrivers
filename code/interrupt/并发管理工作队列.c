#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

int irq; 

struct work_data{
    struct work_struct test_work;
    int a;
    int b;
};
struct work_data test_workqueue_work;
struct workqueue_struct *test_workqueue;


void test_work(struct work_struct* work)
{
    struct work_data *pdata;
    pdata = container_of(work,struct work_data,test_work);
    printk("This is test_work\n");
    printk("a is %d,b is %d\n",pdata->a,pdata->b);
}

irqreturn_t test_interrupt(int irq, void* args)
{
    printk("This is test_interrupt\n");
    test_workqueue_work.a = 1;
    test_workqueue_work.b = 2;
    queue_work(test_workqueue,&test_workqueue_work.test_work);
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
    test_workqueue=alloc_workqueue("test_workqueue",WQ_UNBOUND,0);//和工作队列只有这里不一样
    INIT_WORK(&test_workqueue_work.test_work,test_work);

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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/delay.h>

#define IMX_GPIO_NR(bank, nr)  (((bank) - 1) * 32 + (nr))
#define BUTTON1_GPIO  IMX_GPIO_NR(1, 4)
#define BUTTON2_GPIO  IMX_GPIO_NR(1, 10)

int irq1, irq2;

// ========== 两个不同的工作项 ==========
struct work_struct work1;
struct work_struct work2;

// 工作1：处理按钮1
void work1_func(struct work_struct *work)
{
    int i;
    printk("Work1 START (Button1 pressed)\n");
    
    // 模拟耗时操作
    for (i = 0; i < 3; i++) {
        msleep(500);
        printk("  Work1 processing... %d/3\n", i+1);
    }
    
    printk("Work1 DONE\n");
}

// 工作2：处理按钮2
void work2_func(struct work_struct *work)
{
    int i;
    printk("Work2 START (Button2 pressed)\n");
    
    // 模拟耗时操作
    for (i = 0; i < 3; i++) {
        msleep(500);
        printk("  Work2 processing... %d/3\n", i+1);
    }
    
    printk("Work2 DONE\n");
}

// 中断1：按钮1
irqreturn_t button1_interrupt(int irq, void *dev_id)
{
    printk("IRQ1: Button1 pressed, scheduling work1\n");
    schedule_work(&work1);  // ← 共享系统工作队列
    return IRQ_HANDLED;
}

// 中断2：按钮2
irqreturn_t button2_interrupt(int irq, void *dev_id)
{
    printk("IRQ2: Button2 pressed, scheduling work2\n");
    schedule_work(&work2);  // ← 共享系统工作队列
    return IRQ_HANDLED;
}

static int __init interrupt_init(void)
{
    int ret;
    
    printk("=== Shared Work Queue Demo ===\n");
    
    // 初始化两个工作项
    INIT_WORK(&work1, work1_func);
    INIT_WORK(&work2, work2_func);
    
    // 注册按钮1
    gpio_request(BUTTON1_GPIO, "button1");
    gpio_direction_input(BUTTON1_GPIO);
    irq1 = gpio_to_irq(BUTTON1_GPIO);
    ret = request_irq(irq1, button1_interrupt, 
                      IRQF_TRIGGER_FALLING, "button1", NULL);
    if (ret) {
        printk("Failed to request IRQ1\n");
        return ret;
    }
    
    // 注册按钮2
    gpio_request(BUTTON2_GPIO, "button2");
    gpio_direction_input(BUTTON2_GPIO);
    irq2 = gpio_to_irq(BUTTON2_GPIO);
    ret = request_irq(irq2, button2_interrupt, 
                      IRQF_TRIGGER_FALLING, "button2", NULL);
    if (ret) {
        printk("Failed to request IRQ2\n");
        free_irq(irq1, NULL);
        gpio_free(BUTTON1_GPIO);
        return ret;
    }
    
    printk("Both buttons registered\n");
    printk("Work1 and Work2 will share the system work queue\n");
    return 0;
}

static void __exit interrupt_exit(void)
{
    // 取消所有待处理的工作
    cancel_work_sync(&work1);
    cancel_work_sync(&work2);
    
    free_irq(irq1, NULL);
    free_irq(irq2, NULL);
    gpio_free(BUTTON1_GPIO);
    gpio_free(BUTTON2_GPIO);
    
    printk("Module unloaded\n");
}

module_init(interrupt_init);
module_exit(interrupt_exit);
MODULE_LICENSE("GPL");
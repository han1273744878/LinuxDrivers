#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

static struct resource gpio1_resources[] = {
    // 内存资源（从 /proc/iomem 查到）
    [0] = {
        .start = 0x0209C000,  // ← GPIO1 基地址
        .end   = 0x0209FFFF,  // ← 0x0209C000 + 0x4000 - 1
        .flags = IORESOURCE_MEM,
    },
    // 中断资源（从 /proc/interrupts 推算）
    [1] = {
        .start = 66,  // ← GPIO1 的基础中断号
        .end   = 97,  // ← GPIO1 的中断范围
        .flags = IORESOURCE_IRQ,
    },
};

void gpio1_release(struct device *dev )
{
    printk("my_device_release\n");
}
struct platform_device platform_device_test = {
    .name = "test",
    .id = -1,
    .num_resources = ARRAY_SIZE(gpio1_resources),
    .resource =gpio1_resources,
    .dev={
        .release = gpio1_release,
    },
};

static int platform_device_init(void)
{
    int ret;
    
    ret = platform_device_register(&platform_device_test);
    if (ret) {
        printk("platform_device_register failed: %d\n", ret);
        return ret;
    }
    
    printk("platform_device_init\n");
    return 0;
}

static void platform_device_exit(void)
{
    platform_device_unregister(&platform_device_test);
    printk("platform_device_exit\n");
}

module_init(platform_device_init);
module_exit(platform_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yyy");
MODULE_DESCRIPTION("platform device test");
MODULE_VERSION("V1.0");
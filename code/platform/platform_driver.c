#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

struct resource *myresources;
int mydriver_probe(struct platform_device * dev){
    printk("mydriver_probe\n");

    printk("IRQ is %lld\n",dev->resource[1].start);

    myresources = platform_get_resource(dev,IORESOURCE_IRQ,0);
    printk("IRQ is %lld\n",myresources->start);
    
    myresources = platform_get_resource(dev,IORESOURCE_MEM,0);
    printk("Memory is %lld\n",myresources->start);
    return 0;
}

int mydriver_remove(struct platform_device * dev){
    printk("mydriver_remove\n");
    return 0;
}

const struct platform_device_id mydriver_id_table[] = {
    [0] = {
        .name = "test",
    },
};
struct platform_driver platform_driver_test = {
    .probe = mydriver_probe,
    .remove = mydriver_remove,
    .id_table = mydriver_id_table,
    .driver = {
        .name = "test",
        .owner = THIS_MODULE,
    },
};

static int platform_driver_init(void)
{
    int ret;
    
    ret = platform_driver_register(&platform_driver_test);
    if (ret) {
        printk("platform_driver_register failed: %d\n", ret);
        return ret;
    }
    
    printk("platform_driver_init\n");
    return 0;
}

static void platform_driver_exit(void)
{
    platform_driver_unregister(&platform_driver_test);
    printk("platform_driver_exit\n");
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("yyy");
MODULE_DESCRIPTION("platform driver test");
MODULE_VERSION("V1.0");
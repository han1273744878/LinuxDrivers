#include <linux/module.h>
#include <linux/init.h>

static int hello_init(void)
{
    printk("Hello, World!\n");
    return 0;
}
static void hello_exit(void)
{
    printk("Goodbye, World!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kkk");
MODULE_VERSION("V1.0");
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

static int a = 0;
static int array[5]={0};
module_param(a,int,S_IRUGO);
MODULE_PARM_DESC(a,"e.g: a=1");

module_param_array(array,int,&array.size,S_IRUGO);
MODULE_PARM_DESC(array,"e.g: array=1,2,3");

static int module_init(void)
{
    int i = 0;
    printk("a is %d\n",a);

    for(i=0;i<array_size;i++)
    {
        printk("array[%d] is %d\n",i,array[i]);
    }
    printk("array_size is %d\n",array_size);
    return 0;
}
static int module_exit(void)
{
    printk("bye\n");
    return 0;
}


module_init(module_init);
module_exit(module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zzz");
MODULE_VERSION("V1.0");

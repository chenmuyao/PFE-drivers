#include <linux/module.h>

//#error Are we building this file?

// insmod first_drv.ko (kernel object)
static int __init first_drv_init(void)
{
	printk(KERN_INFO "----> My first driver init()\n");
	return 0;
}

// rmmod first_drv
static void __exit first_drv_exit(void)
{
	printk(KERN_INFO "<---- My first driver exit().\n");
}

module_init(first_drv_init);
module_exit(first_drv_exit);

MODULE_AUTHOR("Muyao");
MODULE_DESCRIPTION("My first test driver");
MODULE_LICENSE("GPL");

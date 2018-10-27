#include <linux/module.h>

//#error Are we building this file?

static struct class 				*leddrv_class;
static struct class_device *leddrv_class_dev;

static int led_drv_open(struct inode *inode, struct file *file)
{
	//TODO
}

static ssize_t led_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	//TODO
}

static struct file_operations led_drv_fops = {
	.owner 	= THIS_MODULE,
	.open 	= led_drv_open,
	.write	= led_drv_write,
};

int major; 	// Device class
static int __init led_drv_init(void)
{
	printk(KERN_INFO "----> Led driver init()\n");
	major = register_chrdev(0,"led_drv", &led_drv_fops);
	leddrv_class = class_create(THIS_MODULE, "leddrv"); // ??
	leddrv_class_dev = class_device_create(leddrv_class, NULL , MKDEV(major,0), NULL, "xyz");
	// TODO
	return 0;
}

static void __exit led_drv_exit(void)
{
	unregister_chrdev(major,"led_drv");
	class_device_unregister(leddrv_class_dev);
	class_destroy(leddrv_class);
	//TODO
	printk(KERN_INFO "<---- Led driver exit().\n");
}




module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_AUTHOR("Muyao");
MODULE_DESCRIPTION("My led driver");
MODULE_LICENSE("GPL");
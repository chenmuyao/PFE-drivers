#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/io.h>
//#error Are we building this file?

volatile unsigned long *gpio1con = NULL;
volatile unsigned long *gpio1dat = NULL;

//#define LED1	49
#define LED1 17

static struct class 				*leddrv_class;
static struct device *leddrv_class_dev;

static int led_drv_open(struct inode *inode, struct file *file)
{
	//gpio_direction_output(LED1, 0);
	*gpio1con &= ~(1<<LED1);
	return 0;
}

static ssize_t led_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;
	copy_from_user(&val, buf, count);
	//gpio_set_value(LED1, val);
	if (val == 1)
	{
		*gpio1dat &= ~(1<<LED1);
	}
	else
	{
		*gpio1dat |= (1<<LED1);
	}
	return 0;
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
	leddrv_class_dev = device_create(leddrv_class, NULL , MKDEV(major,0), NULL, "xyz");
	//gpio_request(LED1, "sysfs");
	//gpio_export(LED1, false);
	gpio1con = (volatile unsigned long *)ioremap(0x4804C134,32); // GPIO 0x4804C000 + offset 134h : GPIO_OE(Output Enable)
	gpio1dat = (volatile unsigned long *)ioremap(0x4804C13C,32); // GPIO 0x4804C000 + offset 13Ch : GPIO_DATAOUT
	return 0;
}

static void __exit led_drv_exit(void)
{
	unregister_chrdev(major,"led_drv");
	device_unregister(leddrv_class_dev);
	class_destroy(leddrv_class);

	//gpio_free(LED1);
	iounmap(gpio1con);
	iounmap(gpio1dat);
	printk(KERN_INFO "<---- Led driver exit().\n");
}




module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_AUTHOR("Muyao");
MODULE_DESCRIPTION("My led driver");
MODULE_LICENSE("GPL");
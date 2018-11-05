#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/init.h>
//#error Are we building this file?

#define LED1	49 

static struct class 				*leddrv_class;
static struct device *leddrv_class_dev;

static int led_drv_open(struct inode *inode, struct file *file)
{
	gpio_direction_output(LED1, 0);
	return 0;
}

static ssize_t led_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;
	copy_from_user(&val, buf, count);
	gpio_set_value(LED1, val);
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
/*
register_chrdev — Register a major number for character devices.
major
major device number or 0 for dynamic allocation
name
name of this range of devices
fops
file operations associated with this devices
*/

	major = register_chrdev(0,"led_drv", &led_drv_fops);
/*
class_create — create a struct class structure
owner
pointer to the module that is to “own” this struct class
name
pointer to a string for the name of this class.
*/
	leddrv_class = class_create(THIS_MODULE, "leddrv"); 
/*
device_create — creates a device and registers it with sysfs

class
pointer to the struct class that this device should be registered to
parent
pointer to the parent struct device of this new device, if any
devt
the dev_t for the char device to be added
fmt
string for the device's name
...
variable arguments
*/	
	leddrv_class_dev = device_create(leddrv_class, NULL , MKDEV(major,0), NULL, "xyz");
	gpio_request(LED1, "sysfs");
	gpio_export(LED1, false);
	return 0;
}

static void __exit led_drv_exit(void)
{
	unregister_chrdev(major,"led_drv");
	device_unregister(leddrv_class_dev);
	class_destroy(leddrv_class);

	gpio_free(LED1);
	printk(KERN_INFO "<---- Led driver exit().\n");
}




module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_AUTHOR("Muyao");
MODULE_DESCRIPTION("My led driver");
MODULE_LICENSE("GPL");

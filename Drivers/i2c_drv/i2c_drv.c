#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <asm/uaccess.h>


static int major;
static struct class *class;
static struct i2c_client *at24cxx_client;

/* Entree: buf[0] : addr
 * Sortie: buf[0] : data
 */
static ssize_t at24cxx_read(struct file * file, char __user *buf, size_t count, loff_t *off)
{
	unsigned char addr, data = 0xBB;
	
	copy_from_user(&addr, buf, 1);
	data = i2c_smbus_read_byte_data(at24cxx_client, addr);
	copy_to_user(buf, &data, 1);
	return 1;
}

/* buf[0] : addr
 * buf[1] : data
 */
static ssize_t at24cxx_write(struct file *file, const char __user *buf, size_t count, loff_t *off)
{
	unsigned char ker_buf[2];
	unsigned char addr;
	unsigned char data;
	//unsigned char data[1];
	unsigned int err;
/*
		struct i2c_msg msg[1] = {
			{
				.addr = 0,
				.flags = at24cxx_client->flags,
				.len = 1,
				.buf = data,
			},
		};
*/
	copy_from_user(ker_buf, buf, 2);
	addr = ker_buf[0];
	data = ker_buf[1];
	//data[0] = ker_buf[1];

	//msg[0].addr = addr;

	//printk("addr = 0x%02x, data = 0x%02x\n", addr, data[0]);
	printk("addr = 0x%02x, data = 0x%02x\n", addr, data);
	/*
	  * Warning: Can't use SMBus Quick Write command, will skip some addresses
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                                                 
10:                                                 
20:                                                 
30: -- -- -- -- -- -- -- --                         
40:                                                 
50: UU -- -- -- UU UU UU UU -- -- -- -- -- -- -- -- 
60:                                                 
70:                                                 
	  */
	err =	i2c_smbus_write_byte_data(at24cxx_client, addr, data); // System Management Bus
	printk("write error %d\n", err);

	//err = i2c_transfer(at24cxx_client->adapter, msg, 1);
	//printk("write error %d\n", err);
	if (!err) 
	{	
		printk("write error %d\n", err);
		return 2;
	}
	else
		return -EIO;	
}

static struct file_operations at24cxx_fops = {
	.owner = THIS_MODULE,
	.read  = at24cxx_read,
	.write = at24cxx_write,
};

static int at24cxx_probe(struct i2c_client *client,
				  const struct i2c_device_id *id)
{
	at24cxx_client = client;
		
	//printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	major = register_chrdev(0, "at24cxx", &at24cxx_fops);
	class = class_create(THIS_MODULE, "at24cxx");
	device_create(class, NULL, MKDEV(major, 0), NULL, "at24cxx"); /* /dev/at24cxx */
	
	return 0;
}

static int  at24cxx_remove(struct i2c_client *client)
{
	//printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	device_destroy(class, MKDEV(major, 0));
	class_destroy(class);
	unregister_chrdev(major, "at24cxx");
		
	return 0;
}

static const struct i2c_device_id at24cxx_id_table[] = {
	{ "at24c02", 0 },
	{}
};

/*
  * When kernel detects a I2C device on I2C bus, kernel gets the device name based on the device tree. 
  * After this kernel scans through list of registered drivers if any of the drivers handle this device. 
  * If yes, then kernel will call probe of that particular driver.
  */

/* 1. 分配/设置i2c_driver */
static struct i2c_driver at24cxx_driver = {
	.driver	= {
		.name	= "my_i2c_drv",
		.owner	= THIS_MODULE,
	},
	.probe		= at24cxx_probe,
	.remove		= at24cxx_remove,
	.id_table	= at24cxx_id_table,
};

static int at24cxx_drv_init(void)
{
	/* 2. 注册i2c_driver */
	i2c_add_driver(&at24cxx_driver);
	
	return 0;
}

static void at24cxx_drv_exit(void)
{
	i2c_del_driver(&at24cxx_driver);
}


module_init(at24cxx_drv_init);
module_exit(at24cxx_drv_exit);
MODULE_LICENSE("GPL");

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>

#include <linux/slab.h>
#include <linux/device.h>
#include <sound/core.h>
#include <linux/spi/spi.h>
#include <asm/uaccess.h>

#include <mach/hardware.h>
#include <mach/regs-gpio.h>

#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
//#error Are we building this file?
// Reference : /drivers/spi/spidev.c
// https://elixir.bootlin.com/linux/v3.8/source/drivers/misc/eeprom/at25.c

/*
  * Basic fuctions : write and read
  * write : byte write & page write
  * byte write : Instruction+address+data
  * read : (write)Instruction+(write)address+(read)data
  */

#define	EEPROM_WREN	0x06		/* latch the write enable */
#define	EEPROM_WRDI	0x04		/* reset the write enable */
#define	EEPROM_RDSR		0x05		/* read status register */
#define	EEPROM_WRSR	0x01		/* write status register */
#define	EEPROM_READ	0x03		/* read byte(s) */
#define	EEPROM_WRITE	0x02		/* write byte(s)/sector */

static struct class 				*spidrv_class;
static struct device *spidrv_class_dev;
static struct spi_device *spi_eeprom_dev;
int major; 	// Device class
static unsigned char *ker_buf;

static int eepromWRITE(unsigned int addr, unsigned char *buf, int length)
{

	return 0;
}

static ssize_t eeprom_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	/* chipselect only toggles at start or end of operation */
	if (count > 4096)
		return -EMSGSIZE;
	copy_from_user(ker_buf,buf,count);
	char
	eepromWRITE()


	return status;
}

static ssize_t eeprom_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{

}

static long spidev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{

}

static struct file_operations eeprom_ops = {
	.owner 	= THIS_MODULE,
	.ioctl		= eeprom_ioctl,
	.write	= eeprom_write,
	.read 		= eeprom_read,
};

static int __devinit spi_eeprom_probe(struct spi_device *spi)
{
    spi_oled_dev = spi;
    
    //s3c2410_gpio_cfgpin(spi_oled_dc_pin, S3C2410_GPIO_OUTPUT);
    //s3c2410_gpio_cfgpin(spi->chip_select, S3C2410_GPIO_OUTPUT);

    ker_buf = kmalloc(4096, GFP_KERNEL);
    
    major = register_chrdev(0, "eeprom", &eeprom_ops);

	spidrv_class = class_create(THIS_MODULE, "eeprom");

	device_create(spidrv_class, NULL, MKDEV(major, 0), NULL, "eeprom"); /* /dev/eeprom */
    
    return 0;
}

static int __devexit spi_eeprom_remove(struct spi_device *spi)
{

	device_destroy(spidrv_class, MKDEV(major, 0));
	class_destroy(spidrv_class);
	unregister_chrdev(major, "eeprom");

    kfree(ker_buf);
    
	return 0;
}


static struct spi_driver spi_eeprom_drv = {
	.driver = {
		.name	= "eeprom",
		.owner	= THIS_MODULE,
	},
	.probe		= spi_eeprom_probe,
	.remove	= __devexit_p(spi_eeprom_remove),
};
/*
int spi_register_driver(struct spi_driver *sdrv)
{
	sdrv->driver.bus = &spi_bus_type;
	if (sdrv->probe)
		sdrv->driver.probe = spi_drv_probe;
	if (sdrv->remove)
		sdrv->driver.remove = spi_drv_remove;
	if (sdrv->shutdown)
		sdrv->driver.shutdown = spi_drv_shutdown;
	return driver_register(&sdrv->driver);
}
*/

static int __init spi_eeprom_init(void)
{
	printk(KERN_INFO "----> Spi driver init()\n");
	return spi_register_driver(&spi_eeprom_drv);
}

static void __exit spi_eeprom_exit(void)
{
	spi_unregister_driver(&spi_eeprom_drv);
	printk(KERN_INFO "<---- Spi driver exit().\n");
}




module_init(spi_eeprom_init);
module_exit(spi_eeprom_exit);

MODULE_AUTHOR("Muyao");
MODULE_DESCRIPTION("My eeprom 25LC020 spi driver");
MODULE_LICENSE("GPL");

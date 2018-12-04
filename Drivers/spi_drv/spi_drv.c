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

#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/spi/eeprom.h>
//#include <linux/of.h>


// Reference : /drivers/spi/spidev.c
// https://elixir.bootlin.com/linux/v3.8/source/drivers/misc/eeprom/at25.c

/*
  * Basic fuctions : write and read
  * write : byte write & page write
  * byte write : Instruction+address+data
  * read : (write)Instruction+(write)address+(read)data
  */

struct eeprom_data {
	struct spi_device	*spi;
	//struct memory_accessor	mem;
	struct mutex		lock;
	struct bin_attribute	bin;
	unsigned		addrlen;
};

#define	EEPROM_WREN	0x06		/* latch the write enable */
#define	EEPROM_WRDI	0x04		/* reset the write enable */
#define	EEPROM_RDSR		0x05		/* read status register */
#define	EEPROM_WRSR	0x01		/* write status register */
#define	EEPROM_READ	0x03		/* read byte(s) */
#define	EEPROM_WRITE	0x02		/* write byte(s)/sector */
#define	EEPROM_SR_nRDY	0x01		/* nRDY = write-in-progress */

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

/* static struct file_operations eeprom_ops = {
	.owner 	= THIS_MODULE,
	.ioctl		= eeprom_ioctl,
	.write	= eeprom_write,
	.read 		= eeprom_read,
}; */

static int __devinit spi_eeprom_probe(struct spi_device *spi)
{
	struct eeprom_data *eeprom = NULL;
	int sr;
	int err;

//?????????????????????????????????????????????????????????????????
	//major = register_chrdev(0, "eeprom", &eeprom_ops);
	//spidrv_class = class_create(THIS_MODULE, "eeprom");
	//device_create(spidrv_class, NULL, MKDEV(major, 0), NULL, "eeprom"); /* /dev/eeprom */
//?????????????????????????????????????????????????????????????????

	/* Test reading status */
	sr = spi_w8r8(spi, EEPROM_RDSR);
	if (sr < 0 || sr & EEPROM_SR_nRDY) {
		dev_dbg(&spi->dev, "rdsr --> %d (%02x)\n", sr, sr);
		err = -ENXIO;
		goto fail;

	/* allocate the memory for eeprom_data struct*/
	if (!(eeprom = kzalloc(sizeof *eeprom, GFP_KERNEL))) {	// kzalloc - allocate memory. The memory is set to zero.
		err = -ENOMEM;
		goto fail;
	}	

	mutex_init(&at25->lock);
	eeprom->spi = spi_dev_get(spi);
	dev_set_drvdata(&spi->dev, eeprom);
	eeprom->addrlen = 1; // 8bits

	// Use bin to store the data
	sysfs_bin_attr_init(&eeprom->bin);
	eeprom->bin.attr.name = "eeprom";
	eeprom->bin.attr.mode = S_IRUSR;
	eeprom->bin.read = eeprom_bin_read; //TODO
	//eeprom->mem.read = at25_mem_read;

	eeprom->bin.size = 2048;
	eeprom->bin.write = eeprom_bin_write; //TODO
	eeprom->bin.attr.mode |= S_IWUSR;
	//eeprom->mem.write = at25_mem_write;

	err = sysfs_create_bin_file(&spi->dev.kobj, &eeprom->bin);
	if (err)
		goto fail;

	// if (chip.setup)
	// 	chip.setup(&at25->mem, chip.context);

	dev_info(&spi->dev, "%Zd %s",
		(eeprom->bin.size < 1024)
			? eeprom->bin.size
			: (eeprom->bin.size / 1024),
		(eeprom->bin.size < 1024) ? "Byte" : "KByte");

    return 0;

    fail:
	dev_dbg(&spi->dev, "probe err %d\n", err);
	kfree(eeprom);
	return err;
}

static int __devexit spi_eeprom_remove(struct spi_device *spi)
{
	struct eeprom_data	*eeprom;

	eeprom = dev_get_drvdata(&spi->dev);
	sysfs_remove_bin_file(&spi->dev.kobj, &eeprom->bin);
	kfree(eeprom);

	device_destroy(spidrv_class, MKDEV(major, 0));
	class_destroy(spidrv_class);
	unregister_chrdev(major, "eeprom");
    
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

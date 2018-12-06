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

#define KER_BUF_TX_SIZE  17
#define KER_BUF_RX_SIZE  4096

#define	EEPROM_WREN	0x06		/* latch the write enable */
#define	EEPROM_WRDI	0x04		/* reset the write enable */
#define	EEPROM_RDSR		0x05		/* read status register */
#define	EEPROM_WRSR	0x01		/* write status register */
#define	EEPROM_READ	0x03		/* read byte(s) */
#define	EEPROM_WRITE	0x02		/* write byte(s)/sector */
#define	EEPROM_SR_nRDY	0x01		/* nRDY = write-in-progress */

static struct class 				*spidrv_class;
static struct spi_device	*eeprom_dev;
int major; 	// Device class
static unsigned char *ker_buf_tx;
static unsigned char *ker_buf_rx;

static struct spi_board_info const spi_info_25lc020[] = {
	 {
        .modalias = "eeprom",     /*Device name corresponding to spi_driver */ 
        .max_speed_hz = 10000000,  /* 10 MHz max. Clock Frequency*/
        .bus_num = 0,     /* eeprom will be connected to bus 1 */
        .mode    = SPI_MODE_0,
        .chip_select   = 1,    /*Depends to spi_master TODO to be verified*/
        .platform_data = NULL , /* used in spi_driver (optional) */
     }            	 
};


//TODO : read_status

static ssize_t eeprom_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	/*
	ker_buf_tx[0] - Addr (transform in the upper layer)
	ker_buf_tx[...] - données
	*/ 
	unsigned char tx_buf[1];
	int ret;   

	struct spi_transfer	t[] = {
            {
    			.tx_buf		= tx_buf,
    			.len		= 1,
        	},
			{
    			.tx_buf		= ker_buf_tx,
    			.len		= count,
			},
		};
	struct spi_message	m;

	if (count > KER_BUF_TX_SIZE)
		return -EMSGSIZE;

	ret = copy_from_user(ker_buf_tx,buf,count);
	/*
	Up to 16 bytes of data can be sent to
	the device before a write cycle is necessary. The only
	restriction is that all of the bytes must reside in the
	same page. Additionally, a page address begins with
	XXXX 0000 and ends with XXXX 1111. If the internal
	address counter reaches XXXX 1111 and clock signals
	continue to be applied to the chip, the address counter
	will roll back to the first address of the page and over-
	write any data that previously existed in those
	locations.
	*/

    tx_buf[0] = EEPROM_READ;

	spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
	spi_message_add_tail(&t[1], &m);
	spi_sync(eeprom_dev, &m);    

	return 0;
}


static ssize_t eeprom_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	/*
	ker_buf_rx[0] - Addr (transform in the upper layer)
	ker_buf_rx[1] - length to read
	*/ 
	unsigned char tx_buf[1];
	unsigned char addr_buf[1];
	int ret;   
	struct spi_transfer	t[] = {
            {
    			.tx_buf		= tx_buf,
    			.len		= 1,
        	},
        	{
        		.tx_buf 		= addr_buf,
        		.len 	= 1,
        	},
			{
    			.rx_buf		= ker_buf_rx,
    			.len		= count,
			},
		};
	struct spi_message	m;

	if (count > 4096)
		return -EMSGSIZE;

	// addr
	ret = copy_from_user(addr_buf,buf,count);

    tx_buf[0] = EEPROM_READ;

	spi_message_init(&m);
	spi_message_add_tail(&t[0], &m);
	spi_message_add_tail(&t[1], &m);
	spi_message_add_tail(&t[2], &m);
	spi_sync(eeprom_dev, &m);    

	ret = copy_to_user(buf, ker_buf_rx,count);

	return 0;

}

static struct file_operations eeprom_ops = {
	.owner 	= THIS_MODULE,
//	.ioctl		= eeprom_ioctl,
	.write	= eeprom_write,
	.read 		= eeprom_read,
}; 

static int spi_eeprom_probe(struct spi_device *spi)
{
	eeprom_dev = spi;

    ker_buf_tx = kmalloc(KER_BUF_TX_SIZE, GFP_KERNEL);
    ker_buf_rx = kmalloc(KER_BUF_RX_SIZE, GFP_KERNEL);
    printk(KERN_INFO "---Spi driver probe---\n");
    /* file_operations */
    major = register_chrdev(0, "eeprom", &eeprom_ops);
	spidrv_class = class_create(THIS_MODULE, "eeprom");
	device_create(spidrv_class, NULL, MKDEV(major, 0), NULL, "eeprom"); /* /dev/eeprom */
    return 0;
}

static int spi_eeprom_remove(struct spi_device *spi)
{
	device_destroy(spidrv_class, MKDEV(major, 0));
	class_destroy(spidrv_class);
	unregister_chrdev(major, "eeprom");
    printk(KERN_INFO "---Spi driver remove---\n");
    kfree(ker_buf_tx);
	return 0;
}


static struct spi_driver spi_eeprom_drv = {
	.driver = {
		.name	= "eeprom",
		.owner	= THIS_MODULE,
	},
	.probe		= spi_eeprom_probe,
	.remove	= spi_eeprom_remove,
};


static int __init spi_eeprom_init(void)
{
	printk(KERN_INFO "----> Spi driver init()\n");
	//spi_register_board_info(spi_info_25lc020, ARRAY_SIZE(spi_info_25lc020));
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

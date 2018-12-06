#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/gpio.h>

// Need to be compiled into the kernel

static struct spi_board_info const spi_info_25lc020[] = {
	 {
        .modalias = "eeprom",     /*Device name corresponding to spi_driver */ 
        .max_speed_hz = 10000000,  /* 10 MHz max. Clock Frequency*/
        .bus_num = 1,     /* eeprom will be connected to bus 1 */
        .mode    = SPI_MODE_0,
        .chip_select   = 1,    /*Depends to spi_master TODO to be verified*/
        .platform_data = NULL , /* used in spi_driver (optional) */
     }            	 
};

static int spi_info_25lc020_init(void)
{
    return spi_register_board_info(spi_info_25lc020, ARRAY_SIZE(spi_info_25lc020));
}


module_init(spi_info_25lc020_init);
MODULE_DESCRIPTION("EEPROM 25LC020 SPI Driver");
MODULE_AUTHOR("Muyao");
MODULE_LICENSE("GPL");
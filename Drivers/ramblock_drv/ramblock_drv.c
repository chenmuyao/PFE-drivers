
/* Référence:
 * drivers\block\xd.c
 * drivers\block\z2ram.c
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/genhd.h>
#include <linux/hdreg.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/blkdev.h>
#include <linux/blkpg.h>
#include <linux/delay.h>
#include <linux/io.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/dma.h>

static struct gendisk *ramblock_disk;
static struct request_queue *ramblock_queue;

static int major;

static DEFINE_SPINLOCK(ramblock_lock);

#define RAMBLOCK_SIZE (1024*1024)
static unsigned char *ramblock_buf;

/*Décription du disk*/
static int ramblock_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	/* capacité=heads*cylinders*sectors*512 */
	geo->heads     = 2;
	geo->cylinders = 32;
	geo->sectors   = RAMBLOCK_SIZE/2/32/512;
	return 0;
}


static struct block_device_operations ramblock_fops = {
	.owner	= THIS_MODULE,
	.getgeo	= ramblock_getgeo,
};

static void do_ramblock_request(struct request_queue * q)
{
	struct request *req;

	while ((req = blk_fetch_request(q)) != NULL) {
		
		/* Déterminer la taille du fichier à transférer*/
		unsigned long offset = req->__sector * 512;	
		unsigned long len = blk_rq_cur_sectors(req) * 512;

		/*READ et WRITE*/
		if (rq_data_dir(req) == READ)
		{
			//printk("do_ramblock_request read %d\n", ++r_cnt);
			memcpy(req->buffer, ramblock_buf+offset, len);
		}
		else
		{
			//printk("do_ramblock_request write %d\n", ++w_cnt);
			memcpy(ramblock_buf+offset, req->buffer, len);
		}		
		
		__blk_end_request_all(req , 0);
	}
}

static int ramblock_init(void)
{
	/* Créer un disk virtuel */
	ramblock_disk = alloc_disk(16);

	/*Configuration*/ 
	/* 1. Créer un queue d'attente et allouer la fonction de lecture/écriture */
	ramblock_queue = blk_init_queue(do_ramblock_request, &ramblock_lock); 
	// Structure queue pour faciliter la lecture/écriture

	ramblock_disk->queue = ramblock_queue;
	
	/* 2. Configurer les autres attributs */
	major = register_blkdev(0, "ramblock");  /* 	cat /proc/devices */	
	ramblock_disk->major      		  = major;
	ramblock_disk->first_minor = 0;
	sprintf(ramblock_disk->disk_name, "ramblock");
	ramblock_disk->fops        = &ramblock_fops;
	set_capacity(ramblock_disk, RAMBLOCK_SIZE / 512); // Prendre "sector" comme l'argument
	// Linux always considers sectors to be 512 bytes long independently of the devices real block size.

	/* 3. Allouer la mémoire pour le buffer - (dans la RAM) */
	ramblock_buf = kzalloc(RAMBLOCK_SIZE, GFP_KERNEL);

	/* 4. Enregistrer le disk */
	add_disk(ramblock_disk);

	return 0;
}

static void ramblock_exit(void)
{
	unregister_blkdev(major, "ramblock");
	del_gendisk(ramblock_disk);
	put_disk(ramblock_disk);
	blk_cleanup_queue(ramblock_queue);

	kfree(ramblock_buf);
}

module_init(ramblock_init);
module_exit(ramblock_exit);

MODULE_LICENSE("GPL");

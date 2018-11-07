#Makefile for driver

KERN_DIR = /home/chenmy/ARMLINUX/Linux_kernel/linux

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m   += spi_drv.o

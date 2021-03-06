### 2018.10.1 Set up uboot

reference : 

https://elinux.org/Building_BBB_Kernel

https://wiki.beyondlogic.org/index.php/BeagleBoneBlack_Building_Kernel

http://beagleboard.org/linux

#### Pb1 :

```
chenmy@chenmy-UBT:~/ARMLINUX/PFE-drivers/uboot/u-boot-2017.09$ make tools-only
scripts/kconfig/conf  --silentoldconfig Kconfig
***
*** Configuration file ".config" not found!
***
*** Please run some configurator (e.g. "make oldconfig" or
*** "make menuconfig" or "make xconfig").
***
scripts/kconfig/Makefile:46: recipe for target 'silentoldconfig' failed
make[2]: *** [silentoldconfig] Error 1
Makefile:478: recipe for target 'silentoldconfig' failed
make[1]: *** [silentoldconfig] Error 2
make: *** No rule to make target 'include/config/auto.conf', needed by 'include/config/uboot.release'.  Stop.

```

#### Solution:

Install sudo apt-get install libssl-dev

use:

```
make sandbox_defconfig tools-only
```



### 2018.10.2 Compile Linux

http://beagleboard.org/linux Example build procedure for BeagleBone 3.8-rcX kernel

I followed the instructions in "Building_BBB_Kernel", though the kernel.git has been replaced by linux.git

#### Pb

```shell
In file included from include/linux/compiler.h:54:0,
                 from include/uapi/linux/stddef.h:1,
                 from include/linux/stddef.h:4,
                 from /home/chenmy/ARMLINUX/PFE-drivers/kbuild/kernel/kernel/include/uapi/linux/posix_types.h:4,
                 from include/uapi/linux/types.h:13,
                 from include/linux/types.h:5,
                 from include/linux/page-flags.h:8,
                 from kernel/bounds.c:9:
include/linux/compiler-gcc.h:100:1: fatal error: linux/compiler-gcc7.h: No such file or directory
 #include gcc_header(__GNUC__)
 ^~~~
compilation terminated.

```

Maybe it's the problem of gcc's version



2 possible solutions : Use an older gcc / Abandon this method.

 



#### Pb How to find serial port number

#### Solution:

Simple run dmesg command
`$ dmesg | grep tty`



### 2018.10.3-10.5

#### Building UBOOT

uboot : Mentioned above

GCC compiler : arm-linux-gnueabihf-gcc (Ubuntu/Linaro 7.3.0-27ubuntu1~18.04) 7.3.0

```shell
chenmy@chenmy-UBT:~/ARMLINUX/PFE-drivers/uboot/u-boot-2017.09$ arm-linux-gnueabihf-gcc --version
arm-linux-gnueabihf-gcc (Ubuntu/Linaro 7.3.0-27ubuntu1~18.04) 7.3.0
Copyright (C) 2017 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Steps : 

1. Compile the defconfig

   1. find the defconfig provided by BBB in ./configs/am335x_boneblack_defconfig

   2. `make am335x_boneblack_defconfig `// `make am335x_evm_defconfig ``make omap3_beagle_config `

      1. result 

         ```
         #
         # configuration written to .config
         #
         
         ```

2. Compile the uboot image

   1. `make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4` (-jx to be faster, x is the core number)

3. Result 

   1. MLO, u-boot.img...


#### Building Linux

GCC

Linux 4.4 : 

```shell
git clone https://github.com/beagleboard/linux.git
git checkout 4.4
```



Steps : 

Simple way : **change the ./jenkins_build.sh file , comment the last commande `make ARCH=arm clean`**

Then run the ./jenkins_build.sh file

Result : in ./arch/arm/boot  There will be the zImage



Hard way (consulting jenkins_build.sh):

1. Compile the defconfig `make ARCH=arm bb.org_defconfig`
2. Compile the zImage  `make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage -j4`
3. Compile the modules `make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules -j4`
4. Compile the dtbs `make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs -j4`



#### Flashing the sd card

Software : fdisk

```sh
sudo fdisk /dev/sdX
p : show partition
d : delete partition
x : Expert mode 
	h
	s
	c
n : new partition
	p
	1
	+6g
n - p - 2 - use default values
t - 1 - c Changed system type of partition 1 to c (W95 FAT32 (LBA))
a - 1 Set as bootable
w - write

sudo mkfs.vfat -F 32 -n boot /dev/sdX1
sudo mkfs.ext3 -L rootfs /dev/sdX2
```

Copy MLO then u-boot.img to the first partition on sd card (boot)

It should be OK...



#### PB

**No output, nothing**

Possible reason : 

- SD card flashing method is wrong
  - Have checked many tutos, found no different way
- U-boot Problem
  - Have used different config files...



### 2018.10.11 Install Debian System

Resource : https://beagleboard.org/getting-started

Images : https://beagleboard.org/latest-images

Distro Chosen : Debian 9.5 2018-10-07 4GB SD IoT image

Tool used : Etcher

Process : Download Image - Etcher - Flash...



**-----------------------------------------------------------  IMPORTANT------------------------------------------------------**

Use a FTDI device to debug (to see all the booting process and even toggle with the Uboot)

https://www.blaess.fr/christophe/2013/05/17/console-serie-de-debug-pour-beaglebone-black/

GND - GND (**J1** on BBB, Black square)

FTDI Tx - Rx (Position 4)

FTDI Rx - Tx (Position 5)

**--------------------------------------------------------------------------------------------------------------------------------------------**



#### PROBLEM SOLVED

http://linux-sunxi.org/Bootable_SD_card 

`dd` `if``=u-boot-sunxi-with-spl.bin of=${card} bs=1024 seek=8`

http://processors.wiki.ti.com/index.php/AM335x_U-Boot_User%27s_Guide

Select “u-boot-spl.bin” for the transfer


<<<<<<< HEAD

**In fact, the ancient solution works, we just have to use DEBUGGER!!!**



### 2018.10.14

#### Pb Hang after "Starting Kernel..."

[TI solution]: http://processors.wiki.ti.com/index.php/Kernel_-_Common_Problems_Booting_Linux#Problem_.231_-_No_more_output_is_seen_on_the_console_after_.22Starting_Kernel....22



#### Solution

Using Linux 3.8 uImage

=> New Problem : Wrong ID  => Adding .dtb => New problem => hang after "booting the kernel"

```shell
=> fatload mmc 0 0x82000000 uImage
5587504 bytes read in 350 ms (15.2 MiB/s)
=> fatload mmc 0 0x88000000 am335x-boneblack-uboot.dtb
59066 bytes read in 6 ms (9.4 MiB/s)
=> bootm 0x82000000 - 0x88000000
## Booting kernel from Legacy Image at 82000000 ...
   Image Name:   Linux-3.8.13
   Created:      2018-10-13  15:12:14 UTC
   Image Type:   ARM Linux Kernel Image (uncompressed)
   Data Size:    5587440 Bytes = 5.3 MiB
   Load Address: 80008000
   Entry Point:  80008000
   Verifying Checksum ... OK
## Flattened Device Tree blob at 88000000
   Booting using the fdt blob at 0x88000000
   Loading Kernel Image ... OK
   Loading Device Tree to 8ffee000, end 8ffff6b9 ... OK

Starting kernel ...

Uncompressing Linux... done, booting the kernel.

```



Possible solution : Configuration(Try and Use env def config + omap) ========= NOT working

http://processors.wiki.ti.com/index.php/Linux_Core_U-Boot_User%27s_Guide



For the newest problem , try to use other .dtb, not working.





### 2018.10.15 Resolved 

[Solution]: http://processors.wiki.ti.com/index.php/AM335x_U-Boot_User%27s_Guide#SD_.28Secured_Digital_card.29	"Setting U-Boot environment using uEnv.txt"



uEnv.txt : 

```
bootargs=console=ttyO0,115200n8 root=/dev/mmcblk0p2 mem=1G rw rootwait
bootcmd=mmc rescan; fatload mmc 0 0x82000000 uImage; fatload mmc 0 0x88000000 am335x-boneblack.dtb; bootm 0x82000000 - 0x88000000
uenvcmd=boot
```



#### Summary

zImage of Linux 4.4	Not working									Hang at "Starting Kernel..."

uImage of Linux 3.8 	Not working without proper driver tree binary 	Error of wrong ID

uImage of Linux 3.8	Not working without proper uEnv.txt			Hang at "booting the kernel."

uImage of Linux 3.8 	WORKING with proper uEnv.txt



#### File system

resource : 

https://getpocket.com/a/read/2346376898

https://www.digikey.com/eewiki/display/linuxonarm/BeagleBone+Black#BeagleBoneBlack-Setuname_rin/boot/uEnv.txt

http://www.armhf.com/boards/beaglebone-black/bbb-sd-install/



Format SD card and copy file system

```sh
sudo umount /media/chenmy/rootfs 
sudo mkfs.ext3 -L rootfs /dev/sdX2  # mkf4 not working : Cannot find file system
tar xf ***-armhf-***.tar #unzip to local
cd ***-armhf-***
tar xvfp armhf-rootfs-****.tar -C /media/chenmy/rootfs/
sync

```

ubuntu-18.04.1-minimal-armhf-2018-07-30 **working**

```sh
wget -c https://rcn-ee.com/rootfs/eewiki/minfs/ubuntu-18.04.1-minimal-armhf-2018-07-30.tar.xz
```



debian-9.5-minimal-armhf-2018-07-30.tar **Not working** 	Error : "Unable to cd /home/debian"



**Till now, UBUNTU on BBB!**



### 2018.10.20 First driver

Resource: **DriverCreationGuide.pdf**

1. The kernel version must correspond to the compiling environment of driver program
   1. **Cannot use the official image. We must have the initial compiling environment.**
2. The defconfig doesn't support ethernet on USB - **Maybe can be solved in the configuration** No... It's a driver...
   1. **Have to connect with a router to transmit files**
   2. `scp FILE_NAME ubuntu@192.168.1.100:/home/ubuntu/Drivers/first_drv/`
   3. **SOLUTION : ** **use an ethernet to usb connector**, the one we used in TP
3. **To program a LED** (Hardware)
   1. Find out the PIN to toggle with - **Schema of board**
   2. Find out how pins are initialized (steps, registers) - **User's Manuel**
   3. Find out the helpful libraries - **Kernel codes**



### 2018.10.24

https://stackoverflow.com/questions/29480980/generating-a-tarball-of-linux-modules

**How to generate modules tarball**



#### Pb : 

Have to use the same version to compile the driver program. 

​	Cannot use Buildroot (No make environment provided) 

​	Cannot use official Image

**Disadvantage** : 

No **Ethernet over USB**. - Don't know how to add this function yet.

Solution : Ethernet USB adapter + Ethernet Cable ? - **Solved**(In the Tuto 2018.10.27)



### 2018.11.18

https://stackoverflow.com/questions/22632713/how-to-write-a-simple-linux-device-driver
https://community.nxp.com/docs/DOC-335264
https://fr.slideshare.net/sysplay/spi-drivers
http://invo-tronics.com/spi-driver-for-linux-based-embedded-system/

https://elixir.bootlin.com/linux/v3.8/source/drivers/misc/eeprom/at25.c



http://processors.wiki.ti.com/index.php/AM335x_McSPI_Driver%27s_Guide TI am335x SPI 



### 2018.1.8

https://www.kernel.org/doc/Documentation/blockdev/ramdisk.txt

https://myroughnotes.wordpress.com/2012/10/08/sample-block-device-driver-linux-kernel-3-x/
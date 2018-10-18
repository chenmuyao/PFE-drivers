# Linux sur BBB

### 1 Préparation

**Objectif** : Lancer une **application** <- **Système** (Linux) qui founit les ressources nécessaires <- **Bootloader** (U-Boot) pour démarrer un sytème (Copy - Exécuter)



#### 1-1 Toolchain

Compilateur croisée : gcc-arm-linux

```sh
sudo apt install gcc-arm-linux-gnueabihf-
```



#### 1-2 Format de la carte SD

On a besoin de 2 partition, une pour **boot**, l'autre pour le système de fichier (**rootfs**)

- Lister les disks présents

```sh
fdisk -l
fdisk: cannot open /dev/loop0: Permission denied
fdisk: cannot open /dev/loop1: Permission denied
fdisk: cannot open /dev/loop2: Permission denied
fdisk: cannot open /dev/loop3: Permission denied
fdisk: cannot open /dev/loop4: Permission denied
fdisk: cannot open /dev/loop5: Permission denied
fdisk: cannot open /dev/loop6: Permission denied
fdisk: cannot open /dev/loop7: Permission denied
fdisk: cannot open /dev/sda: Permission denied
fdisk: cannot open /dev/loop8: Permission denied
fdisk: cannot open /dev/loop9: Permission denied
fdisk: cannot open /dev/loop10: Permission denied
fdisk: cannot open /dev/loop11: Permission denied
fdisk: cannot open /dev/loop12: Permission denied
fdisk: cannot open /dev/loop13: Permission denied
fdisk: cannot open /dev/loop14: Permission denied
fdisk: cannot open /dev/sdb: Permission denied

```

- Nettoyer la carte SD

```sh
sudo fdisk /dev/sdb
Welcome to fdisk (util-linux 2.31.1).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.


Command (m for help): m

Help:

  DOS (MBR)
   a   toggle a bootable flag
   b   edit nested BSD disklabel
   c   toggle the dos compatibility flag

  Generic
   d   delete a partition
   F   list free unpartitioned space
   l   list known partition types
   n   add a new partition
   p   print the partition table
   t   change a partition type
   v   verify the partition table
   i   print information about a partition

  Misc
   m   print this menu
   u   change display/entry units
   x   extra functionality (experts only)

  Script
   I   load disk layout from sfdisk script file
   O   dump disk layout to sfdisk script file

  Save & Exit
   w   write table to disk and exit
   q   quit without saving changes

  Create a new label
   g   create a new empty GPT partition table
   G   create a new empty SGI (IRIX) partition table
   o   create a new empty DOS partition table
   s   create a new empty Sun partition table

Command (m for help): p
Disk /dev/sdb: 14,9 GiB, 15931539456 bytes, 31116288 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000

Device     Boot Start      End  Sectors  Size Id Type
/dev/sdb1  *     8192 31116287 31108096 14,9G  c W95 FAT32 (LBA)

Command (m for help): d
Selected partition 1
Partition 1 has been deleted.

Command (m for help): p
Disk /dev/sdb: 14,9 GiB, 15931539456 bytes, 31116288 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000
```

m - menu

d - supprimer une partition

- Créer la première partition du type FAT pour BOOT

```sh
Command (m for help): n
Partition type
   p   primary (0 primary, 0 extended, 4 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (1-4, default 1): 1
First sector (2048-31116287, default 2048): 2048
Last sector, +sectors or +size{K,M,G,T,P} (2048-31116287, default 31116287): +1G

Created a new partition 1 of type 'Linux' and of size 1 GiB.

Command (m for help): p
Disk /dev/sdb: 14,9 GiB, 15931539456 bytes, 31116288 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000

Device     Boot Start     End Sectors Size Id Type
/dev/sdb1        2048 2099199 2097152   1G 83 Linux

Command (m for help): t
Selected partition 1
Hex code (type L to list all codes): c
Changed type of partition 'Linux' to 'W95 FAT32 (LBA)'.

Command (m for help): p
Disk /dev/sdb: 14,9 GiB, 15931539456 bytes, 31116288 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000

Device     Boot Start     End Sectors Size Id Type
/dev/sdb1        2048 2099199 2097152   1G  c W95 FAT32 (LBA)
```

- Créer la deuxième partition du type Linux pour FS

```sh
Command (m for help): n
Partition type
   p   primary (1 primary, 0 extended, 3 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (2-4, default 2): 2
First sector (2099200-31116287, default 2099200): 
Last sector, +sectors or +size{K,M,G,T,P} (2099200-31116287, default 31116287): 

Created a new partition 2 of type 'Linux' and of size 13,9 GiB.

Command (m for help): p
Disk /dev/sdb: 14,9 GiB, 15931539456 bytes, 31116288 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x00000000

Device     Boot   Start      End  Sectors  Size Id Type
/dev/sdb1          2048  2099199  2097152    1G  c W95 FAT32 (LBA)
/dev/sdb2       2099200 31116287 29017088 13,9G 83 Linux

Command (m for help): w
The partition table has been altered.
```



- Formatter les deux partition

```sh
sudo umount /dev/sdX1
sudo umount /dev/sdX2
sudo mkfs.vfat -F 32 -n boot /dev/sdb1
mkfs.fat 4.1 (2017-01-24)
mkfs.fat: warning - lowercase labels might not work properly with DOS or Windows
sudo mkfs.ext3 -L rootfs /dev/sdb2
mke2fs 1.44.1 (24-Mar-2018)
Creating filesystem with 3627136 4k blocks and 907536 inodes
Filesystem UUID: 760c9e34-d7fd-448f-8413-faa76702a594
Superblock backups stored on blocks: 
	32768, 98304, 163840, 229376, 294912, 819200, 884736, 1605632, 2654208

Allocating group tables: done                            
Writing inode tables: done                            
Creating journal (16384 blocks): done
Writing superblocks and filesystem accounting information: done  
```





#### 1-3 Debugger sur BBB

- FTDI

  ![Image associÃ©e](http://lechacalshop.com/163-thickbox_default/ftdi-usb-to-ttl-serial-converter-adapter-ft232rl.jpg)

![img](https://www.blaess.fr/christophe/wp-content/uploads/2013/05/img-02.jpg)

- Branchement
  - GND - GND (J1 sur BBB, coté carée noire)
  - FTDI Tx - Rx (Position 4)
  - FTDI Rx - Tx (Position 5)

#### 1.4 Picocom pour la liaison série

```sh
sudo picocom -l -b 115200 /dev/ttyUSB0
```





### 2 U-Boot

#### 2-1 Téléchargement

https://github.com/u-boot/u-boot

```sh
git clone https://github.com/u-boot/u-boot.git
```

#### 2-2 Compilation Config

- Trouver la bonne defconfig

```sh
find -name "am335*defconfig"
./configs/am335x_shc_sdboot_prompt_defconfig
./configs/am335x_hs_evm_defconfig
./configs/am335x_boneblack_defconfig
./configs/am335x_shc_defconfig
./configs/am335x_hs_evm_uart_defconfig
./configs/am335x_shc_prompt_defconfig
./configs/am335x_sl50_defconfig
./configs/am335x_evm_defconfig
./configs/am335x_evm_nor_defconfig
./configs/am335x_pdu001_defconfig
./configs/am335x_shc_netboot_defconfig
./configs/am335x_shc_ict_defconfig
./configs/am335x_igep003x_defconfig
./configs/am335x_evm_usbspl_defconfig
./configs/am335x_boneblack_vboot_defconfig
./configs/am335x_evm_norboot_defconfig
./configs/am335x_evm_spiboot_defconfig
./configs/am335x_shc_sdboot_defconfig
./configs/am335x_baltos_defconfig


```

C'est **am335x_boneblack_defconfig** qui nous intéresse.

- make defconfig

```sh
 make ARCH=arm am335x_boneblack_defconfig
  HOSTCC  scripts/basic/fixdep
  HOSTCC  scripts/kconfig/conf.o
  YACC    scripts/kconfig/zconf.tab.c
  LEX     scripts/kconfig/zconf.lex.c
  HOSTCC  scripts/kconfig/zconf.tab.o
  HOSTLD  scripts/kconfig/conf
#
# configuration written to .config
#
```

On peut utiliser `make menuconfig` pour changer des paramètres

#### 2-3 Compilation croisée de Uboot

- make all

```sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4
...
  CC      spl/drivers/usb/musb-new/musb_gadget.o
  LD      spl/drivers/watchdog/built-in.o
  CC      spl/drivers/usb/musb-new/musb_gadget_ep0.o
  CC      spl/drivers/serial/ns16550.o
  CC      spl/drivers/serial/serial_omap.o
  LD      spl/drivers/serial/built-in.o
  CC      spl/drivers/usb/musb-new/musb_core.o
  CC      spl/drivers/usb/musb-new/musb_uboot.o
  CC      spl/drivers/usb/musb-new/musb_host.o
  CC      spl/drivers/usb/musb-new/musb_dsps.o
  LD      spl/fs/fat/built-in.o
  LD      spl/fs/built-in.o
  SYM     u-boot.sym
  COPY    u-boot.bin
  MKIMAGE u-boot.img
  LD      spl/drivers/usb/musb-new/built-in.o
  LD      spl/drivers/built-in.o
  LD      spl/u-boot-spl
  OBJCOPY spl/u-boot-spl-nodtb.bin
  COPY    spl/u-boot-spl.bin
  MKIMAGE MLO.byteswap
  MKIMAGE MLO
  CFGCHK  u-boot.cfg

```

**-j4** pour maximiser l'utilisation de CPU (4 dépend du nombre de coeurs du machine de développement)



- Terminé. On aura le Uboot.img et le bootloader secondaire MLO

```sh
ls
api        Documentation  lib           scripts             u-boot.img
arch       drivers        Licenses      spl                 u-boot.lds
board      dts            MAINTAINERS   System.map          u-boot.map
cmd        env            Makefile      test                u-boot-nodtb.bin
common     examples       MLO           tools               u-boot.srec
config.mk  fs             MLO.byteswap  u-boot              u-boot.sym
configs    include        net           u-boot.bin
disk       Kbuild         post          u-boot.cfg
doc        Kconfig        README        u-boot.cfg.configs

```



#### 2-4 UBOOT sur carte SD

- Copier **MLO** et **u-boot.img** à .../boot/

```sh
cp MLO /media/.../boot
cp u-boot.img /media/.../boot
sync
```

**sync** pour etre sur que tout est écrit sur la carte



Brancher d'abord le debugger, ouvrir une session de Picocom (1.4), brancher ensuite l'alim' en appuyant sur Button S2.

```sh
U-Boot 2018.11-rc1-00130-g0a60a81ba3 (Oct 14 2018 - 11:39:49 +0200)

CPU  : AM335X-GP rev 2.1
I2C:   ready
DRAM:  512 MiB
No match for driver 'omap_hsmmc'
No match for driver 'omap_hsmmc'
Some drivers were not found
MMC:   OMAP SD/MMC: 0, OMAP SD/MMC: 1
Loading Environment from FAT... *** Warning - bad CRC, using default environment

Loading Environment from MMC... *** Warning - bad CRC, using default environment

<ethaddr> not set. Validating first E-fuse MAC
Net:   cpsw, usb_ether
Press SPACE to abort autoboot in 2 seconds
=>    
```



### 3 Linux

- Téléchargement

https://github.com/beagleboard/linux

```sh
git clone https://github.com/beagleboard/linux.git
cd linux
git checkout 3.8
```

- Compilation defconfig

```sh
find -name bb.org*
./arch/arm/configs/bb.org_defconfig
make ARCH=arm bb.org_defconfig
  HOSTCC  scripts/basic/fixdep
  HOSTCC  scripts/kconfig/conf.o
  SHIPPED scripts/kconfig/zconf.tab.c
  SHIPPED scripts/kconfig/zconf.lex.c
  SHIPPED scripts/kconfig/zconf.hash.c
  HOSTCC  scripts/kconfig/zconf.tab.o
In file included from scripts/kconfig/zconf.tab.c:2503:0:
scripts/kconfig/menu.c: In function ‘get_symbol_str’:
scripts/kconfig/menu.c:561:18: warning: ‘jump’ may be used uninitialized in this function [-Wmaybe-uninitialized]
     jump->offset = r->len - 1;
     ~~~~~~~~~~~~~^~~~~~~~~~~~
scripts/kconfig/menu.c:515:19: note: ‘jump’ was declared here
  struct jump_key *jump;
                   ^~~~
  HOSTLD  scripts/kconfig/conf
drivers/video/Kconfig:60:error: recursive dependency detected!
drivers/video/Kconfig:60:	symbol FB is selected by DRM_KMS_HELPER
drivers/gpu/drm/Kconfig:28:	symbol DRM_KMS_HELPER is selected by DRM_TILCDC
drivers/gpu/drm/tilcdc/Kconfig:1:	symbol DRM_TILCDC depends on BACKLIGHT_LCD_SUPPORT
drivers/video/backlight/Kconfig:5:	symbol BACKLIGHT_LCD_SUPPORT is selected by FB_BACKLIGHT
drivers/video/Kconfig:247:	symbol FB_BACKLIGHT is selected by PMAC_BACKLIGHT
drivers/macintosh/Kconfig:134:	symbol PMAC_BACKLIGHT depends on FB
warning: (USB_MUSB_HDRC) selects TWL6030_USB which has unmet direct dependencies (USB_SUPPORT && (USB || USB_GADGET) && TWL4030_CORE && OMAP_USB2 && USB_MUSB_OMAP2PLUS)
warning: (DVB_USB_PCTV452E) selects TTPCI_EEPROM which has unmet direct dependencies (MEDIA_SUPPORT && MEDIA_PCI_SUPPORT && MEDIA_DIGITAL_TV_SUPPORT && I2C)
warning: (USB_MUSB_HDRC) selects TWL6030_USB which has unmet direct dependencies (USB_SUPPORT && (USB || USB_GADGET) && TWL4030_CORE && OMAP_USB2 && USB_MUSB_OMAP2PLUS)
warning: (DVB_USB_PCTV452E) selects TTPCI_EEPROM which has unmet direct dependencies (MEDIA_SUPPORT && MEDIA_PCI_SUPPORT && MEDIA_DIGITAL_TV_SUPPORT && I2C)
#
# configuration written to .config
#
```

- Compilation noyau (>10min selon la performance de la machine de développement)

```sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -j4
...
  IHEX2FW firmware/keyspan_pda/keyspan_pda.fw
  IHEX2FW firmware/keyspan_pda/xircom_pgs.fw
  IHEX    firmware/cpia2/stv0672_vp4.bin
  IHEX    firmware/yam/1200.bin
  IHEX    firmware/yam/9600.bin
```

- Terminé

```sh
ls ./arch/arm/boot/
bootp  compressed  dts  Image  install.sh  Makefile  uImage  zImage
```

Utiliser **uImage** et **./arch/arm/boot/dts/am335x-boneblack.dtb**

- Mettre sur la carte SD

```sh
cp ./arch/arm/boot/uImage /dev/.../boot
cp ./arch/arm/boot/dts/am335x-boneblack.dtb /dev/.../boot
```

- Ajout du fichier **uEnv.txt** dans /dev/.../boot

```
bootargs=console=ttyO0,115200n8 root=/dev/mmcblk0p2 mem=1G rw rootwait
bootcmd=mmc rescan; fatload mmc 0 0x82000000 uImage; fatload mmc 0 0x88000000 am335x-boneblack.dtb; bootm 0x82000000 - 0x88000000
uenvcmd=boot
```



### 4 Système de fichier

- Téléchargement

```sh
wget -c https://rcn-ee.com/rootfs/eewiki/minfs/ubuntu-18.04.1-minimal-armhf-2018-07-30.tar.xz
```

- unzip et le mettre sur la carte SD

```sh
tar xf ubuntu-18.04.1-minimal-armhf-2018-07-30.tar.xz
cd ubuntu-18.04.1-minimal-armhf-2018-07-30/
tar xfvp armhf-rootfs-ubuntu-bionic.tar /media/.../rootfs
```



### Voilà boot .... et c'est FINI!


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
```

- Nettoyer la carte SD

```sh
sudo fdisk /dev/sdb

```

m - menu

d - supprimer une partition

- Créer la première partition du type FAT pour BOOT

```sh

```

- Créer la deuxième partition du type Linux pour FS

```sh

```



#### 1-3 Debugger sur BBB

- FTDI

  ![Image associÃ©e](http://lechacalshop.com/163-thickbox_default/ftdi-usb-to-ttl-serial-converter-adapter-ft232rl.jpg)

![img](https://www.blaess.fr/christophe/wp-content/uploads/2013/05/img-02.jpg)

- Branchement
  - GND - GND (J1 sur BBB, coté carée noire)
  - FTDI Tx - Rx (Position 4)
  - FTDI Rx - Tx (Position 5)



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



Brancher d'abord le debugger, brancher ensuite l'alim' en appuyant sur Button S2

```sh

```







### 3 Linux




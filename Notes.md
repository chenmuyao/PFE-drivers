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

   2. `make am335x_boneblack_defconfig `

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


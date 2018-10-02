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




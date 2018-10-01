### 2018.10.1 Set up uboot

reference : 

https://elinux.org/Building_BBB_Kernel

https://wiki.beyondlogic.org/index.php/BeagleBoneBlack_Building_Kernel

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


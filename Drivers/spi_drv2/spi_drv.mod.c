#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xae4b0ceb, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x86c8ad2e, "malloc_sizes" },
	{ 0xb45e1167, "device_destroy" },
	{ 0xcd7317e5, "__register_chrdev" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0x27e1a049, "printk" },
	{ 0x9660b5c3, "driver_unregister" },
	{ 0x6c4fc9f7, "device_create" },
	{ 0x289ccb58, "spi_sync" },
	{ 0x8758668a, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x2f729c13, "class_destroy" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xaa69cf53, "spi_register_driver" },
	{ 0x95d52825, "__class_create" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "E9718EBE3D93AB77F103A0B");
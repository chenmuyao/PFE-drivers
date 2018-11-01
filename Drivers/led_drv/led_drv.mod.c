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
	{ 0xfe990052, "gpio_free" },
	{ 0x2f729c13, "class_destroy" },
	{ 0x5bcb149, "device_unregister" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x82f776b7, "gpio_export" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x6c4fc9f7, "device_create" },
	{ 0x95d52825, "__class_create" },
	{ 0xcd7317e5, "__register_chrdev" },
	{ 0x27e1a049, "printk" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xa8f59416, "gpio_direction_output" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "1B2D7C56E1F53ABD31ECECF");
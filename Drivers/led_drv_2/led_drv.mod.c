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
	{ 0xc2165d85, "__arm_iounmap" },
	{ 0x2f729c13, "class_destroy" },
	{ 0x5bcb149, "device_unregister" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0x6c4fc9f7, "device_create" },
	{ 0x95d52825, "__class_create" },
	{ 0xcd7317e5, "__register_chrdev" },
	{ 0x27e1a049, "printk" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0xfa2a45e, "__memzero" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "11F12D0E1522BA5BBED0D74");

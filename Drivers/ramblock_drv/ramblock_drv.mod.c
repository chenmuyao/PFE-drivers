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
	{ 0x37a0cba, "kfree" },
	{ 0xa9da1804, "blk_cleanup_queue" },
	{ 0x21ba6011, "put_disk" },
	{ 0x1e77a421, "del_gendisk" },
	{ 0xb5a459dc, "unregister_blkdev" },
	{ 0x8237633c, "add_disk" },
	{ 0x8758668a, "kmem_cache_alloc_trace" },
	{ 0x71a50dbc, "register_blkdev" },
	{ 0x2678b833, "blk_init_queue" },
	{ 0x86c8ad2e, "malloc_sizes" },
	{ 0xfdb26809, "alloc_disk" },
	{ 0x75c2c5c8, "blk_fetch_request" },
	{ 0xf3513ea1, "__blk_end_request_all" },
	{ 0x9d669763, "memcpy" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EE946682B04B8AD03B9DB27");

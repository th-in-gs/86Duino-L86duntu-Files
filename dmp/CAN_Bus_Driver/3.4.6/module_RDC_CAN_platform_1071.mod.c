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
	{ 0xe478cefa, "module_layout" },
	{ 0x91715312, "sprintf" },
	{ 0x50eedeb8, "printk" },
	{ 0xd136d21c, "platform_device_del" },
	{ 0x93c730b0, "platform_device_alloc" },
	{ 0x81dbb21, "platform_device_add" },
	{ 0xb4390f9a, "mcount" },
	{ 0x945b2423, "platform_device_add_resources" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x4845c423, "param_array_ops" },
	{ 0x6d044c26, "param_ops_uint" },
	{ 0x2482e688, "vsprintf" },
	{ 0x3cbac4cf, "platform_device_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "33D1FA9D7AF76DEE7EB06B7");

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
	{ 0xadf42bd5, "__request_region" },
	{ 0xa24d1581, "kmalloc_caches" },
	{ 0x6dae1c2f, "RDCCAN_shared_irq_reference" },
	{ 0x788fe103, "iomem_resource" },
	{ 0x7c309c51, "dev_set_drvdata" },
	{ 0x4f9c0615, "RDCCAN_shutdown_port_device" },
	{ 0x4238e35c, "RDCCAN_suspend_candev" },
	{ 0x8a27c56b, "RDCCAN_initialize_port_device" },
	{ 0x75d0047, "RDCCAN_register_candev" },
	{ 0x91715312, "sprintf" },
	{ 0xd7c7da68, "pnp_unregister_driver" },
	{ 0x96dbef9d, "RDCCAN_unregister_candev" },
	{ 0x59d8223a, "ioport_resource" },
	{ 0x50eedeb8, "printk" },
	{ 0x26fcd06, "RDCCAN_setup_port_device" },
	{ 0xb4390f9a, "mcount" },
	{ 0xf94ab21f, "pnp_register_driver" },
	{ 0xf54c1829, "pnp_get_resource" },
	{ 0x3af98f9e, "ioremap_nocache" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x45586b81, "RDCCAN_unregister_port_device" },
	{ 0x9bce482f, "__release_region" },
	{ 0x864d3d1e, "kmem_cache_alloc_trace" },
	{ 0x6225a28e, "RDCCAN_shared_irq_dereference" },
	{ 0x37a0cba, "kfree" },
	{ 0xedc03953, "iounmap" },
	{ 0xcfc505ea, "RDCCAN_resume_candev" },
	{ 0x2482e688, "vsprintf" },
	{ 0x5401d0b3, "dev_get_drvdata" },
	{ 0xa63ae769, "RDCCAN_register_port_device" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=module_RDC_SocketCAN";

MODULE_ALIAS("pnp:dPNP0513*");
MODULE_ALIAS("acpi*:PNP0513:*");

MODULE_INFO(srcversion, "10142E1433C7A2D36398137");

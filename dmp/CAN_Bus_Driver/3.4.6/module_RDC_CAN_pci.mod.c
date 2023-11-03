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
	{ 0x72a1baa4, "pci_bus_read_config_byte" },
	{ 0x6dae1c2f, "RDCCAN_shared_irq_reference" },
	{ 0x788fe103, "iomem_resource" },
	{ 0x7c309c51, "dev_set_drvdata" },
	{ 0x831f045c, "pci_disable_device" },
	{ 0x4f9c0615, "RDCCAN_shutdown_port_device" },
	{ 0x4238e35c, "RDCCAN_suspend_candev" },
	{ 0x8a27c56b, "RDCCAN_initialize_port_device" },
	{ 0x75d0047, "RDCCAN_register_candev" },
	{ 0xd53905ed, "pci_bus_write_config_word" },
	{ 0x91715312, "sprintf" },
	{ 0x96dbef9d, "RDCCAN_unregister_candev" },
	{ 0x59d8223a, "ioport_resource" },
	{ 0x3c55edf, "pci_restore_state" },
	{ 0x50eedeb8, "printk" },
	{ 0x26fcd06, "RDCCAN_setup_port_device" },
	{ 0xb4390f9a, "mcount" },
	{ 0x93acd4ee, "pci_bus_write_config_dword" },
	{ 0xe3dc6805, "__pci_enable_wake" },
	{ 0xf17b7746, "pci_get_domain_bus_and_slot" },
	{ 0x3af98f9e, "ioremap_nocache" },
	{ 0xf014794d, "pci_bus_read_config_word" },
	{ 0x2e295a8c, "pci_bus_read_config_dword" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x45586b81, "RDCCAN_unregister_port_device" },
	{ 0x9bce482f, "__release_region" },
	{ 0x2ef7345a, "pci_unregister_driver" },
	{ 0x864d3d1e, "kmem_cache_alloc_trace" },
	{ 0xae4fbad1, "pci_set_power_state" },
	{ 0x6225a28e, "RDCCAN_shared_irq_dereference" },
	{ 0x1eb30225, "pci_bus_write_config_byte" },
	{ 0x37a0cba, "kfree" },
	{ 0x4845c423, "param_array_ops" },
	{ 0xedc03953, "iounmap" },
	{ 0x1231562a, "__pci_register_driver" },
	{ 0xa8610c05, "pci_dev_put" },
	{ 0xcfc505ea, "RDCCAN_resume_candev" },
	{ 0xd86b65f5, "pci_choose_state" },
	{ 0x849d9170, "pci_enable_device" },
	{ 0x6d044c26, "param_ops_uint" },
	{ 0x2482e688, "vsprintf" },
	{ 0x5401d0b3, "dev_get_drvdata" },
	{ 0xa63ae769, "RDCCAN_register_port_device" },
	{ 0xbf820ecc, "pci_save_state" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=module_RDC_SocketCAN";

MODULE_ALIAS("pci:v000017F3d00001070sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000017F3d00001071sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "CDA861761326214B4F67B6F");

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
	{ 0xe68a66b1, "module_layout" },
	{ 0x55aa33da, "ethtool_op_get_link" },
	{ 0x1ae8143e, "eth_change_mtu" },
	{ 0x73700bbf, "eth_validate_addr" },
	{ 0xa2a11f97, "eth_mac_addr" },
	{ 0xa1d0f842, "pci_unregister_driver" },
	{ 0x8e415640, "__pci_register_driver" },
	{ 0x53b2d6b, "phy_start" },
	{ 0xc7eea848, "x86_dma_fallback_dev" },
	{ 0xbf5875bb, "dma_alloc_from_coherent" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0xd8a60c09, "dma_release_from_coherent" },
	{ 0x8ff4079b, "pv_irq_ops" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x16813ecb, "phy_stop" },
	{ 0xf9a482f9, "msleep" },
	{ 0xf1faac3a, "_raw_spin_lock_irq" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x9d0527d3, "skb_tstamp_tx" },
	{ 0x5c01dd7e, "netdev_err" },
	{ 0x4931d1f4, "napi_complete" },
	{ 0xb0b9168c, "mem_map" },
	{ 0x27e33546, "netif_receive_skb" },
	{ 0xffb4b95, "eth_type_trans" },
	{ 0x81d25be5, "skb_put" },
	{ 0x14f9abb0, "__netdev_alloc_skb" },
	{ 0xe2bf26a3, "consume_skb" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0x8a2c90c7, "__netif_schedule" },
	{ 0x76ebea8, "pv_lock_ops" },
	{ 0x3c2687e8, "dev_kfree_skb_irq" },
	{ 0xde290110, "dma_ops" },
	{ 0x67f7403e, "_raw_spin_lock" },
	{ 0x645391b3, "__napi_schedule" },
	{ 0x9ff0acb9, "pci_iounmap" },
	{ 0x11fdceb1, "register_netdev" },
	{ 0x7b0f253b, "_dev_info" },
	{ 0x59f1ff3b, "phy_connect" },
	{ 0xc164e378, "phy_find_first" },
	{ 0x6c02a053, "mdiobus_register" },
	{ 0x2a78b2ca, "kmem_cache_alloc_trace" },
	{ 0xe83ea961, "kmalloc_caches" },
	{ 0xb81960ca, "snprintf" },
	{ 0xf78c0983, "mdiobus_alloc" },
	{ 0x4fa0c244, "netif_napi_add" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0xd085185b, "pci_iomap" },
	{ 0xdbc3910c, "pci_request_regions" },
	{ 0x23e5b439, "alloc_etherdev_mqs" },
	{ 0x583f848d, "pci_set_master" },
	{ 0xe1b9ca9a, "dma_supported" },
	{ 0xdaa39903, "dev_err" },
	{ 0x7b82aac0, "dma_set_mask" },
	{ 0x573457f0, "pci_enable_device" },
	{ 0xfaf98462, "bitrev32" },
	{ 0xa34f1ef5, "crc32_le" },
	{ 0x7adf0c14, "phy_mii_ioctl" },
	{ 0xb35a2be2, "netdev_warn" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xc5534d64, "ioread16" },
	{ 0xf97456ea, "_raw_spin_unlock_irqrestore" },
	{ 0xf10de535, "ioread8" },
	{ 0x21fb443e, "_raw_spin_lock_irqsave" },
	{ 0x50eedeb8, "printk" },
	{ 0x8c183cbe, "iowrite16" },
	{ 0xc02c68a6, "phy_ethtool_gset" },
	{ 0x396031f6, "phy_ethtool_sset" },
	{ 0xe914e41e, "strcpy" },
	{ 0xa6ff1f2d, "dev_set_drvdata" },
	{ 0x69e37420, "pci_disable_device" },
	{ 0xfbe402b1, "free_netdev" },
	{ 0x5fe79def, "pci_release_regions" },
	{ 0x2dbfe655, "mdiobus_free" },
	{ 0x37a0cba, "kfree" },
	{ 0xc4e94054, "mdiobus_unregister" },
	{ 0xfc1d4124, "unregister_netdev" },
	{ 0xe169a838, "dev_get_drvdata" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v000017F3d00006040sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "5026A7B39D1612EDAB3D899");

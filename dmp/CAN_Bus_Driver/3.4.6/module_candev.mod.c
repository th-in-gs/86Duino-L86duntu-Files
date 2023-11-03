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
	{ 0xb727545c, "netif_carrier_on" },
	{ 0xdfe7ecb1, "skb_clone" },
	{ 0xbbf7eba4, "netif_carrier_off" },
	{ 0xfb0e29f, "init_timer_key" },
	{ 0x7d11c268, "jiffies" },
	{ 0x596e81f2, "__netdev_alloc_skb" },
	{ 0xe5bee209, "netif_rx" },
	{ 0xd5f2172f, "del_timer_sync" },
	{ 0x50eedeb8, "printk" },
	{ 0x61995295, "free_netdev" },
	{ 0xf609402b, "register_netdev" },
	{ 0xb4390f9a, "mcount" },
	{ 0x50c52926, "nla_put" },
	{ 0x8834396c, "mod_timer" },
	{ 0xf1da92d9, "rtnl_link_unregister" },
	{ 0x22d073af, "kfree_skb" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0xd1f9029b, "alloc_netdev_mqs" },
	{ 0x8e9af14b, "netdev_err" },
	{ 0x2143a445, "netdev_warn" },
	{ 0xab225e7f, "rtnl_link_register" },
	{ 0x12e14a58, "unregister_netdev" },
	{ 0x85670f1d, "rtnl_is_locked" },
	{ 0x76432a68, "skb_put" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "832098B8FC13EAB0A2CFDCC");

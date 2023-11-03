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
	{ 0xb3d7ada3, "kmem_cache_destroy" },
	{ 0x73eacd96, "can_rx_register" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0xdfe7ecb1, "skb_clone" },
	{ 0x776d03ae, "skb_copy" },
	{ 0x7cceb482, "__rtnl_register" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0x37312ae1, "skb_trim" },
	{ 0xf1db1704, "nla_memcpy" },
	{ 0x50eedeb8, "printk" },
	{ 0x5152e605, "memcmp" },
	{ 0xb4390f9a, "mcount" },
	{ 0x50c52926, "nla_put" },
	{ 0xd52ec05c, "kmem_cache_free" },
	{ 0xc990c2db, "dev_get_by_index" },
	{ 0x14c764cf, "init_net" },
	{ 0x262d6104, "can_send" },
	{ 0x6075bcd3, "kmem_cache_alloc" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x4f391d0e, "nla_parse" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0xdc245df7, "kmem_cache_create" },
	{ 0x37a0cba, "kfree" },
	{ 0x579e0bf5, "rtnl_unregister_all" },
	{ 0xdb5fcb2d, "can_rx_unregister" },
	{ 0x85670f1d, "rtnl_is_locked" },
	{ 0x3c72fa64, "__nlmsg_put" },
	{ 0x6e720ff2, "rtnl_unlock" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=module_can";


MODULE_INFO(srcversion, "DDCCE57B52EAF295A74F059");

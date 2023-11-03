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
	{ 0x402b8281, "__request_module" },
	{ 0x8d09836e, "sock_init_data" },
	{ 0x76ebea8, "pv_lock_ops" },
	{ 0x66dcc62b, "single_open" },
	{ 0x15692c87, "param_ops_int" },
	{ 0x91eb9b4, "round_jiffies" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0xa682332a, "single_release" },
	{ 0x96a592f6, "seq_puts" },
	{ 0xdfe7ecb1, "skb_clone" },
	{ 0x96c7e43b, "seq_printf" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0xe58e92fc, "remove_proc_entry" },
	{ 0xfb0e29f, "init_timer_key" },
	{ 0x2b0f0286, "mutex_unlock" },
	{ 0xde87e491, "seq_read" },
	{ 0x7d11c268, "jiffies" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0x7bec1789, "sock_get_timestamp" },
	{ 0xd5f2172f, "del_timer_sync" },
	{ 0x50ef7811, "skb_queue_purge" },
	{ 0x40649e25, "netif_rx_ni" },
	{ 0x78ee146, "proc_mkdir" },
	{ 0xa042edc6, "sk_alloc" },
	{ 0x50eedeb8, "printk" },
	{ 0x42c1edb, "proc_net_remove" },
	{ 0xb4390f9a, "mcount" },
	{ 0xb863e8df, "seq_putc" },
	{ 0xd52ec05c, "kmem_cache_free" },
	{ 0x38975d95, "mutex_lock" },
	{ 0xd0cb5712, "sk_free" },
	{ 0x4cd3059b, "dev_remove_pack" },
	{ 0xc2cdbf1, "synchronize_sched" },
	{ 0x8834396c, "mod_timer" },
	{ 0x14c764cf, "init_net" },
	{ 0x40c4a84a, "module_put" },
	{ 0xf8e53461, "proto_register" },
	{ 0x6075bcd3, "kmem_cache_alloc" },
	{ 0xbdb668fd, "sock_register" },
	{ 0x86a4889a, "kmalloc_order_trace" },
	{ 0xb0207ecf, "___ratelimit" },
	{ 0x22d073af, "kfree_skb" },
	{ 0x5aae3fbe, "proto_unregister" },
	{ 0x67f7403e, "_raw_spin_lock" },
	{ 0xdc245df7, "kmem_cache_create" },
	{ 0x5c3edd59, "_raw_write_unlock_bh" },
	{ 0x3b75ff2c, "proc_create_data" },
	{ 0x1e047854, "warn_slowpath_fmt" },
	{ 0xdb246df1, "seq_lseek" },
	{ 0x37a0cba, "kfree" },
	{ 0x50f5e532, "call_rcu_sched" },
	{ 0x62737e1d, "sock_unregister" },
	{ 0x32eeaded, "_raw_write_lock_bh" },
	{ 0xc3478c06, "dev_add_pack" },
	{ 0xfd4006e4, "consume_skb" },
	{ 0x41d8bc03, "dev_queue_xmit" },
	{ 0x2a21c6cd, "try_module_get" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7BC99676270A52956CF8507");

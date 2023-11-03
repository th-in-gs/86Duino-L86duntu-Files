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
	{ 0x9c24f263, "release_sock" },
	{ 0xa24d1581, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x73eacd96, "can_rx_register" },
	{ 0x66dcc62b, "single_open" },
	{ 0x22e85e41, "hrtimer_forward" },
	{ 0x9824b22e, "sock_no_setsockopt" },
	{ 0x3eb31449, "sock_no_getsockopt" },
	{ 0xa682332a, "single_release" },
	{ 0xb75d662a, "hrtimer_cancel" },
	{ 0x47939e0d, "__tasklet_hi_schedule" },
	{ 0x8a02435, "can_proto_unregister" },
	{ 0x96c7e43b, "seq_printf" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0xe93f356c, "sock_no_getname" },
	{ 0xc87c1f84, "ktime_get" },
	{ 0xe58e92fc, "remove_proc_entry" },
	{ 0xeb768b8a, "sock_queue_rcv_skb" },
	{ 0x714f78cc, "skb_recv_datagram" },
	{ 0xda01f41b, "sock_i_ino" },
	{ 0x91715312, "sprintf" },
	{ 0xde87e491, "seq_read" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0x74ccebec, "sock_no_sendpage" },
	{ 0xa414e013, "sock_no_mmap" },
	{ 0x2bc95bd4, "memset" },
	{ 0xec57caf2, "sock_no_socketpair" },
	{ 0x78ee146, "proc_mkdir" },
	{ 0x50eedeb8, "printk" },
	{ 0x2a433144, "sock_no_bind" },
	{ 0x93f884a7, "lock_sock_nested" },
	{ 0x42c1edb, "proc_net_remove" },
	{ 0xfaef0ed, "__tasklet_schedule" },
	{ 0xb4390f9a, "mcount" },
	{ 0xb863e8df, "seq_putc" },
	{ 0xb78e38bf, "sock_no_listen" },
	{ 0x5b533904, "sock_no_accept" },
	{ 0xaed072d0, "dev_get_by_index_rcu" },
	{ 0xd0cb5712, "sk_free" },
	{ 0xc990c2db, "dev_get_by_index" },
	{ 0x9545af6d, "tasklet_init" },
	{ 0x14c764cf, "init_net" },
	{ 0x99a374bc, "sock_no_shutdown" },
	{ 0x82072614, "tasklet_kill" },
	{ 0xa01d2a25, "can_ioctl" },
	{ 0x262d6104, "can_send" },
	{ 0x6677392b, "__sock_recv_ts_and_drops" },
	{ 0x9ceb163c, "memcpy_toiovec" },
	{ 0x710c5019, "__alloc_skb" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x11a4c038, "datagram_poll" },
	{ 0x22d073af, "kfree_skb" },
	{ 0x8750ccc5, "hrtimer_start" },
	{ 0x113c8a8f, "can_proto_register" },
	{ 0x864d3d1e, "kmem_cache_alloc_trace" },
	{ 0x5c3edd59, "_raw_write_unlock_bh" },
	{ 0x3b75ff2c, "proc_create_data" },
	{ 0xdb246df1, "seq_lseek" },
	{ 0x37a0cba, "kfree" },
	{ 0x2e60bace, "memcpy" },
	{ 0x9fb3dd30, "memcpy_fromiovec" },
	{ 0xea905975, "hrtimer_init" },
	{ 0x32eeaded, "_raw_write_lock_bh" },
	{ 0xdb5fcb2d, "can_rx_unregister" },
	{ 0x76432a68, "skb_put" },
	{ 0xe914e41e, "strcpy" },
	{ 0x48b03509, "skb_free_datagram" },
	{ 0x4cdb3178, "ns_to_timeval" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=module_can";


MODULE_INFO(srcversion, "A2E1808842F17A17C299EDF");

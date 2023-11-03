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
	{ 0x73eacd96, "can_rx_register" },
	{ 0x8a02435, "can_proto_unregister" },
	{ 0xdfe7ecb1, "skb_clone" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0xeb768b8a, "sock_queue_rcv_skb" },
	{ 0x6729d3df, "__get_user_4" },
	{ 0x714f78cc, "skb_recv_datagram" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0x74ccebec, "sock_no_sendpage" },
	{ 0xa414e013, "sock_no_mmap" },
	{ 0xec57caf2, "sock_no_socketpair" },
	{ 0x50eedeb8, "printk" },
	{ 0x93f884a7, "lock_sock_nested" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0xb78e38bf, "sock_no_listen" },
	{ 0x5b533904, "sock_no_accept" },
	{ 0xd0cb5712, "sk_free" },
	{ 0xc990c2db, "dev_get_by_index" },
	{ 0x14c764cf, "init_net" },
	{ 0x99a374bc, "sock_no_shutdown" },
	{ 0xa01d2a25, "can_ioctl" },
	{ 0x262d6104, "can_send" },
	{ 0x6677392b, "__sock_recv_ts_and_drops" },
	{ 0x9ceb163c, "memcpy_toiovec" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0x11a4c038, "datagram_poll" },
	{ 0x22d073af, "kfree_skb" },
	{ 0x113c8a8f, "can_proto_register" },
	{ 0x9178a482, "sock_alloc_send_skb" },
	{ 0xd61408a7, "sock_no_connect" },
	{ 0x5c3edd59, "_raw_write_unlock_bh" },
	{ 0x37a0cba, "kfree" },
	{ 0x9fb3dd30, "memcpy_fromiovec" },
	{ 0x32eeaded, "_raw_write_lock_bh" },
	{ 0xe2e8065e, "memdup_user" },
	{ 0xdb5fcb2d, "can_rx_unregister" },
	{ 0x7113d272, "sock_tx_timestamp" },
	{ 0x76432a68, "skb_put" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x48b03509, "skb_free_datagram" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=module_can";


MODULE_INFO(srcversion, "8AB2010312F06B926FD9273");

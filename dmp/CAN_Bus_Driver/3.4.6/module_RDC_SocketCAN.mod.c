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
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xf9a482f9, "msleep" },
	{ 0xd4c54f41, "register_candev" },
	{ 0x76ebea8, "pv_lock_ops" },
	{ 0x8d41e44e, "alloc_can_err_skb" },
	{ 0x47458af0, "napi_complete" },
	{ 0x2b0f0286, "mutex_unlock" },
	{ 0xaca24d6e, "alloc_candev" },
	{ 0x91715312, "sprintf" },
	{ 0x498c53bd, "can_bus_off" },
	{ 0x68dfc59f, "__init_waitqueue_head" },
	{ 0x2bc95bd4, "memset" },
	{ 0x9ff0b7ef, "close_candev" },
	{ 0xf10de535, "ioread8" },
	{ 0x6e51ac2e, "_raw_spin_trylock_bh" },
	{ 0xf97456ea, "_raw_spin_unlock_irqrestore" },
	{ 0x50eedeb8, "printk" },
	{ 0x5383f34b, "_raw_spin_trylock" },
	{ 0xfaef0ed, "__tasklet_schedule" },
	{ 0xb4390f9a, "mcount" },
	{ 0x212eb311, "netif_receive_skb" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x38975d95, "mutex_lock" },
	{ 0xdd1a2871, "down" },
	{ 0x9545af6d, "tasklet_init" },
	{ 0xbcdea25f, "netif_napi_add" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0x6fbb8fca, "free_candev" },
	{ 0x82072614, "tasklet_kill" },
	{ 0x8ff4079b, "pv_irq_ops" },
	{ 0xfe6ba013, "unregister_candev" },
	{ 0x382972, "alloc_can_skb" },
	{ 0x8bf826c, "_raw_spin_unlock_bh" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x4292364c, "schedule" },
	{ 0x727c4f3, "iowrite8" },
	{ 0x22d073af, "kfree_skb" },
	{ 0xf1faac3a, "_raw_spin_lock_irq" },
	{ 0xc41e8f7a, "wait_for_completion_interruptible_timeout" },
	{ 0x4f833ab5, "open_candev" },
	{ 0x67f7403e, "_raw_spin_lock" },
	{ 0x21fb443e, "_raw_spin_lock_irqsave" },
	{ 0x4f68e5c9, "do_gettimeofday" },
	{ 0x8c183cbe, "iowrite16" },
	{ 0x37a0cba, "kfree" },
	{ 0x2e60bace, "memcpy" },
	{ 0xc4554217, "up" },
	{ 0xc5534d64, "ioread16" },
	{ 0x19a9e62b, "complete" },
	{ 0xdd64801f, "can_get_echo_skb" },
	{ 0x6c07d0c6, "__netif_schedule" },
	{ 0xf1f08c59, "can_put_echo_skb" },
	{ 0x436c2179, "iowrite32" },
	{ 0xc79718c4, "can_free_echo_skb" },
	{ 0x2482e688, "vsprintf" },
	{ 0xe484e35f, "ioread32" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=module_candev";


MODULE_INFO(srcversion, "0F3FBC7770E3E9F101B684C");

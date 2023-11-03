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
	{ 0xa24d1581, "kmalloc_caches" },
	{ 0x15692c87, "param_ops_int" },
	{ 0x76b95be0, "framebuffer_release" },
	{ 0x7c309c51, "dev_set_drvdata" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xfbb2db5f, "fb_sys_read" },
	{ 0xd33fe978, "usb_kill_urb" },
	{ 0xfb0e29f, "init_timer_key" },
	{ 0x6baae653, "cancel_delayed_work_sync" },
	{ 0x2b0f0286, "mutex_unlock" },
	{ 0x999e8297, "vfree" },
	{ 0x8a8ebaa3, "sys_copyarea" },
	{ 0x61cb0aaf, "input_set_abs_params" },
	{ 0x18d0cfca, "input_event" },
	{ 0x68dfc59f, "__init_waitqueue_head" },
	{ 0xf97456ea, "_raw_spin_unlock_irqrestore" },
	{ 0x66e55691, "current_task" },
	{ 0xe238f878, "usb_deregister" },
	{ 0x31650c43, "__mutex_init" },
	{ 0x50eedeb8, "printk" },
	{ 0x7a890c8, "fb_alloc_cmap" },
	{ 0x4117a838, "register_framebuffer" },
	{ 0xb4390f9a, "mcount" },
	{ 0x38975d95, "mutex_lock" },
	{ 0xda00f745, "usb_free_coherent" },
	{ 0x98b71c6, "fb_dealloc_cmap" },
	{ 0x8db7edf0, "_dev_info" },
	{ 0xde5fd214, "usb_submit_urb" },
	{ 0x93e1c00, "usb_get_dev" },
	{ 0x2f977d70, "sys_fillrect" },
	{ 0x77edf722, "schedule_delayed_work" },
	{ 0xb824d6d3, "input_register_device" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x86c9dea7, "sys_imageblit" },
	{ 0xb04ccf4c, "usb_clear_halt" },
	{ 0x2d25222e, "fb_sys_write" },
	{ 0xa0b04675, "vmalloc_32" },
	{ 0x864d3d1e, "kmem_cache_alloc_trace" },
	{ 0x21fb443e, "_raw_spin_lock_irqsave" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0x783b180a, "framebuffer_alloc" },
	{ 0x92a866e7, "fb_deferred_io_cleanup" },
	{ 0x76c39f7e, "fb_deferred_io_init" },
	{ 0x37a0cba, "kfree" },
	{ 0x2e60bace, "memcpy" },
	{ 0x69348ba7, "input_unregister_device" },
	{ 0x622fa02a, "prepare_to_wait" },
	{ 0xe9e1c2a5, "usb_register_driver" },
	{ 0x75bb675a, "finish_wait" },
	{ 0xece39f34, "dev_warn" },
	{ 0xd61bcb70, "usb_alloc_coherent" },
	{ 0xc68f8a4f, "vmalloc_to_page" },
	{ 0x5401d0b3, "dev_get_drvdata" },
	{ 0x7bc03ed4, "usb_free_urb" },
	{ 0x5dfcfb98, "usb_alloc_urb" },
	{ 0xecfd1fa, "input_allocate_device" },
	{ 0xdf2ad0a5, "unregister_framebuffer" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=fb_sys_fops,syscopyarea,sysfillrect,sysimgblt";


MODULE_INFO(srcversion, "55F7DA8EAA3DB0D429AC04F");

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# module_can setting
module_can-objs := ./can/af_can.o ./can/proc.o

obj-m += module_can.o

# module_canraw setting
module_canraw-objs := ./can/raw.o

obj-m += module_canraw.o

# module_canbcm setting
module_canbcm-objs := ./can/bcm.o

obj-m += module_canbcm.o

# module_cangw setting
module_cangw-objs := ./can/gw.o

obj-m += module_cangw.o

# module_candev setting
module_candev-objs := candev.o

obj-m += module_candev.o

# module_RDC_CAN setting
module_RDC_SocketCAN-objs := RDC_CAN_Module.o SystemPorting.o BusDeviceIO.o \
    HwiCAN.o HwdCAN.o DrvDebug.o

obj-m += module_RDC_SocketCAN.o

# module_RDC_CAN_pic setting
module_RDC_CAN_pci-objs := RDC_CAN_17f3.o DrvDebug.o

obj-m += module_RDC_CAN_pci.o

# module_RDC_CAN_platform setting
module_RDC_CAN_platform-objs := RDC_CAN_platform.o DrvDebug.o

obj-m += module_RDC_CAN_platform.o

# module_RDC_CAN_platform_1070 setting
module_RDC_CAN_platform_1070-objs := RDC_CAN_platform_1070.o DrvDebug.o

obj-m += module_RDC_CAN_platform_1070.o

# module_RDC_CAN_platform_1071 setting
module_RDC_CAN_platform_1071-objs := RDC_CAN_platform_1071.o DrvDebug.o

obj-m += module_RDC_CAN_platform_1071.o

# module_RDC_CAN_pnp setting
module_RDC_CAN_pnp-objs := RDC_CAN_pnp.o DrvDebug.o

obj-m += module_RDC_CAN_pnp.o

all:
# use Tab
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	#
	modinfo module_candev.ko
	#
	modinfo module_RDC_SocketCAN.ko
	#
	modinfo module_RDC_CAN_pci.ko
	#
	#modinfo module_RDC_CAN_platform.ko
	#
	#modinfo module_RDC_CAN_platform_add.ko
	
	
	
clean:
# use Tab
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	

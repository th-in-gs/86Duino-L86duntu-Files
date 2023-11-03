#
# Makefile for the Linux network (ethercard) device drivers.
#

#PWD := /home/dmp/Documents/r6040
#KERNEL_DIR = /usr/src/linux-headers-3.2.0-23-generic
KERNEL_DIR = /lib/modules/`uname -r`/build

MODULE_NAME = r6040
obj-m := $(MODULE_NAME).o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean


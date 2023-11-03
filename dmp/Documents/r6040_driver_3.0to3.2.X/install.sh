#!/bin/sh

make
sudo rm /lib/modules/`uname -r`/kernel/drivers/net/ethernet/rdc/r6040.ko
sudo cp r6040.ko /lib/modules/`uname -r`/kernel/drivers/net/ethernet/rdc
sudo depmod
sudo leafpad /etc/rc.local
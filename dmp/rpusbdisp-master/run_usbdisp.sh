#!/bin/sh

modprobe rp_usbdisplay
sleep 1
modprobe -r rp_usbdisplay
modprobe rp_usbdisplay
sleep 1
modprobe -r rp_usbdisplay
modprobe rp_usbdisplay
cp /home/dmp/rpusbdisp-master/drivers/linux-driver/xserver_conf/10-disp.conf /usr/share/X11/xorg.conf.d/
lightdm

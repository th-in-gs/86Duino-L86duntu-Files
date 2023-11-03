#!/bin/sh

#modprobe rp_usbdisplay
#sleep 1
#modprobe -r rp_usbdisplay
modprobe rp_usbdisplay
sleep 1
cp drivers/linux-driver/xserver_conf/10-disp.conf /usr/share/X11/xorg.conf.d/
start lightdm

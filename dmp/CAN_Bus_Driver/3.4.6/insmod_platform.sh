#!/bin/sh
module="RDC_CAN_17f3"
device="RDCCAN"
mode="664"
maxports="32"

echo -e "Load can module and can protocol"
insmod module_can.ko
insmod module_canraw.ko

echo -e "Load candev module and CAN Controller driver"
insmod module_candev.ko
insmod module_RDC_SocketCAN.ko
insmod module_RDC_CAN_platform.ko
insmod module_RDC_CAN_platform_1071.ko mem=0xfebff800 irq=18

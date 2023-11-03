#!/bin/sh
module="RDC_CAN_17f3"
device="RDCCAN"
mode="664"
maxports="32"

echo -e "Unload candev module and CAN Controller driver"
rmmod module_RDC_CAN_platform_1071
rmmod module_RDC_CAN_platform
rmmod module_RDC_SocketCAN
rmmod module_candev

echo -e "Unload can module and can protocol"
rmmod module_canraw
rmmod module_can

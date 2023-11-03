#!/bin/sh
module="RDC_CAN_17f3"
device="RDCCAN"
mode="664"
maxports="32"

echo -e "Load can module and can protocol"
insmod /home/dmp/CAN_Bus_Driver/3.4.6/module_can.ko
insmod /home/dmp/CAN_Bus_Driver/3.4.6/module_canraw.ko

echo -e "Load candev module and CAN Controller driver"
insmod /home/dmp/CAN_Bus_Driver/3.4.6/module_candev.ko
insmod /home/dmp/CAN_Bus_Driver/3.4.6/module_RDC_SocketCAN.ko
insmod /home/dmp/CAN_Bus_Driver/3.4.6/module_RDC_CAN_pci.ko

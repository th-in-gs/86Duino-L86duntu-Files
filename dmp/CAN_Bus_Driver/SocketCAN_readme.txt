Introduction:
    This is a RDC CAN Controller driver package.
    RDC CAN module is based on SocketCAN device interface.
    
    The first verion is based on linux 3.5 and CAN Version (2.6.31)

    RDC_SocketCAN_Module is for DMP. this version is old driver architecture.
    DMP's 1070 CAN need External Clock - 20000000.

build & insmod:
    check your Kernel version and go to the package dir.
    typing "make clean | make all" to build them.
    typing "sh insmod_RDC_CAN_pnp.sh" to load SocketCAN module and RDC CAN PNP Device Module.
    typing "sh rmmod_RDC_CAN_pnp.sh" to remove SocketCAN module and RDC CAN PNP Device Module.
    typing "sh insmod_RDC_CAN_pci.sh" to load SocketCAN module and RDC CAN PCI Device Module.
    typing "sh rmmod_RDC_CAN_pci.sh" to remove SocketCAN module and RDC CAN PCI Device Module.
    typing "sh ipup_RDCCAN.sh" to strat CAN0 and CAN1.
    typing "sh ipdown_RDCCAN.sh" to shutdown CAN0 and CAN1.
    
    you can load different SocketCAN module and protocols through modifying insmod_RDC_CAN.sh.
    you can config different CAN parameter through modifying ipup_RDC_CAN.sh.
    
    module list:
        module_candev.ko - this is can-dev.ko.
        module_RDC_SocketCAN.ko - this is RDC CAN module.
        module_RDC_CAN_pci.ko - this is PCI CAN device driver. it include 1070.
        module_RDC_CAN_pnp.ko - this is PNP CAN device driver. it include PNP1070.

    test programs
        ./tst-raw-1 -i can1 -sm
        ./tst-raw-0 -i can0 -sm
        ctrl + c to stop
        // run tst-raw-1 first, then tst-raw-0
        
        // tst-raw-0 expect to recevie extend frame from tst-raw-1 and can_eid is in increasing order.
        // if the value of can_eid is not expected, tst-raw-0 will send a stop frame to tst-raw-1 and dump the following frame.
        // tst-raw-1 expect to recevie standard frame from tst-raw-0 and can_sid is in increasing order.
        // if the value of can_sid is not expected, tst-raw-1 will send a stop frame to tst-raw-0 and dump the following frame.

    show information:
        ip -details -statistics link show can0
        ip -details -statistics link show can1

        ipconfig
    
    Note:
        ip utility, iproute2-ss091009
            you can go to "RPM Serach" to search iproute2 to get a updated iproute2.

Our build enviroment:
    //2.6.18  --  Centos 5.9 (2.6.18)
    2.6.31  --  Fefora 12 (2.6.31)
    //2.6.32  --  Centos 6.4 (2.6.32)
    //2.6.37  --  openSUSE 11.4 (2.6.37)
    3.5.0   --  ubuntu 12.04.2 (3.5.0), fedora 18 (3.6.0), fedora 18 updated (3.8.6)
                openSUSE 12.3 (3.7.10) 


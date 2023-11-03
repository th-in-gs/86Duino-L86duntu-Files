#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>     // kzalloc

// return -ERRNO -EFAULT
// device -EBUSY, -ENODEV, -ENXIO
// parameter -EOPNOTSUPP

#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/wait.h>

#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/delay.h>

#include <linux/kthread.h>
//#include <linux/freezer.h>

#include <linux/netdevice.h>
#include <linux/skbuff.h>

#include <linux/can.h>
#include <linux/can/dev.h>

//#include <linux/pci.h>      // about PCI_DEVFN, PCI_SLOT, PCI_FUNC
//#include <linux/device.h>
//#include <linux/fs.h>       // struct inode need this header file.
//#include <linux/cdev.h>

#define    DMA_ADDR_INVALID    (~(dma_addr_t)0)
//#include <linux/dma-mapping.h>

// because we need the serial core

#include "SPtype.h"
#include "DrvDebug.h"   //#define DrvDbgPrint
#include "funcresult.h"
#include "CANDef.h"

#include "SystemPorting.h"
#include "BusDeviceIO.h"
#include "HwiCAN.h"
#include "HwdCAN.h"

#include "RDC_CAN_Module.h"

//----
// Driver Infomation
//

//netif_receive_skb invoked in interrupt context
//netif_rx_ni invoked in non-interrupt context

//#define KernelVersion      "3.5"        // linux version
#define DRIVER_NAME         "RDC_SocketCAN"    // RDC SocketCAN Driver
#define DRIVER_VERSION      "1.0.0"          //

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kevin.haung@rdc.com.tw");
MODULE_DESCRIPTION("RDC SocketCAN Network Device Driver");
//MODULE_INFO(depends, "");


//----
// define sharing interrupt list
// following reference examples, we use shared_irq_list to do a manual sharing interrupt handle.
// in our architecture, one pci device can only register one IRQHandler for all ports.
// if a pci device supports multiple ports, we need shared_irq_list to handle interrupt of all ports.
// PS: we may repeatedly register IRQHandler for each port. ??? need check
// 

struct shared_irq_list {
    spinlock_t          lock;
    unsigned int        referencecount;   // reference count
    struct list_head    head;
};

// struct list_head    shared_irq_entry;

static struct shared_irq_list ThisModule_shared_irq_lists[NR_IRQS];
    // the index is irq number(pcidev->irq).
    /*
    * NR_IRQS is the upper bound of how many interrupts can be handled
    * in the platform.
    */
    // in general, one pci device use one interrupt.
    // two pci device may use the same interrupt, or use the different interrupt.


//----
// define driver data for this module
//
#define MAX_MODULE_PORTS     32
                                // totoal number of ports for this module.
                                // each device may have more the one port.
                                // ex: four 8-ports pci card.

//#define ThisDeviceFile_MAJOR  30
//#define ThisDeviceFile_MINOR  0

static struct module_driver_data ThisModule_driver_data = {
    .owner              = THIS_MODULE,
    .driver_name        = "RDC_SocketCAN",
    .dev_name           = "RDCCAN",
    .numberdevicefile   = 0,
    .minor_start        = 0,
    
};

//--------
// define port config
//

//--------
// define device file ops for this module.
//

//--------
// define port device for this module
//

static DEFINE_MUTEX(ThisModule_port_devices_mutex);

static struct module_port_device ThisModule_port_devices[MAX_MODULE_PORTS];
    // static devices, and the index is minor number.


//--------
// define SocketCAN Device Driver config
//
    // SYNC + Prop + Seg1 + Seg2 list table
    // by CAN spec. total bit time = 8 ~ 25 Tq
    // SYNC + Prop + Seg1 + Seg2
    // 1    + 4    + 1    + 2 = 8
    // 1    + 3    + 2    + 2 = 8   => default setting
    // 1    + X    + 3    + 3 = 8~15
    // 1    + X    + 4    + 4 = 10~17
    // 1    + X    + 5    + 5 = 12~19
    // 1    + X    + 6    + 6 = 14~21
    // 1    + X    + 7    + 7 = 16~23
    // 1    + X    + 8    + 8 = 18~25
    // 1    + 8    + 8    + 8 = 25
static struct can_bittiming_const rdc_bittiming_const = {
    .name       = "RDC_CAN",
    .tseg1_min  = 5,    // see SYNC + Prop + Seg1 + Seg2 list table
    .tseg1_max  = 16,   /* Time segement 1 = prop_seg + phase_seg1 */
    .tseg2_min  = 3,    // see SYNC + Prop + Seg1 + Seg2 list table
    .tseg2_max  = 8,    /* Time segement 2 = phase_seg2 */
    .sjw_max    = 4,
    .brp_min    = 2,    // see our spec.
    .brp_max    = 0x20000,   /* Bit-rate prescaler */ // see our spec. (0xFFFF + 1)*2
    .brp_inc    = 2,    // see our spec.
};

static const struct net_device_ops rdc_can_netdev_ops = {
    .ndo_open        = rdc_candev_open,
    .ndo_stop        = rdc_candev_close,
    .ndo_start_xmit  = rdc_candev_start_xmit,
};

//----
// Module Block
//

int __init Module_init(void)
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
    
    unsigned int                i;
    
    //dev_t                       chardev_number;
    
    DrvDbgPrint(0, "Module_init: %s \n", DRIVER_NAME);
    
    DrvDbgPrint(0, "    sizeof(char):%lu sizeof(int):%lu sizeof(long):%lu \n", sizeof(char), sizeof(int), sizeof(long));
    DrvDbgPrint(0, "    sizeof(SHORT):%lu sizeof(LONG):%lu sizeof(INT):%lu \n", sizeof(SHORT), sizeof(LONG), sizeof(INT));
    DrvDbgPrint(0, "    sizeof(void*):%lu sizeof(UINTPTR):%lu \n", sizeof(void*), sizeof(UINTPTR));
    
    
    // initialize module level (global) data.
    
    retval = 0;
    
    for (i = 0; i < NR_IRQS; i++)
    {
        spin_lock_init(&ThisModule_shared_irq_lists[i].lock);
        ThisModule_shared_irq_lists[i].referencecount = 0;
        INIT_LIST_HEAD(&ThisModule_shared_irq_lists[i].head);
    }
    
    DrvDbgPrint(0, "    this module supprts %u ports \n", MAX_MODULE_PORTS);
    for (i = 0; i < MAX_MODULE_PORTS; i++)
    {
        ThisModule_port_devices[i].bUsed = FALSE;
        ThisModule_port_devices[i].pcidev = NULL;
        ThisModule_port_devices[i].minor = i;
        ThisModule_port_devices[i].candev = NULL;
    }
    
    
    
    return 0;
}

void __exit Module_exit(void)
{
    DrvDbgPrint(0, "Module_exit: %s \n", DRIVER_NAME);
    
}

module_init(Module_init);
module_exit(Module_exit);

void
_rdc_show_regs(
            PHwdCAN             hHwdCAN
    )
{
    ULONG              Value;
    
    Value = 0;
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiGlobalControlReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiGlobalControlReg:0x%x \n", Value);
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiClockPrescalerReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiClockPrescalerReg:0x%x \n", Value);
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiBusTimingReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiBusTimingReg:0x%x \n", Value);
    
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiGlobalStatusReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiGlobalStatusReg:0x%x \n", Value);
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiRequestReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiRequestReg:0x%x \n", Value);
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiTxBuffer0TransmitStatusReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiTxBuffer0TransmitStatusReg:0x%x \n", Value);
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiTxBuffer1TransmitStatusReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiTxBuffer1TransmitStatusReg:0x%x \n", Value);
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiTxBuffer2TransmitStatusReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiTxBuffer2TransmitStatusReg:0x%x \n", Value);
    
    HwdCAN_GetReg(
        hHwdCAN,
        CANRegID_HwiReceiveStatusReg,
        &Value
        );
    DrvDbgPrint(0, "CANRegID_HwiReceiveStatusReg:0x%x \n", Value);
    
}

int
RDCCAN_shared_irq_reference(
            int                 irq
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    struct shared_irq_list      *this_shared_irq_list;
    
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    if (is_real_interrupt(irq))
    {
        this_shared_irq_list = &ThisModule_shared_irq_lists[irq];
        
        spin_lock_irq(&this_shared_irq_list->lock);
        
        if (this_shared_irq_list->referencecount == 0)
        {
            this_shared_irq_list->referencecount = 1;
            
            spin_unlock_irq(&this_shared_irq_list->lock);
            
            // PS: request_irq is sleeping function. it cannot be call in irq lock.
            // register irq handler
            if (request_irq(irq, IRQHandler, IRQF_SHARED, DRIVER_NAME, this_shared_irq_list) != 0)
            {
                DrvDbgPrint(0, "    request interrupt %i failed\n", irq);
                retval = -EFAULT;
                goto funcexit;
            }
            else
            {
                DrvDbgPrint(0, "    request IRQ %i handler success \n", irq);
            }
        }
        else
        {
            this_shared_irq_list->referencecount += 1;
            
            spin_unlock_irq(&this_shared_irq_list->lock);
            
            DrvDbgPrint(0, "    this_shared_irq_list->referencecount += 1; \n");
        }
    }
    else
    {
        DrvDbgPrint(0, "    No IRQ.  Check hardware Setup!\n");
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return;
}


int
RDCCAN_shared_irq_dereference(
            int                 irq
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    struct shared_irq_list      *this_shared_irq_list;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    // release irq
    if (is_real_interrupt(irq))
    {
        this_shared_irq_list = &ThisModule_shared_irq_lists[irq];
        
        spin_lock_irq(&this_shared_irq_list->lock);
        
        this_shared_irq_list->referencecount -= 1;
        
        if (this_shared_irq_list->referencecount == 0)
        {
            // PS: free_irq is sleeping function. it cannot be call in irq lock.
            spin_unlock_irq(&this_shared_irq_list->lock);
            
            free_irq(irq, this_shared_irq_list);
            DrvDbgPrint(0, "    free_irq %i \n", irq);
        }
        else
        {
            spin_unlock_irq(&this_shared_irq_list->lock);
        }
        
        DrvDbgPrint(0, "    this_shared_irq_list->referencecount -= 1; \n");
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return;
}

int
RDCCAN_register_port_device(
            unsigned int                portid,
            unsigned int                iospace,
            unsigned int                iolength,
            unsigned int                ioaddress,
            void *                      memaddress,
            int                         irq,
            unsigned int                *pminor // module_port_device * this_port_device
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //unsigned int                result;
                                // check if (result != 0)
    
    unsigned int                minor;
                                
    struct module_port_device   *this_port_device;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_register_port_device Entry \n");
    
    *pminor = 0;
    
    mutex_lock(&ThisModule_port_devices_mutex);
    
    for (minor = 0; minor < MAX_MODULE_PORTS; minor++)
    {
        if (ThisModule_port_devices[minor].bUsed == FALSE)
        {
            // init port device
            this_port_device = &ThisModule_port_devices[minor];
            
            this_port_device->portid = portid;
            
            // record io address & irq
            this_port_device->iospace = iospace;
            this_port_device->iolength = iolength;
            if (iospace == 0)
            {   // memory space
                this_port_device->iospace = 0;
                this_port_device->ioaddress = 0;
                this_port_device->memaddress = memaddress;
            }
            else
            {   // io space
                this_port_device->iospace = 1;
                this_port_device->ioaddress = ioaddress;
                this_port_device->memaddress = 0;
            }
            
            this_port_device->irq = irq;
            
            DrvDbgPrint(0, "    this_port_device->iospace:%u \n", this_port_device->iospace);
            DrvDbgPrint(0, "    this_port_device->ioaddress:0x%x \n", this_port_device->ioaddress);
            DrvDbgPrint(0, "    this_port_device->memaddress:0x%p \n", this_port_device->memaddress);
            
            // no irq port may use a timer.
                // our device should have a irq, so we dont support a timer for no irq handle.
            //init_timer(&this_port_device->timer);
            //this_port_device->timer.function = serialxr_timeout;
            
            this_port_device->bUsed = TRUE;
            *pminor = minor;
            break;
        }
    }
    mutex_unlock(&ThisModule_port_devices_mutex);
    
    if (minor == MAX_MODULE_PORTS)
    {
        DrvDbgPrint(0, "    there is no non-used minor port\n");
        retval = -EFAULT;
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_register_port_device Exit \n");
    
    return retval;
    //return;
}


int
RDCCAN_initialize_port_device(
            unsigned int                minor,
            unsigned int                BusNumber,
            unsigned int                DeviceNumber,
            unsigned int                FunctionNumber
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    struct module_port_device   *this_port_device;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_initialize_port_device Entry \n");
    
    this_port_device = &ThisModule_port_devices[minor];
    
    this_port_device->candev = NULL;
    
    this_port_device->BusNumber = BusNumber;
    this_port_device->DeviceNumber = DeviceNumber;
    this_port_device->FunctionNumber = FunctionNumber;
    
    // init sCANGlobalControl
    this_port_device->sCANGlobalControl.bSilent = 0;
    this_port_device->sCANGlobalControl.bTxRxPinLoopback = 0;
    this_port_device->sCANGlobalControl.bTransmitWithNoACK = 0;
    this_port_device->sCANGlobalControl.bSelfReception = 0;
    
    // we use round robin mode, because net device transmit message with a round robin architecture.
    // if using buffer index mode, we should make sure the order of message transmission.
    this_port_device->sCANGlobalControl.TransmitPriorityMode = 0;       // 0: buffer index, 1: ID, 2: round robin
                                // if using buffer index Priority, we should let every 3 Tx buffers as a transmission cycle.
                                
                                                                        
    this_port_device->sCANGlobalControl.bArbitrationLostRetry = 1;
    this_port_device->sCANGlobalControl.bBusErrorRetry = 1;
    this_port_device->sCANGlobalControl.bPowerSaving = 0;
    this_port_device->sCANGlobalControl.bRxBusError = 0;
    
    this_port_device->TxBufferFlag[0] = 0;
    this_port_device->TxBufferFlag[1] = 0;
    this_port_device->TxBufferFlag[2] = 0;
    
    //spin_lock_init(&this_port_device->spinlock_candev);
    SPSpinLock_Initialize(&this_port_device->sSPSpinLock_candev);
    
    tasklet_init(&this_port_device->ISRDPCTasklet, ISRDPCRoutine, (unsigned long)minor);
    spin_lock_init(&this_port_device->spinlock_ISRDPCQueue);
    SPZeroMemory(&this_port_device->sInterruptExtension, sizeof(INTERRUPT_EXTENSION));
    
    SPEvent_Initialize(&this_port_device->sSPEventReceive);
    SPEvent_Initialize(&this_port_device->sSPEventTxBuffer0Transmit);
    SPEvent_Initialize(&this_port_device->sSPEventTxBuffer1Transmit);
    SPEvent_Initialize(&this_port_device->sSPEventTxBuffer2Transmit);
    
    spin_lock_init(&this_port_device->lock);
    this_port_device->OpenCount = 0;
    
    this_port_device->startup = port_startup;
    this_port_device->shutdown = port_shutdown;
    this_port_device->pm = port_pm;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_initialize_port_device Exit \n");
    
    return retval;
    //return;
}


int
RDCCAN_setup_port_device(
            unsigned int                minor
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    unsigned int                result;
                                // check if (result != 0)
    
    struct module_port_device   *this_port_device;
    struct shared_irq_list      *this_shared_irq_list;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_setup_port_device Entry minor:%u \n", minor);
    
    this_port_device = &ThisModule_port_devices[minor];
    
    // call startup callback
    result = this_port_device->startup(this_port_device);
    if (result != 0)
    {
        DrvDbgPrint(0, "    this_port_device->startup failed \n");
        
        mutex_lock(&ThisModule_port_devices_mutex);
        this_port_device->bUsed = FALSE;
        mutex_unlock(&ThisModule_port_devices_mutex);
        
        retval = -EFAULT;
    }
    else
    {
        // link shared_irq_entry to shared_irq_list
        {
            INIT_LIST_HEAD(&this_port_device->shared_irq_entry);
            
            if (is_real_interrupt(this_port_device->irq))
            {
                // link_irq_chain()
                this_shared_irq_list = &ThisModule_shared_irq_lists[this_port_device->irq];
                
                spin_lock_irq(&this_shared_irq_list->lock);
                
                list_add_tail(&this_port_device->shared_irq_entry, &this_shared_irq_list->head);
                
                spin_unlock_irq(&this_shared_irq_list->lock);
                
                DrvDbgPrint(0, "    link shared_irq_entry to shared_irq_list, device minor: %u \n", this_port_device->minor);
            }
        }
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_setup_port_device Exit \n");
    
    return retval;
    //return;
}


int
RDCCAN_shutdown_port_device(
            unsigned int                minor
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //unsigned int                result;
                                // check if (result != 0)
    
    struct module_port_device   *this_port_device;
    struct shared_irq_list      *this_shared_irq_list;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_shutdown_port_device Entry minor:%u \n", minor);
    
    this_port_device = &ThisModule_port_devices[minor];
    
    // unlink shared_irq_entry from shared_irq_list
    if (is_real_interrupt(this_port_device->irq))
    {
        // unlink_irq_chain()
        this_shared_irq_list = &ThisModule_shared_irq_lists[this_port_device->irq];
        
        spin_lock_irq(&this_shared_irq_list->lock);
        
        list_del(&this_port_device->shared_irq_entry);
        
        spin_unlock_irq(&this_shared_irq_list->lock);
    
        DrvDbgPrint(0, "    unlink shared_irq_entry from shared_irq_list, device minor: %u \n", this_port_device->minor);
    }
    
    // shutdown
    this_port_device->shutdown(this_port_device);
    
    tasklet_kill(&this_port_device->ISRDPCTasklet);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_shutdown_port_device Exit \n");
    
    return retval;
    //return;
}





int
RDCCAN_unregister_port_device(
            unsigned int                minor
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    struct module_port_device   *this_port_device;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_unregister_port_device Entry minor:%u \n", minor);
    
    this_port_device = &ThisModule_port_devices[minor];
    
    // unregister port device
    mutex_lock(&ThisModule_port_devices_mutex);
    
    this_port_device->bUsed = FALSE;
        
    mutex_unlock(&ThisModule_port_devices_mutex);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_unregister_port_device Exit \n");
    
    return retval;
    //return;
}


int
RDCCAN_register_candev(
            unsigned int                minor,
            struct device               *dev,   // &pcidev->dev
            int                         irq,
            unsigned int                sourceclock
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    unsigned int                result;
                                // check if (result != 0)
                                
    struct module_port_device   *this_port_device;
    
    struct net_device           *candev;
    struct rdc_can_priv         *rdccanpriv;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_register_candev Entry minor:%u \n", minor);
    
    this_port_device = &ThisModule_port_devices[minor];
    
    // register device file
    candev = NULL;
    candev = alloc_candev(sizeof(struct rdc_can_priv), RDC_echo_skb_max);
    if (candev == NULL)
    {
        DrvDbgPrint(0, "    alloc_candev return null \n");
        retval = -ENOMEM;
        goto funcexit;
    }
    
    // init candev
    candev->flags |= IFF_ECHO;
    candev->netdev_ops = &rdc_can_netdev_ops;
    SET_NETDEV_DEV(candev, dev);


    candev->irq = irq;
    
    rdccanpriv = netdev_priv(candev);
    this_port_device->candev = candev;
    this_port_device->rdccanpriv = rdccanpriv;
    
    // init can_priv
    DrvDbgPrint(0, "    sourceclock:%u \n", sourceclock);
    rdccanpriv->port_minor = minor;
    rdccanpriv->can.clock.freq = sourceclock;
    rdccanpriv->can.bittiming_const = &rdc_bittiming_const;
    rdccanpriv->can.ctrlmode_supported = CAN_CTRLMODE_LISTENONLY | CAN_CTRLMODE_LOOPBACK;// | CAN_CTRLMODE_3_SAMPLES;
    
    // rdccanpriv->can.can_bittiming, this variable will be setup when do_set_bittiming.
    rdccanpriv->can.do_set_bittiming = rdc_candev_set_bittiming;
    rdccanpriv->can.do_set_mode = rdc_candev_set_mode;
    rdccanpriv->can.do_get_state = rdc_candev_get_state;
    rdccanpriv->can.do_get_berr_counter = rdc_candev_get_berr_counter;
    
    netif_napi_add(candev, &rdccanpriv->napi, rdc_candev_napi_poll, 64); // upper  level rx packet size = 64
    
    result = register_candev(candev);
    if (result != 0)
    {
        DrvDbgPrint(0, "    register_candev failed \n");
        free_candev(candev);
        this_port_device->candev = NULL;
    }
    else
    {
        DrvDbgPrint(0, "    register_candev ok \n");
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_register_candev Exit \n");
    
    return retval;
    //return;
}

int
RDCCAN_unregister_candev(
            unsigned int                minor
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    struct module_port_device   *this_port_device;
    struct net_device           *candev;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_unregister_candev Entry minor:%u \n", minor);
    
    this_port_device = &ThisModule_port_devices[minor];
    
    candev = this_port_device->candev;
    
    if (candev != NULL)
    {
        unregister_candev(candev);
        
        free_candev(candev);
        
        this_port_device->candev = NULL;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_unregister_candev Exit \n");
    
    return retval;
    //return;
}

int
RDCCAN_suspend_candev(
            unsigned int                minor
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    unsigned int                i;
    
    struct module_port_device   *this_port_device;
    struct net_device           *candev;
    struct rdc_can_priv         *rdccanpriv;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_suspend_candev Entry minor:%u \n", minor);
    
    this_port_device = &ThisModule_port_devices[minor];
    
    candev = this_port_device->candev;
    
    rdccanpriv = netdev_priv(candev);
    
    if (this_port_device->OpenCount == 1 && candev != NULL)
    {
        // stop netif queue
        netif_stop_queue(candev);
    
        // disable napi
        napi_disable(&rdccanpriv->napi);
        
        rdc_candev_stop(candev);
        
        // wait for ISRDPC done ???
        
        // abort and wait for all transmission
        for (i = 0; i < RDC_Number_TxBuffer; i++)
        {
            if (this_port_device->TxBufferFlag[i] != 0)
            {
                //SPSpinLock_Acquire(&this_port_device->sSPSpinLock_candev);
                _rdc_candev_tx_aborted(candev, i);
                //SPSpinLock_Release(&this_port_device->sSPSpinLock_candev);
            }
        }
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_suspend_candev Exit \n");
    
    return retval;
    //return;
}


int
RDCCAN_resume_candev(
            unsigned int                minor
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    
    struct module_port_device   *this_port_device;
    struct net_device           *candev;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "RDCCAN_resume_candev Entry minor:%u \n", minor);
    
    this_port_device = &ThisModule_port_devices[minor];
    
    candev = this_port_device->candev;
    
    if (this_port_device->OpenCount == 1 && candev != NULL)
    {
        rdc_candev_start(candev);
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    DrvDbgPrint(0, "RDCCAN_resume_candev Exit \n");
    
    return retval;
    //return;
}


EXPORT_SYMBOL(RDCCAN_shared_irq_reference);
EXPORT_SYMBOL(RDCCAN_shared_irq_dereference);
EXPORT_SYMBOL(RDCCAN_register_port_device);
EXPORT_SYMBOL(RDCCAN_initialize_port_device);
EXPORT_SYMBOL(RDCCAN_setup_port_device);
EXPORT_SYMBOL(RDCCAN_shutdown_port_device);
EXPORT_SYMBOL(RDCCAN_unregister_port_device);
EXPORT_SYMBOL(RDCCAN_register_candev);
EXPORT_SYMBOL(RDCCAN_unregister_candev);
EXPORT_SYMBOL(RDCCAN_suspend_candev);
EXPORT_SYMBOL(RDCCAN_resume_candev);

irqreturn_t
IRQHandler(
            int                 irq,
            void *              devicedata
                // In general, devicedata is device context.
                // each pci device will register its IRQHandler.
                // OS handle interrupt by polling each chained IRQHandler.
                // If a interrupt occurs, OS will pass irq and devicedata to IRQHandler.
                // according the devicedata(device context), IRQHandler will know which pci device it handle this time.
                
                // In here, devicedata is this_shared_irq_list.
                // This module will register the individual irq used by all pci devices that this module supported.
                // If there are two devices and they IRQ are different, two IRQHandler will be registered.
                // If there are two devices and they IRQ are the same, one IRQHandler will be registered and be shared.
                // each pci device may be more than one port device, and they are link to shared_irq_list.
                // If a interrupt occurs, OS will pass irq and devicedata to IRQHandler.
                // according the devicedata(this_shared_irq_list), IRQHandler will know which irq it handled this time.
                // Because port devices shared this irq(port devices are linked to shared_irq_list),
                // IRQHandler can also handle all the port devices on shared_irq_list.
                // advantage: if port devices is need real time response,
                //            using shared_irq_list is more suitable than OS polling chained IRQHandler.
                
                // maybe, we can change this_shared_irq_list to device context.
                // that is each port device register will also its IRQHandler.
                // but why 8250.c use this shared irq list to handle interrupt?
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
    
    
    struct shared_irq_list      *this_shared_irq_list;
    struct module_port_device   *this_port_device;
    
    struct net_device           *candev;
    struct rdc_can_priv         *rdccanpriv;
    
    PINTERRUPT_EXTENSION        pInterruptExtension;
    
    ULONG                       IsInterruptServiced;
    ULONG                       IsDpcQueued;
    
    ULONG                       IndexStart;
    ULONG                       IndexEnd;
    ULONG                       bFull;
    
    ULONG                       bInterrupt;
    CANInterrupt                sCANInterrupt;
    CANVolatileInfo             sCANVolatileInfo;
    
    //
    // initialize local variables
    //
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    this_shared_irq_list = (struct shared_irq_list*) devicedata;
    
    retval = IRQ_NONE;
    
    spin_lock(&this_shared_irq_list->lock);
    
    if (list_empty(&this_shared_irq_list->head))
    {
        //DrvDbgPrint(0, "IRQHandler this_shared_irq_list emtpy \n");
    }
    else
    {
        list_for_each_entry(this_port_device, &this_shared_irq_list->head, shared_irq_entry)
        {
            IsInterruptServiced = FALSE;
            IsDpcQueued = FALSE;
            bInterrupt = FALSE;
            
            pInterruptExtension = &this_port_device->sInterruptExtension;
            
            if (this_port_device->OpenCount == 0)
            {
                HwdCAN_ISR(
                    this_port_device->hHwdCAN,
                    &bInterrupt,
                    &sCANInterrupt,
                    &sCANVolatileInfo
                    );
            }
            else
            /*{
                candev = this_port_device->candev;
                rdccanpriv = netdev_priv(candev);
                
                HwdCAN_CheckInterrupt(
                    this_port_device->hHwdCAN,
                    &bInterrupt
                    );
                if (bInterrupt == TRUE)
                {   
                    //HwdCAN_DisableInterrupt(this_port_device->hHwdCAN, &this_port_device->sHwdCANInterrupt); // mark for real case
                    
                    napi_schedule(&rdccanpriv->napi);
                    
                    retval = IRQ_HANDLED;
                }
            }*/
            {
                // save InterruptVolatileInformation into DpcQueue
                bInterrupt = FALSE;
                HwdCAN_ISR(
                    this_port_device->hHwdCAN,
                    &bInterrupt,
                    &sCANInterrupt,
                    &sCANVolatileInfo
                    );
                if (bInterrupt == FALSE)
                {
                    
                }
                else
                {
                    IsInterruptServiced = TRUE;
                    
                    // check if FIFO Full
                    IndexStart = pInterruptExtension->Interrupt_Start;
                    IndexEnd = pInterruptExtension->Interrupt_End;
                    bFull = FALSE;
                    if (((IndexEnd + 1)%InterruptQueue_MaxCount) == IndexStart)
                    {
                        // overrun
                        bFull = TRUE;
                    }
                    
                    if (bFull == FALSE)
                    {
                        pInterruptExtension->Interrupt_Overrun = FALSE;
                        
                        // put data into FIFO
                        //SPCopyMemory(&pInterruptExtension->losCANInterruptInfo[IndexEnd], &sCANInterruptInfo, sizeof(CANInterruptInfo));
                        pInterruptExtension->losInterrupt[IndexEnd] = sCANInterrupt;
                        pInterruptExtension->losVolatileInfo[IndexEnd] = sCANVolatileInfo;
                        
                        // update IndexEnd
                        pInterruptExtension->Interrupt_End = (IndexEnd + 1)%InterruptQueue_MaxCount;
                        pInterruptExtension->Interrupt_Count += 1;
                    }
                    else
                    {
                        // the program should run to here.
                        pInterruptExtension->Interrupt_Overrun = TRUE;
                        
                        DrvDbgPrint(0, "    Interrupt_Overrun \n");
                    }
                }
                
                if (IsInterruptServiced == TRUE)
                {
                    // schedule EvtInterruptDpc
                    tasklet_schedule(&this_port_device->ISRDPCTasklet);
                    
                    retval = IRQ_HANDLED;
                }
            }
        }
    }
    
    spin_unlock(&this_shared_irq_list->lock);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return;
}


int
rdc_candev_napi_poll(
            struct napi_struct *napi,
            int                 rxquota
    )
{
    struct module_port_device   *this_port_device;
    struct net_device           *candev;
    struct rdc_can_priv         *rdccanpriv;
    
    
    unsigned long               flags;
                                //irq flags use unsigned long in linux
    
    PINTERRUPT_EXTENSION        pInterruptExtension;
    
    ULONG                       IndexStart;
    ULONG                       IndexEnd;
    ULONG                       bFull;
    
    ULONG                       IsInterruptServiced;
    
    ULONG                       bInterrupt;
    CANInterrupt                sCANInterrupt[10];
    CANVolatileInfo             sCANVolatileInfo[10];
    ULONG                       InterruptCount;
    
    unsigned int                i;
    
    int                         rxpackets;
    
    //
    // initialize local variables
    //
    
    //DrvDbgPrint(DMT_Trace, "rdc_candev_napi_poll Entry \n");
    
    
    
    rxpackets = 0;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    candev = napi->dev;
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    IsInterruptServiced = FALSE;
    bInterrupt = FALSE;
    
    InterruptCount = 0;
    
    if (this_port_device->OpenCount == 0)
    {
        while(1)
        {
            HwdCAN_ISR(
                this_port_device->hHwdCAN,
                &bInterrupt,
                &sCANInterrupt[0],
                &sCANVolatileInfo[0]
                );
            if (bInterrupt == FALSE)
            {
                break;
            }
        }
    }
    else
    {
        InterruptCount = 0;
        while(1)
        {
            // save InterruptVolatileInformation into DpcQueue
            bInterrupt = FALSE;
            HwdCAN_ISR(
                this_port_device->hHwdCAN,
                &bInterrupt,
                &sCANInterrupt[InterruptCount],
                &sCANVolatileInfo[InterruptCount]
                );
            if (bInterrupt == FALSE)
            {
                break;
            }
            else
            {
                IsInterruptServiced = TRUE;
                
                InterruptCount += 1;
                
                if (InterruptCount == 10)
                {
                    break;
                }
            }
            
            if (IsInterruptServiced == TRUE)
            {
                
            }
        }
    }
    
    if (InterruptCount > 0)
    {
        pInterruptExtension = &this_port_device->sInterruptExtension;
        
        spin_lock_irqsave(&this_port_device->spinlock_ISRDPCQueue, flags);
        
        for (i = 0; i < InterruptCount; i++)
        {
            // check if FIFO Full
            IndexStart = pInterruptExtension->Interrupt_Start;
            IndexEnd = pInterruptExtension->Interrupt_End;
            bFull = FALSE;
            if (((IndexEnd + 1)%InterruptQueue_MaxCount) == IndexStart)
            {
                // overrun
                bFull = TRUE;
            }
            
            if (bFull == FALSE)
            {
                pInterruptExtension->Interrupt_Overrun = FALSE;
                
                // put data into FIFO
                //SPCopyMemory(&pInterruptExtension->losCANInterruptInfo[IndexEnd], &sCANInterruptInfo, sizeof(CANInterruptInfo));
                pInterruptExtension->losInterrupt[IndexEnd] = sCANInterrupt[i];
                pInterruptExtension->losVolatileInfo[IndexEnd] = sCANVolatileInfo[i];
                
                // update IndexEnd
                pInterruptExtension->Interrupt_End = (IndexEnd + 1)%InterruptQueue_MaxCount;
                pInterruptExtension->Interrupt_Count += 1;
            }
            else
            {
                // the program should run to here.
                pInterruptExtension->Interrupt_Overrun = TRUE;
                
                DrvDbgPrint(0, "    Interrupt_Overrun \n");
            }
        }
        
        spin_unlock_irqrestore(&this_port_device->spinlock_ISRDPCQueue, flags);
        
        tasklet_schedule(&this_port_device->ISRDPCTasklet);
    }
    
    //for (i = 0; i < InterruptCount; i++)
    {
        // if use QuickHandleInterrupt(), rdc_candev_napi_poll() consume more time.
        //QuickHandleInterrupt(this_port_device->minor, &sCANInterrupt[0], &sCANVolatileInfo[0]);
    }
    
    napi_complete(napi);
    
    //HwdCAN_EnableInterrupt(this_port_device->hHwdCAN, &this_port_device->sHwdCANInterrupt);
    
    //goto funcexit;
//funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //DrvDbgPrint(DMT_Trace, "rdc_candev_napi_poll Exit \n");
    
    //return ntstatus;
    return 0;
}


void
QuickHandleInterrupt(
            UINTPTR             minor,
            PCANInterrupt       pCANInterrupt,
            PCANVolatileInfo    pCANVolatileInfo
    )
{
    struct module_port_device   *this_port_device;
    struct net_device           *candev;
    struct rdc_can_priv         *rdccanpriv;
    //struct net_device_stats     *net_stats;
                                // net_device_stats refer to netdevice.h
    
    ULONG                       TxIndex;
    CANTransmitStatus           sCANTransmitStatus;
    
    enum can_state              canstate;
    
    //
    // initialize local variables
    //
    
    //DrvDbgPrint(DMT_Trace, "EvtInterruptDpc Entry \n");
    
    
    
    //hDevice = AssociatedObject;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    this_port_device = &ThisModule_port_devices[minor];
    
    candev = this_port_device->candev;
    
    rdccanpriv = netdev_priv(candev);
    
    //net_stats = &candev->stats;
    
    //while(bEmpty == FALSE)
    {
        //SPSpinLock_AcquireAtDpcLevel(&this_port_device->sSPSpinLock_candev, &flags);
        
        // handle InterruptVolatileInformation
        if (pCANInterrupt->Receive == TRUE)
        {
            if (pCANVolatileInfo->NumRxCANMessage == 1)
            {
                _rdc_candev_rx(candev, &pCANVolatileInfo->sCANRxBuffer);
            }
            
            if (pCANVolatileInfo->NumRxCANMessage == 2)
            {
                _rdc_candev_rx(candev, &pCANVolatileInfo->sCANRxBuffer);
                _rdc_candev_rx(candev, &pCANVolatileInfo->sCANRxBuffer2);
            }
            
            /*if (pCANVolatileInfo->NumRxCANMessage == 3)
            {
                _rdc_candev_rx(candev, &pCANVolatileInfo->sCANRxBuffer);
                _rdc_candev_rx(candev, &pCANVolatileInfo->sCANRxBuffer2);
                _rdc_candev_rx(candev, &pCANVolatileInfo->sCANRxBuffer3);
            }*/
            
            //SPEvent_Set(&this_port_device->sSPEventReceive);
        }
        if (pCANInterrupt->TxBuffer0Transmit == TRUE)
        {
            TxIndex = 0;
            HwdCAN_GetTransmitStatus(
                this_port_device->hHwdCAN,
                TxIndex,
                &sCANTransmitStatus
                );
            if (sCANTransmitStatus.RequestAborted)
            {   
                _rdc_candev_tx_aborted(candev, TxIndex);
            }
            else if (sCANTransmitStatus.TransmissionComplete)
            {
                _rdc_candev_tx(candev, TxIndex, this_port_device->TxBufferDLC[TxIndex]);
            }
            else
            {
                _rdc_candev_tx_error(candev, TxIndex, &sCANTransmitStatus);
            }
            
            this_port_device->TxBufferFlag[TxIndex] = 0;
            
            //SPEvent_Set(&this_port_device->sSPEventTxBuffer0Transmit);
            netif_wake_queue(candev);
        }
        if (pCANInterrupt->TxBuffer1Transmit == TRUE)
        {
            TxIndex = 1;
            HwdCAN_GetTransmitStatus(
                this_port_device->hHwdCAN,
                TxIndex,
                &sCANTransmitStatus
                );
            if (sCANTransmitStatus.RequestAborted)
            {
                _rdc_candev_tx_aborted(candev, TxIndex);
            }
            else if (sCANTransmitStatus.TransmissionComplete)
            {
                _rdc_candev_tx(candev, TxIndex, this_port_device->TxBufferDLC[TxIndex]);
            }
            else
            {
                _rdc_candev_tx_error(candev, TxIndex, &sCANTransmitStatus);
            }
            
            this_port_device->TxBufferFlag[TxIndex] = 0;
            
            //SPEvent_Set(&this_port_device->sSPEventTxBuffer1Transmit);
            netif_wake_queue(candev);
        }
        if (pCANInterrupt->TxBuffer2Transmit == TRUE)
        {
            TxIndex = 2;
            HwdCAN_GetTransmitStatus(
                this_port_device->hHwdCAN,
                TxIndex,
                &sCANTransmitStatus
                );
            if (sCANTransmitStatus.RequestAborted)
            {
                _rdc_candev_tx_aborted(candev, TxIndex);
            }
            else if (sCANTransmitStatus.TransmissionComplete)
            {
                _rdc_candev_tx(candev, TxIndex, this_port_device->TxBufferDLC[TxIndex]);
            }
            else
            {
                _rdc_candev_tx_error(candev, TxIndex, &sCANTransmitStatus);
            }
            
            this_port_device->TxBufferFlag[TxIndex] = 0;
            
            //SPEvent_Set(&this_port_device->sSPEventTxBuffer2Transmit);
            
            // in order to let Tx buffer transmit wiht round robin order, we take every 3 Tx buffers as a transmission cycle.
            netif_wake_queue(candev);
        }
        if (pCANInterrupt->ErrorWarning == TRUE)
        {   
            _rdc_candev_errorwarning(candev, pCANVolatileInfo->TxErrorCounter, pCANVolatileInfo->RxErrorCounter);
        }
        if (pCANInterrupt->ErrorPassive == TRUE)
        {   
            _rdc_candev_errorpassive(candev, pCANVolatileInfo->TxErrorCounter, pCANVolatileInfo->RxErrorCounter);
        }
        if (pCANInterrupt->BusOff == TRUE)
        {
            _rdc_candev_busoff(candev);
        }
        if (pCANInterrupt->ArbitrationLost == TRUE)
        {
            rdccanpriv->can.can_stats.arbitration_lost += 1;
        }
        if (pCANInterrupt->RxBusError == TRUE)
        {
            if (pCANVolatileInfo->BusErrorDirection == 1)
            {
                _rdc_candev_rx_error(candev, &pCANVolatileInfo->sCANReceiveStatus);
            }
        }
        if (pCANInterrupt->RxFIFOOverrun == TRUE)
        {
            _rdc_candev_rx_overrun(candev);
        }
        if (pCANInterrupt->PowerSavingExit == TRUE)
        {
        }
        
        //SPSpinLock_ReleaseFromDpcLevel(&this_port_device->sSPSpinLock_candev, &flags);
    }
    
    // finally, we update can_state.
    rdc_candev_get_state(candev, &canstate);
    rdccanpriv->can.state = canstate;
    
    
    //goto funcexit;
//funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //DrvDbgPrint(DMT_Trace, "EvtInterruptDpc Exit \n");
    
    //return ntstatus;
}


void
ISRDPCRoutine(
            UINTPTR             minor
    )
{
    struct module_port_device   *this_port_device;
    struct net_device           *candev;
    struct rdc_can_priv         *rdccanpriv;
    //struct net_device_stats     *net_stats;
                                // net_device_stats refer to netdevice.h
    
    PINTERRUPT_EXTENSION        pInterruptExtension;
    
    unsigned long               flags;
                                //irq flags use unsigned long in linux
    
    ULONG                       IndexStart;
    ULONG                       IndexEnd;
    ULONG                       bEmpty;
    
    CANInterrupt                sCANInterrupt;
    CANVolatileInfo             sCANVolatileInfo;
    
    ULONG                       TxIndex;
    CANTransmitStatus           sCANTransmitStatus;
    
    enum can_state              canstate;
    
    //
    // initialize local variables
    //
    
    //DrvDbgPrint(DMT_Trace, "EvtInterruptDpc Entry \n");
    
    
    
    //hDevice = AssociatedObject;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    this_port_device = &ThisModule_port_devices[minor];
    
    candev = this_port_device->candev;
    
    rdccanpriv = netdev_priv(candev);
    
    //net_stats = &candev->stats;
    
    pInterruptExtension = &this_port_device->sInterruptExtension;
    
    bEmpty = FALSE;
    
    while(bEmpty == FALSE)
    {
        //WdfInterruptAcquireLock(Interrupt);
        spin_lock_irqsave(&this_port_device->spinlock_ISRDPCQueue, flags);
        
        // get InterruptVolatileInformation from DpcQueue
        IndexStart = pInterruptExtension->Interrupt_Start;
        IndexEnd = pInterruptExtension->Interrupt_End;
        
        if (pInterruptExtension->Interrupt_Overrun == TRUE)
        {
            DrvDbgPrint(0, "    Interrupt_Overrun \n");
        }
        
        if (IndexStart == IndexEnd)
        {   // empty
            bEmpty = TRUE;
        }
        else
        {
            bEmpty = FALSE;
            
            // get data from FIFO
            //SPCopyMemory(&sCANInterruptInfo, &pInterruptExtension->losCANInterruptInfo[IndexStart], sizeof(CANInterruptInfo));
            sCANInterrupt = pInterruptExtension->losInterrupt[IndexStart];
            sCANVolatileInfo = pInterruptExtension->losVolatileInfo[IndexStart];
            
            // update IndexStart
            pInterruptExtension->Interrupt_Start = (IndexStart + 1)%InterruptQueue_MaxCount;
            pInterruptExtension->Interrupt_Count -= 1;
        }
        
        //WdfInterruptReleaseLock(Interrupt);
        spin_unlock_irqrestore(&this_port_device->spinlock_ISRDPCQueue, flags);
        
        if (bEmpty == TRUE)
        {
            break;
        }
        
        //SPSpinLock_AcquireAtDpcLevel(&this_port_device->sSPSpinLock_candev, &flags);
        
        // handle InterruptVolatileInformation
        /*HwdCAN_DCPForISR(
            this_port_device->hHwdCAN,
            &sCANInterrupt,
            &sCANVolatileInfo
            );*/
        if (sCANInterrupt.Receive == TRUE)
        {
            if (sCANVolatileInfo.NumRxCANMessage == 1)
            {
                _rdc_candev_rx(candev, &sCANVolatileInfo.sCANRxBuffer);
            }
            
            if (sCANVolatileInfo.NumRxCANMessage == 2)
            {
                _rdc_candev_rx(candev, &sCANVolatileInfo.sCANRxBuffer);
                _rdc_candev_rx(candev, &sCANVolatileInfo.sCANRxBuffer2);
            }
            
            /*if (sCANVolatileInfo.NumRxCANMessage == 3)
            {
                _rdc_candev_rx(candev, &sCANVolatileInfo.sCANRxBuffer);
                _rdc_candev_rx(candev, &sCANVolatileInfo.sCANRxBuffer2);
                _rdc_candev_rx(candev, &sCANVolatileInfo.sCANRxBuffer3);
            }*/
            
            //SPEvent_Set(&this_port_device->sSPEventReceive);
        }
        if (sCANInterrupt.TxBuffer0Transmit == TRUE)
        {
            TxIndex = 0;
            HwdCAN_GetTransmitStatus(
                this_port_device->hHwdCAN,
                TxIndex,
                &sCANTransmitStatus
                );
            if (sCANTransmitStatus.RequestAborted)
            {   
                _rdc_candev_tx_aborted(candev, TxIndex);
            }
            else if (sCANTransmitStatus.TransmissionComplete)
            {
                _rdc_candev_tx(candev, TxIndex, this_port_device->TxBufferDLC[TxIndex]);
            }
            else
            {
                _rdc_candev_tx_error(candev, TxIndex, &sCANTransmitStatus);
            }
            
            this_port_device->TxBufferFlag[TxIndex] = 0;
            
            //SPEvent_Set(&this_port_device->sSPEventTxBuffer0Transmit);
            
            netif_wake_queue(candev);
        }
        if (sCANInterrupt.TxBuffer1Transmit == TRUE)
        {
            TxIndex = 1;
            HwdCAN_GetTransmitStatus(
                this_port_device->hHwdCAN,
                TxIndex,
                &sCANTransmitStatus
                );
            if (sCANTransmitStatus.RequestAborted)
            {
                _rdc_candev_tx_aborted(candev, TxIndex);
            }
            else if (sCANTransmitStatus.TransmissionComplete)
            {
                _rdc_candev_tx(candev, TxIndex, this_port_device->TxBufferDLC[TxIndex]);
            }
            else
            {
                _rdc_candev_tx_error(candev, TxIndex, &sCANTransmitStatus);
            }
            
            this_port_device->TxBufferFlag[TxIndex] = 0;
            
            //SPEvent_Set(&this_port_device->sSPEventTxBuffer1Transmit);
            
            netif_wake_queue(candev);
        }
        if (sCANInterrupt.TxBuffer2Transmit == TRUE)
        {
            TxIndex = 2;
            HwdCAN_GetTransmitStatus(
                this_port_device->hHwdCAN,
                TxIndex,
                &sCANTransmitStatus
                );
            if (sCANTransmitStatus.RequestAborted)
            {
                _rdc_candev_tx_aborted(candev, TxIndex);
            }
            else if (sCANTransmitStatus.TransmissionComplete)
            {
                _rdc_candev_tx(candev, TxIndex, this_port_device->TxBufferDLC[TxIndex]);
            }
            else
            {
                _rdc_candev_tx_error(candev, TxIndex, &sCANTransmitStatus);
            }
            
            this_port_device->TxBufferFlag[TxIndex] = 0;
            
            //SPEvent_Set(&this_port_device->sSPEventTxBuffer2Transmit);
            
            // in order to let Tx buffer transmit wiht round robin order, we take every 3 Tx buffers as a transmission cycle.
            netif_wake_queue(candev);
        }
        if (sCANInterrupt.ErrorWarning == TRUE)
        {   
            _rdc_candev_errorwarning(candev, sCANVolatileInfo.TxErrorCounter, sCANVolatileInfo.RxErrorCounter);
        }
        if (sCANInterrupt.ErrorPassive == TRUE)
        {   
            _rdc_candev_errorpassive(candev, sCANVolatileInfo.TxErrorCounter, sCANVolatileInfo.RxErrorCounter);
        }
        if (sCANInterrupt.BusOff == TRUE)
        {
            _rdc_candev_busoff(candev);
        }
        if (sCANInterrupt.ArbitrationLost == TRUE)
        {
            rdccanpriv->can.can_stats.arbitration_lost += 1;
        }
        if (sCANInterrupt.RxBusError == TRUE)
        {
            if (sCANVolatileInfo.BusErrorDirection == 1)
            {
                _rdc_candev_rx_error(candev, &sCANVolatileInfo.sCANReceiveStatus);
            }
        }
        if (sCANInterrupt.RxFIFOOverrun == TRUE)
        {
            _rdc_candev_rx_overrun(candev);
        }
        if (sCANInterrupt.PowerSavingExit == TRUE)
        {
        }
        
        //SPSpinLock_ReleaseFromDpcLevel(&this_port_device->sSPSpinLock_candev, &flags);
    }
    
    // finally, we update can_state.
    rdc_candev_get_state(candev, &canstate);
    rdccanpriv->can.state = canstate;
    
    
    //goto funcexit;
//funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //DrvDbgPrint(DMT_Trace, "EvtInterruptDpc Exit \n");
    
    //return ntstatus;
}








int
port_startup(
            struct module_port_device   *this_port_device
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    ULONG                       Base0Space;
    PHYSICAL_ADDRESS            Base0Address;
    ULONG                       Base0Length;
    
    PHYSICAL_ADDRESS            Base1Address;
    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    //DrvDbgPrint(0, "port_startup Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    // init HwdCAN
    this_port_device->hHwdCAN = &this_port_device->sHwdCAN;
    
    result = HwdCAN_Initialize(
        this_port_device->hHwdCAN,
        1 //UseIOSpace
        );
    
    //EvtDevicePrepareHardware
    result = HwdCAN_OpenDevice(
        this_port_device->hHwdCAN,
        this_port_device->BusNumber,
        this_port_device->DeviceNumber,
        this_port_device->FunctionNumber,
        this_port_device->portid
        );
    
    if (this_port_device->iospace == 0)
    {
        Base0Space = this_port_device->iospace;
        Base0Address.QuadPart = (ULONGLONG)this_port_device->memaddress;
        Base0Length = this_port_device->iolength;
        
    }
    else
    {
        Base0Space = this_port_device->iospace;
        Base0Address.QuadPart = (ULONGLONG)this_port_device->ioaddress;
        Base0Length = this_port_device->iolength;
    }
    
    DrvDbgPrint(0, "    Base0Address.HighPart:0x%x, LowPart:0x%x \n", Base0Address.HighPart, Base0Address.LowPart);
    
    
    Base1Address.QuadPart = 0;
    result = HwdCAN_BaseAddress(
        this_port_device->hHwdCAN,
        Base0Space,
        Base0Address,
        Base0Length,
        0,
        Base1Address,
        0
        );
    if (!FR_SUCCESS(result))
    {
        DrvDbgPrint(0, "    HwdCAN_BaseAddress Error:0x%X \n", result);
        retval = EFAULT;
        goto funcexit;
    }
    
    result = HwdCAN_InitHW(
        this_port_device->hHwdCAN
        );
    if (!FR_SUCCESS(result))
    {
        DrvDbgPrint(0, "    HwdCAN_SetupHC Error:0x%X \n", result);
        retval = EFAULT;
        goto funcexit;
    }
    
    // EvtDeviceD0Entry
    //result = HwdCAN_StartCAN(
        //this_port_device->hHwdCAN
        //);
    
    //EvtDeviceD0EntryPostInterruptsEnabled
    
    // init controller interrupt
    this_port_device->sHwdCANInterrupt.Receive = TRUE;
    this_port_device->sHwdCANInterrupt.TxBuffer0Transmit = TRUE;
    this_port_device->sHwdCANInterrupt.TxBuffer1Transmit = TRUE;
    this_port_device->sHwdCANInterrupt.TxBuffer2Transmit = TRUE;
    this_port_device->sHwdCANInterrupt.ErrorWarning = TRUE;
    this_port_device->sHwdCANInterrupt.ErrorPassive = TRUE;
    this_port_device->sHwdCANInterrupt.RxFIFOOverrun = TRUE;
    this_port_device->sHwdCANInterrupt.BusOff = TRUE;
    this_port_device->sHwdCANInterrupt.PowerSavingExit = FALSE;
    this_port_device->sHwdCANInterrupt.ArbitrationLost = TRUE;
    this_port_device->sHwdCANInterrupt.RxBusError = TRUE;
    
    result = HwdCAN_EnableInterrupt(
        this_port_device->hHwdCAN,
        &this_port_device->sHwdCANInterrupt
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    if (retval != 0)    // error occured
    {
    }
    
    return retval;
}


void
port_shutdown(
            struct module_port_device   *this_port_device
    )
{
    //int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    //
    // initialize local variables
    //
    
    //retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    //DrvDbgPrint(0, "port_shutdown Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    //EvtDeviceD0ExitPreInterruptsDisabled
    result = HwdCAN_DisableInterrupt(
        this_port_device->hHwdCAN,
        &this_port_device->sHwdCANInterrupt
        );
    
    //EvtDeviceD0Exit
    //result = HwdCAN_StopCAN(
        //this_port_device->hHwdCAN
        //);
    
    //EvtDeviceReleaseHardware
    result = HwdCAN_CloseDevice(
        this_port_device->hHwdCAN
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return retval;
    return ;
}


void
port_pm(
            struct module_port_device   *this_port_device,
            unsigned int                state,
            unsigned int                oldstate
    )
{
    //int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
                                
    //
    // initialize local variables
    //
    
    //retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    DrvDbgPrint(0, "port_pm Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return retval;
    return ;
}

int
_rdc_candev_tx_aborted(
            struct  net_device          *candev,
            ULONG                       TxIndex
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_err;
    struct can_frame            *canframe_err;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_err = NULL;
    canframe_err = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    can_free_echo_skb(candev, TxIndex);
    
    net_stats->tx_dropped += 1;
    net_stats->tx_aborted_errors += 1;
    
    // SocketCAN Error Frame
    skb_err = alloc_can_err_skb(candev, &canframe_err);
    if (skb_err == NULL)
    {
        goto funcexit;
    }
    else
    {
        // reference can/error.h
        
        // we take RequestAborted as a tx timeout.
        canframe_err->can_id |= CAN_ERR_TX_TIMEOUT;
        
        netif_receive_skb(skb_err);
        
        //error can frame is not a real can frame.
        //net_stats->rx_bytes += canframe_err->can_dlc;
        //net_stats->rx_packets += 1;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}

int
_rdc_candev_tx_error(
            struct  net_device          *candev,
            ULONG                       TxIndex,
            PCANTransmitStatus          pCANTransmitStatus
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_err;
    struct can_frame            *canframe_err;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_err = NULL;
    canframe_err = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    can_free_echo_skb(candev, TxIndex);
    
    net_stats->tx_errors += 1;
    
    if (pCANTransmitStatus->BusOff)
    {
        //rdccanpriv->can.can_stats.bus_off += 1; // we increase it in Interrupt.BusOff.
    }
    else
    {
        if (pCANTransmitStatus->ArbitrationLost)
        {
            //rdccanpriv->can.can_stats.arbitration_lost += 1; // we increase it in Interrupt.ArbitrationLost.
        }
        else if (pCANTransmitStatus->BusError)
        {
            rdccanpriv->can.can_stats.bus_error += 1;
        }
    }
    
    // SocketCAN Error Frame
    skb_err = alloc_can_err_skb(candev, &canframe_err);
    if (skb_err == NULL)
    {
        goto funcexit;
    }
    else
    {
        // reference can/error.h
        
        if (pCANTransmitStatus->BusOff)
        {
            // when hw transmit a tx buffer at bus off, we take this error state as timeout.
            canframe_err->can_id |= CAN_ERR_TX_TIMEOUT | CAN_ERR_PROT;
            
            canframe_err->data[2] |= CAN_ERR_PROT_TX;
        }
        else
        {
            if (pCANTransmitStatus->ArbitrationLost)
            {
                canframe_err->can_id |= CAN_ERR_LOSTARB | CAN_ERR_PROT;
            
                canframe_err->data[0] = pCANTransmitStatus->ArbitrationLostLocation + 1;
                
                canframe_err->data[2] |= CAN_ERR_PROT_TX;
            }
            else if (pCANTransmitStatus->BusError)
            {
                canframe_err->can_id |= CAN_ERR_BUSERROR | CAN_ERR_PROT;
                
                canframe_err->data[2] |= CAN_ERR_PROT_TX;
                
                // BusErrorType & BusErrorLocation refer to rdc can spec.
                
                if (pCANTransmitStatus->BusErrorType == 5)  // ACK error
                {
                    canframe_err->can_id |= CAN_ERR_ACK;
                }
                
                if (pCANTransmitStatus->BusErrorType == 1)  // Bit error
                {
                    canframe_err->data[2] |= CAN_ERR_PROT_BIT;
                }
                
                if (pCANTransmitStatus->BusErrorType == 2)  // Stuff error
                {
                    canframe_err->data[2] |= CAN_ERR_PROT_STUFF;
                }
                
                if (pCANTransmitStatus->BusErrorType == 3)  // CRC error
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_CRC_SEQ;
                }
                
                if (pCANTransmitStatus->BusErrorType == 4)  // Form error
                {
                    canframe_err->data[2] |= CAN_ERR_PROT_FORM;
                }
                
                if (pCANTransmitStatus->BusErrorLocation == 0)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_SOF;
                }
                if (pCANTransmitStatus->BusErrorLocation >= 1 && pCANTransmitStatus->BusErrorLocation <= 3)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_ID28_21;
                }
                if (pCANTransmitStatus->BusErrorLocation == 4)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_ID20_18;
                }
                if (pCANTransmitStatus->BusErrorLocation == 5)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_SRTR;
                }
                if (pCANTransmitStatus->BusErrorLocation == 6)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_IDE;
                }
                if (pCANTransmitStatus->BusErrorLocation >= 7 && pCANTransmitStatus->BusErrorLocation <= 8)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_ID17_13;
                }
                if (pCANTransmitStatus->BusErrorLocation >= 9 && pCANTransmitStatus->BusErrorLocation <= 10)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_ID12_05;
                }
                if (pCANTransmitStatus->BusErrorLocation >= 11 && pCANTransmitStatus->BusErrorLocation <= 12)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_ID04_00;
                }
                if (pCANTransmitStatus->BusErrorLocation == 13)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_RTR;
                }
                if (pCANTransmitStatus->BusErrorLocation == 14)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_RES1;
                }
                if (pCANTransmitStatus->BusErrorLocation == 15)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_RES0;
                }
                if (pCANTransmitStatus->BusErrorLocation == 16)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_DLC;
                }
                if (pCANTransmitStatus->BusErrorLocation == 17)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_DATA;
                }
                if (pCANTransmitStatus->BusErrorLocation == 18)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_CRC_SEQ;
                }
                if (pCANTransmitStatus->BusErrorLocation == 19)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_CRC_DEL;
                }
                if (pCANTransmitStatus->BusErrorLocation == 20)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_ACK;
                }
                if (pCANTransmitStatus->BusErrorLocation == 21)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_ACK_DEL;
                }
                if (pCANTransmitStatus->BusErrorLocation == 22)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_EOF;
                }
                if (pCANTransmitStatus->BusErrorLocation == 23)
                {
                    canframe_err->data[3] = CAN_ERR_PROT_LOC_INTERM;
                }
            }
        }
        
        netif_receive_skb(skb_err);
        
        //error can frame is not a real can frame.
        //net_stats->rx_bytes += canframe_err->can_dlc;
        //net_stats->rx_packets += 1;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}

int
_rdc_candev_tx(
            struct  net_device          *candev,
            ULONG                       TxIndex,
            ULONG                       DataLengthCode
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    can_get_echo_skb(candev, TxIndex);
    
    net_stats->tx_bytes += DataLengthCode;
    net_stats->tx_packets += 1;
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
_rdc_candev_rx_error(
            struct  net_device          *candev,
            PCANReceiveStatus           pCANReceiveStatus
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_err;
    struct can_frame            *canframe_err;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_err = NULL;
    canframe_err = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    rdccanpriv->can.can_stats.bus_error += 1;
    
    // SocketCAN Error Frame
    skb_err = alloc_can_err_skb(candev, &canframe_err);
    if (skb_err == NULL)
    {
        goto funcexit;
    }
    else
    {
        // reference can/error.h
        
        if (pCANReceiveStatus->BusError)
        {
            canframe_err->can_id |= CAN_ERR_BUSERROR | CAN_ERR_PROT;
            
            // BusErrorType & BusErrorLocation refer to rdc can spec.
            
            if (pCANReceiveStatus->BusErrorType == 5)  // ACK error
            {
                //canframe_err->can_id |= CAN_ERR_ACK;
                
                canframe_err->data[2] |= CAN_ERR_PROT_BIT;
            }
            
            if (pCANReceiveStatus->BusErrorType == 1)  // Bit error
            {
                canframe_err->data[2] |= CAN_ERR_PROT_BIT;
            }
            
            if (pCANReceiveStatus->BusErrorType == 2)  // Stuff error
            {
                canframe_err->data[2] |= CAN_ERR_PROT_STUFF;
            }
            
            if (pCANReceiveStatus->BusErrorType == 3)  // CRC error
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_CRC_SEQ;
            }
            
            if (pCANReceiveStatus->BusErrorType == 4)  // Form error
            {
                canframe_err->data[2] |= CAN_ERR_PROT_FORM;
            }
            
            if (pCANReceiveStatus->BusErrorLocation == 0)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_SOF;
            }
            if (pCANReceiveStatus->BusErrorLocation >= 1 && pCANReceiveStatus->BusErrorLocation <= 3)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_ID28_21;
            }
            if (pCANReceiveStatus->BusErrorLocation == 4)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_ID20_18;
            }
            if (pCANReceiveStatus->BusErrorLocation == 5)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_SRTR;
            }
            if (pCANReceiveStatus->BusErrorLocation == 6)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_IDE;
            }
            if (pCANReceiveStatus->BusErrorLocation >= 7 && pCANReceiveStatus->BusErrorLocation <= 8)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_ID17_13;
            }
            if (pCANReceiveStatus->BusErrorLocation >= 9 && pCANReceiveStatus->BusErrorLocation <= 10)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_ID12_05;
            }
            if (pCANReceiveStatus->BusErrorLocation >= 11 && pCANReceiveStatus->BusErrorLocation <= 12)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_ID04_00;
            }
            if (pCANReceiveStatus->BusErrorLocation == 13)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_RTR;
            }
            if (pCANReceiveStatus->BusErrorLocation == 14)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_RES1;
            }
            if (pCANReceiveStatus->BusErrorLocation == 15)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_RES0;
            }
            if (pCANReceiveStatus->BusErrorLocation == 16)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_DLC;
            }
            if (pCANReceiveStatus->BusErrorLocation == 17)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_DATA;
            }
            if (pCANReceiveStatus->BusErrorLocation == 18)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_CRC_SEQ;
            }
            if (pCANReceiveStatus->BusErrorLocation == 19)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_CRC_DEL;
            }
            if (pCANReceiveStatus->BusErrorLocation == 20)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_ACK;
            }
            if (pCANReceiveStatus->BusErrorLocation == 21)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_ACK_DEL;
            }
            if (pCANReceiveStatus->BusErrorLocation == 22)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_EOF;
            }
            if (pCANReceiveStatus->BusErrorLocation == 23)
            {
                canframe_err->data[3] = CAN_ERR_PROT_LOC_INTERM;
            }
        }
        
        netif_receive_skb(skb_err);
        
        //error can frame is not a real can frame.
        //net_stats->rx_bytes += canframe_err->can_dlc;
        //net_stats->rx_packets += 1;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
}


int
_rdc_candev_rx_overrun(
            struct  net_device          *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_err;
    struct can_frame            *canframe_err;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_err = NULL;
    canframe_err = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    net_stats->rx_over_errors += 1;
    net_stats->rx_errors += 1;
        
    // SocketCAN Error Frame
    skb_err = alloc_can_err_skb(candev, &canframe_err);
    if (skb_err == NULL)
    {
        goto funcexit;
    }
    else
    {
        
        // reference can/error.h
        
        canframe_err->can_id |= CAN_ERR_CRTL;
        canframe_err->data[1] = CAN_ERR_CRTL_RX_OVERFLOW;
        
        netif_receive_skb(skb_err);
    
        //error can frame is not a real can frame.
        //net_stats->rx_bytes += canframe_err->can_dlc;
        //net_stats->rx_packets += 1;
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
}
    


int
_rdc_candev_rx(
            struct  net_device          *candev,
            PCANRxBuffer                pCANRxBuffer
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    //unsigned int                i;
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_rx;
    struct can_frame            *canframe_rx;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_rx = NULL;
    canframe_rx = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    skb_rx = alloc_can_skb(candev, &canframe_rx);
    if (skb_rx == NULL)
    {
        net_stats->rx_dropped += 1;
        goto funcexit;
    }
    else
    {
        if (pCANRxBuffer->bExtendedFrame == 1)
        {
            canframe_rx->can_id = CAN_EFF_FLAG;
            canframe_rx->can_id |= pCANRxBuffer->ID & CAN_EFF_MASK;
        }
        else
        {
            canframe_rx->can_id = 0; // CAN_SFF_FLAG;
            canframe_rx->can_id |= pCANRxBuffer->ID & CAN_SFF_MASK;
        }
        
        if (pCANRxBuffer->RTRBit == 1)
        {
            canframe_rx->can_id |= CAN_RTR_FLAG;
        }
        
        canframe_rx->can_dlc = pCANRxBuffer->DataLengthCode;
        
        *(PULONG)&canframe_rx->data[0] = pCANRxBuffer->Byte0_3;
        *(PULONG)&canframe_rx->data[4] = pCANRxBuffer->Byte4_7;
        
        netif_receive_skb(skb_rx);
        //netif_rx_ni(skb_rx);
        
        net_stats->rx_bytes += canframe_rx->can_dlc;
        net_stats->rx_packets += 1;
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
_rdc_candev_errorwarning(
            struct  net_device          *candev,
            ULONG                       TxErrorCounter,
            ULONG                       RxErrorCounter
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_err;
    struct can_frame            *canframe_err;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_err = NULL;
    canframe_err = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    rdccanpriv->can.can_stats.error_warning += 1;
    
    // SocketCAN Error Frame
    skb_err = alloc_can_err_skb(candev, &canframe_err);
    if (skb_err == NULL)
    {
        goto funcexit;
    }
    else
    {
        
        // reference can/error.h
        
        canframe_err->can_id |= CAN_ERR_CRTL;
        
        if (TxErrorCounter > 96)
        {
            canframe_err->data[1] |= CAN_ERR_CRTL_TX_WARNING;
        }
        
        if (RxErrorCounter > 96)
        {
            canframe_err->data[1] |= CAN_ERR_CRTL_RX_WARNING;
        }
        
        netif_receive_skb(skb_err);
    
        //error can frame is not a real can frame.
        //net_stats->rx_bytes += canframe_err->can_dlc;
        //net_stats->rx_packets += 1;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
}


int
_rdc_candev_errorpassive(
            struct  net_device          *candev,
            ULONG                       TxErrorCounter,
            ULONG                       RxErrorCounter
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_err;
    struct can_frame            *canframe_err;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_err = NULL;
    canframe_err = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Handle
    rdccanpriv->can.can_stats.error_passive += 1;
    
    // SocketCAN Error Frame
    skb_err = alloc_can_err_skb(candev, &canframe_err);
    if (skb_err == NULL)
    {
        goto funcexit;
    }
    else
    {
        
        // reference can/error.h
        
        canframe_err->can_id |= CAN_ERR_CRTL;
        
        if (TxErrorCounter > 127)
        {
            canframe_err->data[1] |= CAN_ERR_CRTL_TX_PASSIVE;
        }
        
        if (RxErrorCounter > 127)
        {
            canframe_err->data[1] |= CAN_ERR_CRTL_RX_PASSIVE;
        }
        
        netif_receive_skb(skb_err);
    
        //error can frame is not a real can frame.
        //net_stats->rx_bytes += canframe_err->can_dlc;
        //net_stats->rx_packets += 1;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
}


int
_rdc_candev_busoff(
            struct  net_device          *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct sk_buff              *skb_err;
    struct can_frame            *canframe_err;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    skb_err = NULL;
    canframe_err = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    net_stats = &candev->stats;
    
    // SocketCAN Error Frame
    skb_err = alloc_can_err_skb(candev, &canframe_err);
    if (skb_err == NULL)
    {
        goto funcexit;
    }
    else
    {   
        // reference can/error.h
        
        canframe_err->can_id |= CAN_ERR_BUSOFF;
        
        netif_receive_skb(skb_err);
    
        //error can frame is not a real can frame.
        //net_stats->rx_bytes += canframe_err->can_dlc;
        //net_stats->rx_packets += 1;
    }
    
    // SocketCAN Handle
    can_bus_off(candev);
                // can_bus_off do:
                // netif_carrier_off();
                // can_stats.bus_off += 1;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
}





int
rdc_candev_start(
            struct  net_device          *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    DrvDbgPrint(0, "rdc_candev_start Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    // reset CAN & CANIF
    //if (priv->can.state != CAN_STATE_STOPPED)
    HwdCAN_ResetCAN(this_port_device->hHwdCAN);
        
    HwdCAN_ResetCANIF(this_port_device->hHwdCAN);
        
    // re-enable interrupt after reset can
    result = HwdCAN_EnableInterrupt(
        this_port_device->hHwdCAN,
        &this_port_device->sHwdCANInterrupt
        );
    
    // set GlobalControl according to current control mode
    if (rdccanpriv->can.ctrlmode & CAN_CTRLMODE_LISTENONLY)
    {
        this_port_device->sCANGlobalControl.bSilent = 1;
    }
    else
    {
        this_port_device->sCANGlobalControl.bSilent = 0;
    }
    
    if (rdccanpriv->can.ctrlmode & CAN_CTRLMODE_LOOPBACK)
    {
        //this_port_device->sCANGlobalControl.bTxRxPinLoopback = 1; for Phy test
        this_port_device->sCANGlobalControl.bSelfReception = 1;
        this_port_device->sCANGlobalControl.bTransmitWithNoACK = 1;
    }
    else
    {
        this_port_device->sCANGlobalControl.bSelfReception = 0;
        this_port_device->sCANGlobalControl.bTransmitWithNoACK = 0;
    }
    
    HwdCAN_SetGlobalControl(
        this_port_device->hHwdCAN,
        &this_port_device->sCANGlobalControl
        );
    
    // we let CAN Core to handle BusOffHandle
    HwdCAN_SetBusOffHandle(
        this_port_device->hHwdCAN,
        0 // do nothing
        );
    
    // set bit timing
    rdc_candev_set_bittiming(candev);
    
    // start CAN & IF by Pass
    HwdCAN_StartCAN(this_port_device->hHwdCAN);
    
    // re-setting CANIF
    //if (FilterEnable)
    {
        //HwdCAN_DisableCANIFByPass(this_port_device->hHwdCAN);
        //HwdCAN_SetFilter
    }
    //else
    {
        HwdCAN_EnableCANIFByPass(this_port_device->hHwdCAN);
    }
    
    HwdCAN_StartCANIF(this_port_device->hHwdCAN);
    
    this_port_device->EmptyTxBufferIndex = 0;
    this_port_device->TxBufferFlag[0] = 0;
    this_port_device->TxBufferFlag[1] = 0;
    this_port_device->TxBufferFlag[2] = 0;
    
    // set current operational state to CAN_STATE_ERROR_ACTIVE
    rdccanpriv->can.state = CAN_STATE_ERROR_ACTIVE;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
rdc_candev_stop(
            struct  net_device          *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    DrvDbgPrint(0, "rdc_candev_stop Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    // stop CAN
    HwdCAN_StopCAN(this_port_device->hHwdCAN);
    
    // set current operational state to CAN_STATE_STOPPED
    rdccanpriv->can.state = CAN_STATE_STOPPED;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
rdc_candev_open(
            struct  net_device          *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
    
    int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    DrvDbgPrint(0, "rdc_candev_open Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    // open candev
    reterr = open_candev(candev);
    if (reterr < 0)
    {
        DrvDbgPrint(0, "    open_candev failed \n");
        retval = -EFAULT;
        goto funcexit;
    }
    
    // start CAN
    rdc_candev_start(candev);
    
    //_rdc_show_regs(this_port_device->hHwdCAN);
    
    // OpenCount
    this_port_device->OpenCount = 1;
    
    // enable napi
    napi_enable(&rdccanpriv->napi);
    
    // start netif queue
    netif_start_queue(candev);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
rdc_candev_close(
            struct  net_device          *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
          
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    DrvDbgPrint(0, "rdc_candev_close Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    // stop netif queue
    netif_stop_queue(candev);
    
    // disable napi
    napi_disable(&rdccanpriv->napi);
    
    // OpenCount
    this_port_device->OpenCount = 0;
    
    // stop CAN
    rdc_candev_stop(candev);
    
    // close candev
    close_candev(candev);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


netdev_tx_t
rdc_candev_start_xmit(
            struct sk_buff              *skb,
            struct net_device           *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    //unsigned int                i;
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
    struct net_device_stats     *net_stats;
    
    struct can_frame            *pcanframe;
    
    unsigned int                TxBufferIndex;
    CANTxBuffer                 sCANTxBuffer;
    
    enum can_state              canstate;
    
    unsigned long               flags;
                                //irq flags use unsigned long in linux
    
    //
    // initialize local variables
    //
    
    retval = NETDEV_TX_OK;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    net_stats = &candev->stats;
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    //DrvDbgPrint(0, "rdc_candev_start_xmit Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    pcanframe = (struct can_frame *)skb->data;

    // maybe, we can use ioctl to do Hardware Filter Setting. see LDD chapter 17
//#define CAN_ERR_SetFilter           0x00010000U
    // dlc = 9
    // data[0]:FilterIndex
    // data[4-1]:ID
    // data[8-5]:Mask
    

//#define CAN_ERR_EnableFilter        0x00020000U
    // dlc = 1
    // data[0]:FilterIndex
    
    
//#define CAN_ERR_DisableFilter       0x00030000U
    // dlc = 1
    // data[0]:FilterIndex
    
//#define CAN_ERR_RemoveAllFilter     0x00040000U
    // dlc = 0

//#define CAN_ERR_EnableCANIFByPass   0x00050000U
//#define CAN_ERR_DisableCANIFByPass  0x00060000U
    //if ((pcanframe->can_id & CAN_ERR_FLAG) && FilterEnable)
    {
        //if (pcanframe->can_id & CAN_ERR_FilterSetting)
        {
            //HwdCAN_SetFilter(FilterIndex);
        }
        //if (pcanframe->can_id & CAN_ERR_FilterEnable)
        {
            //HwdCAN_EnableFilter(FilterIndex);
        }
        //if (pcanframe->can_id & CAN_ERR_DisableFilter)
        {
            //HwdCAN_DisableFilter(FilterIndex);
        }
        {
            //HwdCAN_RemoveAllFilter
        }
    }
    
    if (can_dropped_invalid_skb(candev, skb))
    {
        DrvDbgPrint(0, "    start_xmit can_dropped_invalid_skb \n");
        retval = NETDEV_TX_OK;
        goto funcexit;
    }
    
    rdc_candev_get_state(candev, &canstate);
    if (canstate == CAN_STATE_BUS_OFF)
    {   // refer to can_dropped_invalid_skb()
        //DrvDbgPrint(0, "    start_xmit when CAN_STATE_BUS_OFF\n");
        kfree_skb(skb);
        net_stats->tx_dropped += 1;
        retval = NETDEV_TX_OK;
        goto funcexit;
    }
    if (canstate == CAN_STATE_STOPPED)
    {   // refer to can_dropped_invalid_skb()
        //DrvDbgPrint(0, "    start_xmit when CAN_STATE_STOPPED\n");
        kfree_skb(skb);
        net_stats->tx_dropped += 1;
        retval = NETDEV_TX_OK;
        goto funcexit;
    }
    
    // find an empty tx buffer.
    TxBufferIndex = this_port_device->EmptyTxBufferIndex;
    if (this_port_device->EmptyTxBufferIndex == (RDC_Number_TxBuffer - 1))
    {
        netif_stop_queue(candev);
        this_port_device->EmptyTxBufferIndex = 0;
    }
    else
    {
        this_port_device->EmptyTxBufferIndex += 1;
    }
    
    //SPZeroMemory(&sCANMessage, sizeof(CANMessage));
    
    sCANTxBuffer.bExtendedFrame = CANMessage_FrameFormat_Standard;
    sCANTxBuffer.ID = pcanframe->can_id & CAN_SFF_MASK;
    if (pcanframe->can_id & CAN_EFF_FLAG)
    {
        sCANTxBuffer.bExtendedFrame = CANMessage_FrameFormat_Extended;
        sCANTxBuffer.ID = pcanframe->can_id & CAN_EFF_MASK;
    }
    
    sCANTxBuffer.RTRBit = CANMessage_FrameType_Data;
    if (pcanframe->can_id & CAN_RTR_FLAG)
    {
        sCANTxBuffer.RTRBit = CANMessage_FrameType_Remote;
    }
    
    sCANTxBuffer.DataLengthCode = pcanframe->can_dlc;
    
    sCANTxBuffer.Byte0_3 = *(PULONG)&pcanframe->data[0];
    sCANTxBuffer.Byte4_7 = *(PULONG)&pcanframe->data[4];
    
    // put skb to candev core echo socket Buffer before sending message.
    
    //because rdc_candev_start_xmit is called in tasklet.
    //SPSpinLock_AcquireAtDpcLevel(&this_port_device->sSPSpinLock_candev, &flags);
    
    can_put_echo_skb(skb, candev, TxBufferIndex);
    
    //SPSpinLock_ReleaseFromDpcLevel(&this_port_device->sSPSpinLock_candev, &flags);
    
    this_port_device->TxBufferFlag[TxBufferIndex] = 1;
    this_port_device->TxBufferDLC[TxBufferIndex] = pcanframe->can_dlc;
    
    result = HwdCAN_SendTxBuffer(
        this_port_device->hHwdCAN,
        TxBufferIndex,
        &sCANTxBuffer
        );
    //if (!FR_SUCCESS(result))
    //{
        // HwdCAN_SendMessage2 dont check busy if hardware have no bug.
        //retval = NETDEV_TX_BUSY;
        //goto funcexit;
    //}
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}

    

int
rdc_candev_set_bittiming(
            struct net_device           *candev
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    DrvDbgPrint(0, "rdc_candev_set_bittiming Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    // setting bit timing according to can_priv.bittiming
    
    DrvDbgPrint(0, "    bittiming.brp = 0x%x, prop_seg=%u, phase_seg1=%u phase_seg2=%u sjw=%u \n",
        rdccanpriv->can.bittiming.brp,
        rdccanpriv->can.bittiming.prop_seg,
        rdccanpriv->can.bittiming.phase_seg1,
        rdccanpriv->can.bittiming.phase_seg2,
        rdccanpriv->can.bittiming.sjw
        );
    
    result = HwdCAN_SetClockPrescaler(
        this_port_device->hHwdCAN,
        (rdccanpriv->can.bittiming.brp >> 1) - 1,
        1
        );
    if (!FR_SUCCESS(result))
    {
        DrvDbgPrint(DMT_Error, "    HwdCAN_SetClockPrescaler Error:0x%X \n", result);
        retval = -EFAULT;
        goto funcexit;
    }
    
    result = HwdCAN_SetBusTiming(
        this_port_device->hHwdCAN,
        rdccanpriv->can.bittiming.prop_seg - 1,
        rdccanpriv->can.bittiming.phase_seg1 - 1,
        rdccanpriv->can.bittiming.phase_seg2 - 1,
        rdccanpriv->can.bittiming.sjw - 1,
        0 // 3 time Sampling    // 0 or 1
        );
    if (!FR_SUCCESS(result))
    {
        DrvDbgPrint(DMT_Error, "    HwdCAN_SetBusTiming Error:0x%X \n", result);
        retval = -EFAULT;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
rdc_candev_set_mode(
            struct net_device           *candev,
            enum can_mode               canmode
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    //DrvDbgPrint(0, "rdc_candev_set_mode Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    // setting CAN power mode
    
    switch (canmode)
    {
        case CAN_MODE_STOP: // 0
        {
            DrvDbgPrint(0, "    can power mode: CAN_MODE_STOP \n");
            retval = -EOPNOTSUPP;
        }
        break;
        
        case CAN_MODE_START: // 1, restart
        {
            DrvDbgPrint(0, "    can power mode: CAN_MODE_START \n");
            rdc_candev_start(candev);
            if (netif_queue_stopped(candev))
            {
                netif_wake_queue(candev);
            }
        }
        break;
        
        case CAN_MODE_SLEEP: // 2
        {
            DrvDbgPrint(0, "    can power mode: CAN_MODE_SLEEP \n");
            retval = -EOPNOTSUPP;
        }
        break;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
rdc_candev_get_state(
            const struct net_device     *candev,
            enum can_state              *pcanstate
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    struct rdc_can_priv         *rdccanpriv;
    
    ULONG                       bErrorCounterWarning;
    ULONG                       bErrorPassive;
    ULONG                       bBusOff;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    //DrvDbgPrint(0, "rdc_candev_get_state Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    if (rdccanpriv->can.state == CAN_STATE_STOPPED)
    {
        *pcanstate = CAN_STATE_STOPPED;
        goto funcexit;
    }
    
    HwdCAN_GetGlobalStatus(
        this_port_device->hHwdCAN,
        &bErrorCounterWarning,
        &bErrorPassive,
        &bBusOff
        );
    
    *pcanstate = CAN_STATE_ERROR_ACTIVE;
    
    if (bErrorCounterWarning)
    {
        *pcanstate = CAN_STATE_ERROR_WARNING;
    }
    
    if (bErrorPassive)
    {
        *pcanstate = CAN_STATE_ERROR_PASSIVE;
    }
    
    if (bBusOff)
    {
        *pcanstate = CAN_STATE_BUS_OFF;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}


int
rdc_candev_get_berr_counter(
            const struct net_device     *candev,
            struct can_berr_counter     *pcanberrcounter
    )
{
    int                         retval;
                                // return -ERRNO -EFAULT
                                
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                // check if (!FR_SUCCESS(result))
    
    struct module_port_device   *this_port_device;
    //struct net_device           *candev;
    struct rdc_can_priv         *rdccanpriv;
    
    ULONG                       TxErrorCounter;
    ULONG                       RxErrorCounter;
                                    
    //
    // initialize local variables
    //
    
    retval = 0;
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    rdccanpriv = netdev_priv(candev);
    
    this_port_device = &ThisModule_port_devices[rdccanpriv->port_minor];
    
    //DrvDbgPrint(0, "rdc_candev_get_berr_counter Entry, device minor:%u portid:%u \n", this_port_device->minor, this_port_device->portid);
    
    HwcCAN_GetErrorCounter(
        this_port_device->hHwdCAN->hHwcCAN,
        &TxErrorCounter,
        &RxErrorCounter
        );
    
    pcanberrcounter->txerr = TxErrorCounter;
    pcanberrcounter->rxerr = RxErrorCounter;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return retval;
    //return ;
}

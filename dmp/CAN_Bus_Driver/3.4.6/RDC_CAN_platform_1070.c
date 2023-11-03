#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>     // kzalloc

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

#include <linux/platform_device.h>
//#include <linux/pci.h>      // about PCI_DEVFN, PCI_SLOT, PCI_FUNC

#include <linux/device.h>
#include <linux/fs.h>       // struct inode need this header file.
#include <linux/cdev.h>

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
#include "RDC_CAN_platform.h"
#include "RDC_CAN_platform_1070.h"

//----
// Driver Infomation
//

//#define KernelVersion      "3.5"        // linux version
#define DRIVER_NAME         "RDC_CAN_platform_1070"   // RDC CAN Driver
#define DRIVER_VERSION      "1.0.0"          //

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kevin.haung@rdc.com.tw");
MODULE_DESCRIPTION("RDC CAN Controller Driver");
MODULE_VERSION(DRIVER_VERSION);
MODULE_INFO(depends, "Module_RDC_CAN_platform");

#define MAX_DEVICES_ON_PLATFORM   2     // this module support


//----
// define module parameters
//

static int io[MAX_DEVICES_ON_PLATFORM] = {
    [0 ... MAX_DEVICES_ON_PLATFORM-1] = 0
};
static int irq[MAX_DEVICES_ON_PLATFORM] = {
    [0 ... MAX_DEVICES_ON_PLATFORM-1] = 0
};

MODULE_PARM_DESC(io, " Base I/O address");
module_param_array(io, int, NULL, 0);

MODULE_PARM_DESC(irq, " IRQ number");
module_param_array(irq, int, NULL, 0);

static struct platform_device * ThisModule_supported_devices[MAX_DEVICES_ON_PLATFORM] =
{
    0,
    0,
    
};

// platform_device_alloc

// platform_device_add_data to platform_data

// platform_device_add_resources
// platform_get_resource

// platform_device_add
// platform_device_del

// platform_device_put  free 

//----
// Module Block
//

int __init Module_init(void)
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
    
    unsigned int                i;
    
    struct resource             resources[2];
    
    DrvDbgPrint(0, "Module_init: %s \n", DRIVER_NAME);
    
    // initialize module level (global) data.
    
    retval = 0;
    
    for (i = 0; i < MAX_DEVICES_ON_PLATFORM; i++)
    {
        ThisModule_supported_devices[i] = NULL;
        if (io[i] != 0 && irq[i] != 0)
        {
            ThisModule_supported_devices[i] = platform_device_alloc("fixed_17f3_1070_1", fixed_17f3_1070_1); // this_device_id->name and this_device_id->driver_data
            if (ThisModule_supported_devices[i] == NULL)
            {
                DrvDbgPrint(0, "    platform_device_alloc failed \n");
                retval = -EFAULT;
                break;
            }
            else
            {
                DrvDbgPrint(0, "    sizeof(resources):%lu \n", sizeof(resources));
                
                DrvDbgPrint(0, "    i = %d \n", i);
                DrvDbgPrint(0, "    io:0x%x irq=%d \n", io[i], irq[i]);
                
                memset(resources, 0, sizeof(resources));
                
                resources[0].start = io[i];
                resources[0].end = io[i] + 128 - 1;
                resources[0].flags = IORESOURCE_IO;
                
                resources[1].start = irq[i];
                resources[1].end = irq[i];
                resources[1].flags = IORESOURCE_IRQ;
                
                result = platform_device_add_resources(
                    ThisModule_supported_devices[i],
					resources,
					2
                    );
                if (result != 0)
                {
                    DrvDbgPrint(0, "    platform_device_add_resources failed \n");
                    retval = -EFAULT;
                    break;
                }
                
                result = platform_device_add(ThisModule_supported_devices[i]);
                if (result != 0)
                {
                    DrvDbgPrint(0, "    platform_device_add failed \n");
                    retval = -EFAULT;
                    break;
                }
            }
        }
    }
    
    if (retval != 0)
    {
        for (i = 0; i < MAX_DEVICES_ON_PLATFORM; i++)
        {
            if (ThisModule_supported_devices[i] != NULL)
            {
                platform_device_del(ThisModule_supported_devices[i]);
                platform_device_put(ThisModule_supported_devices[i]);
                ThisModule_supported_devices[i] = NULL;
            }
        }
    }
    
    return retval;
}

void __exit Module_exit(void)
{
    unsigned int                i;
    
    DrvDbgPrint(0, "Module_exit: %s \n", DRIVER_NAME);
    
    for (i = 0; i < MAX_DEVICES_ON_PLATFORM; i++)
    {
        if (ThisModule_supported_devices[i] != NULL)
        {
            DrvDbgPrint(0, "    platform_device_del i:%d \n", i);
            
            platform_device_del(ThisModule_supported_devices[i]);
            platform_device_put(ThisModule_supported_devices[i]);
            ThisModule_supported_devices[i] = NULL;
        }
    }
}

module_init(Module_init);
module_exit(Module_exit);


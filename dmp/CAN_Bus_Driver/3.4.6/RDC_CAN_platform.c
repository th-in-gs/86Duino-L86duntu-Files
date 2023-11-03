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


//----
// Driver Infomation
//

//#define KernelVersion      "3.5"        // linux version
#define DRIVER_NAME         "RDC_CAN_platform"   // RDC CAN Driver
#define DRIVER_VERSION      "1.0.0"          //

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kevin.haung@rdc.com.tw");
MODULE_DESCRIPTION("RDC CAN Controller Driver");
MODULE_VERSION(DRIVER_VERSION);
//MODULE_INFO(depends, "Module_RDC_CAN");

//----
// define module parameters: mp_
//

static unsigned int mp_Specific_Number[MAX_PLATFORM_SPECIFIC] = {MAX_PLATFORM_SPECIFIC, MAX_PLATFORM_SPECIFIC};

static unsigned int mp_ExternalClock[2] = {0, 0};

MODULE_PARM_DESC(mp_Specific_Number, " Specific_Number[0-1]");
module_param_array(mp_Specific_Number, uint, NULL, 0);

MODULE_PARM_DESC(mp_ExternalClock, " ExternalClock[0-1]");
module_param_array(mp_ExternalClock, uint, NULL, 0);   // dont use "unsigned int", use one token name.


//----
// define multiple platform devices for this module supports
//


// platform_specific may be fixed or runtime
// runtime platform_specific means that the io resource can be specificed when driver loading.
static struct platform_specific ThisModule_platform_specifics[] = {
    [fixed_17f3_1070_1] = {
        .vendor         = 0x17f3,
        .device         = 0x1070,
        .subvendor      = PCI_ANY_ID,
        .subdevice      = PCI_ANY_ID,
        
        .init           = platform_default_init,
        .setup          = platform_default_setup,
        .exit           = platform_default_exit,
        
        .BaseBarForIO   = 0,
        .num_ports      = 1,
        .first_offset   = 0,
        .reg_length     = 128,
        .sourceclock    = 20000000,
    },
    
    [fixed_17f3_1071_1] = {
        .vendor         = 0x17f3,
        .device         = 0x1071,
        .subvendor      = PCI_ANY_ID,
        .subdevice      = PCI_ANY_ID,
        
        .init           = platform_default_init,
        .setup          = platform_default_setup,
        .exit           = platform_default_exit,
        
        .BaseBarForIO   = 0,
        .num_ports      = 1,
        .first_offset   = 0,
        .reg_length     = 128,
        .sourceclock    = 20000000,
    },
    
};


//----
// define platform device table for this module supports
//

static const struct platform_device_id platform_device_id_table[] = {
    // we take driver_data as id
    {
        .name           = "fixed_17f3_1070_1",
        .driver_data    = fixed_17f3_1070_1,
    },
    {
        .name           = "fixed_17f3_1071_1",
        .driver_data    = fixed_17f3_1071_1,
    },
    
    
    { }    /* terminate list */
};
MODULE_DEVICE_TABLE(platform, platform_device_id_table);

//----
// define this platform driver
//

static struct platform_driver ThisModule_platform_driver = {
    .driver = {
        .owner      = THIS_MODULE,
        .name       = DRIVER_NAME,
        },
        
    .id_table                   = platform_device_id_table,
    .probe                      = platformdevice_probe,
    .remove                     = platformdevice_remove,
//#ifdef CONFIG_PM
    .suspend                    = platformdevice_suspend,
    .resume                     = platformdevice_resume,
//#endif
};


//----
// Module Block
//

int __init Module_init(void)
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
    
    unsigned int                i;
    
    DrvDbgPrint(0, "Module_init: %s \n", DRIVER_NAME);
    
    // override pci_specific
    
    for (i = 0; i < MAX_PLATFORM_SPECIFIC; i++)
    {
        if (mp_Specific_Number[i] != MAX_PLATFORM_SPECIFIC)
        {
            if (mp_ExternalClock[i] != 0)
            {
                DrvDbgPrint(0, "    override ThisModule_platform_specifics[%u].sourceclock = %u \n", mp_Specific_Number[i], mp_ExternalClock[i]);
                ThisModule_platform_specifics[mp_Specific_Number[i]].sourceclock = mp_ExternalClock[i];
            }
        }
    }
    
    
    // initialize module level (global) data.
    
    retval = 0;
    
    retval = platform_driver_register(&ThisModule_platform_driver);
    if (retval < 0)
    {
        DrvDbgPrint(0, "    platform_register_driver failed\n");
        DrvDbgPrint(0, "    Module_init failed\n");
        return retval;
    }
    
    return 0;
}

void __exit Module_exit(void)
{
    DrvDbgPrint(0, "Module_exit: %s \n", DRIVER_NAME);
    platform_driver_unregister(&ThisModule_platform_driver);
    
}

module_init(Module_init);
module_exit(Module_exit);


//----
// callback function for pci_driver
//

int
platformdevice_probe(
            struct platform_device *    platformdev // this_dev
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
    
    unsigned int                i;
    
    struct platform_specific    *this_specific;
    struct platform_driver_data *this_driver_data;
    const struct platform_device_id     *this_device_id;
    
    unsigned int                offset;
    
    unsigned int                iospace;
    unsigned int                iolength;
    unsigned int                ioaddress;
    void *                      memaddress;
    unsigned int                assignedminor;  // assignedminor == port_minor
    
    DrvDbgPrint(0, "platformdevice_probe Entry \n");
    
    //
    // initialize local variables
    //
    
    retval = 0;
    this_specific = NULL;
    this_driver_data = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    this_device_id = platform_get_device_id(platformdev);
    
    // check if we support this device, then get its specific information.
    DrvDbgPrint(0, "    find and check this platform specific, name:%s id:%lu \n",
        this_device_id->name, this_device_id->driver_data);
    
    if (this_device_id->driver_data >= ARRAY_SIZE(ThisModule_platform_specifics))  // we take driver_data as specific number(index).
    {
        DrvDbgPrint(0, "    invalid device \n");
        retval = -EINVAL;
        goto funcexit;
    }
    
    this_specific = &ThisModule_platform_specifics[this_device_id->driver_data];;
    
    // if this device is supported by this module,
    // we create releated driver data for it and create a port device.
    
    // create driver data
    this_driver_data = kzalloc(sizeof(struct platform_driver_data), GFP_ATOMIC);
    //this_driver_data = kmalloc(sizeof(struct platform_driver_data), GFP_KERNEL);
    if (this_driver_data == NULL)
    {
        DrvDbgPrint(0, "    this_driver_data == NULL \n");
        retval = -ENOMEM;
        goto funcexit;
    }
    
    // init driver data
    this_driver_data->platformdev = platformdev;
    this_driver_data->this_specific = this_specific;
    for (i = 0; i < PCI_NUM_BAR_RESOURCES; i++)
    {
        this_driver_data->hresource[i] = NULL;
        this_driver_data->BaseAddressSpace[i] = 0;
        this_driver_data->BaseAddress[i] = 0;
        this_driver_data->BaseAddressLength[i] = 0;
        this_driver_data->MappedBaseAddress[i] = NULL;
    }
    
    for (i = 0; i < max_port_id; i++)
    {
        this_driver_data->port_minor[i] = -1;
    }
    
    // device init & setup
    DrvDbgPrint(0, "    platform device init() \n");
    if (this_specific->init)
    {
        this_specific->init(platformdev, this_driver_data);
    }
    
    DrvDbgPrint(0, "    platform device setup() \n");
    if (this_specific->setup)
    {
        result = this_specific->setup(platformdev, this_driver_data);
        if (result != 0)
        {
            retval = -EINVAL;
            goto funcexit;
        }
    }
    
    // create(or assign) a port device for each port on this pci device.
    DrvDbgPrint(0, "    create(or assign) a port device for each port \n");
    for (i = 0; i < this_specific->num_ports; i++)
    {
        // here i as port id of this multiple serial device.
        DrvDbgPrint(0, "    port id: %u \n", i);
        
        offset = this_specific->first_offset + i*this_specific->reg_length; // start offset + port index * uart io length.
        
        iolength = this_specific->reg_length;
        
        if (this_driver_data->BaseAddressSpace[this_specific->BaseBarForIO] == 0)
        {
            iospace = 0;
            ioaddress = 0;
            memaddress = this_driver_data->MappedBaseAddress[this_specific->BaseBarForIO] + offset;
        }
        else
        {
            iospace = 1;
            ioaddress = (UINTPTR)this_driver_data->MappedBaseAddress[this_specific->BaseBarForIO] + offset;
            memaddress = 0;
        }
        
        assignedminor = 0;
        result = RDCCAN_register_port_device(
            i, //portid,
            iospace,
            iolength,
            ioaddress,
            memaddress,
            this_driver_data->irq,
            &assignedminor
            );
        if (result != 0)
        {   // RDCCAN_register_port_device failed
            DrvDbgPrint(0, "    RDCCAN_register_port_device failed \n");
            this_driver_data->port_minor[i] = -1;
            retval = -ENOMEM;
            goto funcexit;
        }
        else
        {
            this_driver_data->port_minor[i] = assignedminor;
            
            // config & init port device
            // platform device dont need BusNumber, DeviceNumber, FunctionNumber
            result = RDCCAN_initialize_port_device(
                assignedminor,
                0,
                0,
                0
                );
            
            // setup port device
            result = RDCCAN_setup_port_device(
                assignedminor
                );
            if (result != 0)
            {
                RDCCAN_unregister_port_device(assignedminor);
                retval = -EFAULT;
                goto funcexit;
            }
            else
            {
                result = RDCCAN_register_candev(
                    assignedminor,
                    &platformdev->dev,
                    this_driver_data->irq,
                    this_specific->sourceclock
                    );
                if (result != 0)
                {
                    DrvDbgPrint(0, "    RDCCAN_register_candev failed \n");
                    RDCCAN_shutdown_port_device(assignedminor);
                    RDCCAN_unregister_port_device(assignedminor);
                    retval = -EFAULT;
                    goto funcexit;
                }
                
            }
        }
    }
    
    // initialize controller
    
    // enable interrupt
    
    DrvDbgPrint(0, "    platformdevice_probe Ok \n");
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    if (retval != 0)    // error occure
    {
        if (this_driver_data != NULL)
        {   
            kfree(this_driver_data);
            this_driver_data = NULL;
        }
    }
    
    return retval;
}


int
platformdevice_remove(
            struct platform_device *    platformdev // this_dev
    )
{
    unsigned int                i;
    int                         port_minor;
    
    struct platform_driver_data *this_driver_data;
    struct platform_specific    *this_specific;
    
    DrvDbgPrint(0, "pcidevice_remove Entry \n");
    
    //
    // initialize local variables
    //
    
    this_driver_data = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    this_driver_data = platform_get_drvdata(platformdev);
    this_specific = this_driver_data->this_specific;
    
    // disable interrupt
    
    // stop device
    
    DrvDbgPrint(0, "    delete(or unassign) port device for each port \n");
    // delete port device for each port.
    for (i = 0; i < this_specific->num_ports; i++)
    {
        DrvDbgPrint(0, "    port id: %u \n", i);
        
        port_minor = this_driver_data->port_minor[i];
        
        
        if (port_minor == -1)
        {
            DrvDbgPrint(0, "    this port dont have been assigned to a minor port \n");
        }
        else
        {
            RDCCAN_unregister_candev(port_minor);
            
            // shutdown port device
            RDCCAN_shutdown_port_device(
                port_minor
                );
            
            
            // unregister port device
            RDCCAN_unregister_port_device(
                port_minor
                );
        }
        
        //remove port_minor
        this_driver_data->port_minor[i] = -1;
    }
    
    // pci device exit
    DrvDbgPrint(0, "    pci device exit() \n");
    if (this_specific->exit)
    {
        this_specific->exit(platformdev, this_driver_data);
    }
    
    // free driver data
    kfree(this_driver_data);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return retval;
    return 0;
}

int
platformdevice_suspend(
            struct platform_device *    platformdev, // this_dev
            pm_message_t                state
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                
    unsigned int                i;
    int                         port_minor;                                    
    
    struct platform_driver_data *this_driver_data;
    struct platform_specific    *this_specific;
    
    //
    // initialize local variables
    //
    
    this_driver_data = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    this_driver_data = platform_get_drvdata(platformdev);
    
    this_specific = this_driver_data->this_specific;
    
    DrvDbgPrint(0, "platformdevice_suspend Entry \n");
    
    // suspend each port device
    for (i = 0; i < this_specific->num_ports; i++)
    {
        //DrvDbgPrint(0, "port id: %u \n", i);
        
        port_minor = this_driver_data->port_minor[i];
        
        if (port_minor == -1)
        {
            //DrvDbgPrint(0, "this port dont have been assigned to a minor port \n");
        }
        else
        {
            RDCCAN_suspend_candev(port_minor);
        }
    }
    
    // suspend platformdevice
    retval = 0;
    
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
platformdevice_resume(
            struct platform_device *    platformdev // this_dev
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
                                
    unsigned int                i;
    int                         port_minor;                                    
    
    struct platform_driver_data *this_driver_data;
    struct platform_specific    *this_specific;
    
    //
    // initialize local variables
    //
    
    this_driver_data = NULL;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    this_driver_data = platform_get_drvdata(platformdev);
    
    this_specific = this_driver_data->this_specific;
    
    DrvDbgPrint(0, "platformdevice_resume Entry \n");
    
    // resume platformdevice
    retval = 0;
    
    // resume each port device
    for (i = 0; i < this_specific->num_ports; i++)
    {
        //DrvDbgPrint(0, "port id: %u \n", i);
        
        port_minor = this_driver_data->port_minor[i];
        
        if (port_minor == -1)
        {
            //DrvDbgPrint(0, "this port dont have been assigned to a minor port \n");
        }
        else
        {
            RDCCAN_resume_candev(port_minor);
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
    
    return retval;
    //return;
}


int
platform_default_init(
            struct platform_device *        platformdev, // this_dev
            struct platform_driver_data *   this_driver_data
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
    
    struct resource             *hdevresource;
    
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
    
    // assign Driver Data to pci device
    platform_set_drvdata(platformdev, this_driver_data);
    
    /* enable device and prepare host */
    //platformdev_enable_device(platformdev);
    
    hdevresource = platform_get_resource(platformdev, IORESOURCE_IRQ, 0);
    if (hdevresource == NULL)
    {
        DrvDbgPrint(0, "    Note: irq == 0 \n");
        this_driver_data->irq = 0;
    }
    else
    {
        this_driver_data->irq = hdevresource->start;
    }
    
    
    // pci_set_master(pcidev);
        // our serial pci card dont support bus master.
    
    // get pci bus information.
    
    
    // show some of pci configuration.
    
    // initialize this pci device through pci configure.

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
platform_default_setup(
            struct platform_device *        platformdev, // this_dev
            struct platform_driver_data *   this_driver_data
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
    
    unsigned int                i;
    
    struct platform_specific    *this_specific;
    
    unsigned int                BaseAddress_space;
    unsigned long               BaseAddress_start;
    unsigned long               BaseAddress_length;
    
    struct resource             *hdevresource;
    
    struct resource             *hresource;
    void                        *MappedAddress;
    
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
    
    this_specific = this_driver_data->this_specific;
    
    // allocate io resoures
    DrvDbgPrint(0, "    allocating io resources\n");
    for (i = 0; i < PCI_NUM_BAR_RESOURCES; i++)
    {
        if(i == this_specific->BaseBarForIO)
        {
            hdevresource = platform_get_resource(platformdev, IORESOURCE_IO, 0);
            if (hdevresource == NULL)
            {
                hdevresource = platform_get_resource(platformdev, IORESOURCE_MEM, 0);
                if (hdevresource == NULL)
                {
                    DrvDbgPrint(0, "    BaseAddress_start == 0 \n");
                    retval = -EFAULT;
                    goto funcexit;
                }
                else
                {
                    BaseAddress_space = 0;
                }
            }
            else
            {
                BaseAddress_space = 1;
            }
            
            {
                BaseAddress_start = hdevresource->start;
                BaseAddress_length = hdevresource->end - hdevresource->start + 1;
                // we just request the right io length, because other io is reserved for GPIO.
                if (this_specific->num_ports*this_specific->reg_length > BaseAddress_length)
                {
                    DrvDbgPrint(0, "    num_ports * reg_length > BaseAddress_length \n");
                    retval = -EFAULT;
                    goto funcexit;
                }
                BaseAddress_length = this_specific->num_ports*this_specific->reg_length;
            }
            
            if (BaseAddress_start != 0)
            {
                if (BaseAddress_space == 0)
                {   // memory space
                    //struct resource *request_mem_region(unsigned long start, unsigned long len, char *name);
                    hresource = request_mem_region(
                        BaseAddress_start,
                        BaseAddress_length,
                        NULL
                        );
                    if (hresource == NULL)
                    {
                        DrvDbgPrint(0, "    request_mem_region hresource == NULL \n");
                        retval = -EFAULT;
                        //goto funcexit;
                    }
                    
                    // void *ioremap_nocache(unsigned long phys_addr, unsigned long size);
                    MappedAddress = ioremap_nocache(
                        BaseAddress_start,
                        BaseAddress_length
                        );
                    if (MappedAddress == NULL)
                    {
                        DrvDbgPrint(0, "    ioremap_nocache MappedAddress == NULL \n");
                        retval = -EFAULT;
                        //goto funcexit;
                    }
                }
                else
                {   // io space
                    //struct resource *request_region(unsigned long start, unsigned long len, char *name);
                    hresource = request_region(
                        BaseAddress_start,
                        BaseAddress_length,
                        NULL
                        );
                    if (hresource == NULL)
                    {
                        DrvDbgPrint(0, "    request_region hresource == NULL \n");
                        retval = -EFAULT;
                        //goto funcexit;
                    }
                    
                    MappedAddress = (void*)(UINTPTR)BaseAddress_start;
                }
                
                this_driver_data->hresource[i] = hresource;
                this_driver_data->BaseAddressSpace[i] = BaseAddress_space;
                this_driver_data->BaseAddress[i] = BaseAddress_start;
                this_driver_data->BaseAddressLength[i] = BaseAddress_length;
                this_driver_data->MappedBaseAddress[i] = MappedAddress;
                
                
                if (retval != 0)
                {
                    goto funcexit;
                }
                
                DrvDbgPrint(0, "    BaseAddress[%u] \n", (unsigned int)i);
                DrvDbgPrint(0, "    BaseAddressSpace:0x%x \n", (unsigned int)BaseAddress_space);
                DrvDbgPrint(0, "    BaseAddress:0x%lx \n", BaseAddress_start);
                DrvDbgPrint(0, "    BaseAddressLength:0x%lx \n", BaseAddress_length);
                DrvDbgPrint(0, "    MappedBaseAddress:0x%p \n", MappedAddress);
            }
        }
    }
    
    // request irq
    // install IRQ handle
    
    // if this pci device provide one controller, we can use this_pci_driver_data as a medium data for irq handle.
    // if this pci device provide more than one controller, we should use this_shared_irq_list to link all controllers
    //      and use it as as a medium data for irq handle.
    
    /* 
    this_pci_driver_data->bRequestIRQ = FALSE;
    if (request_irq(pcidev->irq, handler, IRQF_SHARED, DRIVER_NAME, this_pci_driver_data) != 0)
    {
        DrvDbgPrint(0, "request interrupt %i failed\n", pcidev->irq);
        retval = -EBUSY;
        goto done;
    }
    this_pci_driver_data->bRequestIRQ = TRUE;
    */
    
    retval = RDCCAN_shared_irq_reference(this_driver_data->irq);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    if (retval != 0)    // error occure
    {
        for (i = 0; i < PCI_NUM_BAR_RESOURCES; i++)
        {
            if(i == this_specific->BaseBarForIO)
            {
                if (this_driver_data->hresource[i] != NULL)
                {
                    if (this_driver_data->BaseAddressSpace[i] == 0)
                    {
                        // void iounmap(void * addr);
                        if (this_driver_data->MappedBaseAddress[i] != NULL)
                        {
                            iounmap(this_driver_data->MappedBaseAddress[i]);
                        }
                        
                        // void release_mem_region(unsigned long start, unsigned long len);
                        release_mem_region(
                            this_driver_data->BaseAddress[i],
                            this_driver_data->BaseAddressLength[i]
                            );
                    }
                    else
                    {
                        // void release_region(unsigned long start, unsigned long len);
                        release_region(
                            this_driver_data->BaseAddress[i],
                            this_driver_data->BaseAddressLength[i]
                            );
                    }
                    
                    this_driver_data->hresource[i] = NULL;
                }
            }
        }
    }
    
    
    return retval;
}


void
platform_default_exit(
            struct platform_device *        platformdev, // this_dev
            struct platform_driver_data *   this_driver_data
    )
{
    unsigned int                i;
    
    struct platform_specific    *this_specific;
    
    //
    // initialize local variables
    //
    
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    this_specific = this_driver_data->this_specific;
    
    // release irq
    RDCCAN_shared_irq_dereference(this_driver_data->irq);
    
    // free io resoureces
    for (i = 0; i < PCI_NUM_BAR_RESOURCES; i++)
    {
        if(i == this_specific->BaseBarForIO)
        {
            if (this_driver_data->hresource[i] != NULL)
            {
                if (this_driver_data->BaseAddressSpace[i] == 0)
                {
                    // void iounmap(void * addr);
                    if (this_driver_data->MappedBaseAddress[i] != NULL)
                    {
                        iounmap(this_driver_data->MappedBaseAddress[i]);
                    }
                    
                    // void release_mem_region(unsigned long start, unsigned long len);
                    release_mem_region(
                        this_driver_data->BaseAddress[i],
                        this_driver_data->BaseAddressLength[i]
                        );
                }
                else
                {
                    // void release_region(unsigned long start, unsigned long len);
                    release_region(
                        this_driver_data->BaseAddress[i],
                        this_driver_data->BaseAddressLength[i]
                        );
                }
                
                this_driver_data->hresource[i] = NULL;
            }
        }
    }
    
    //platformdev_disable_device(platformdev);
    
    platform_set_drvdata(platformdev, NULL);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return retval;
    return;
}
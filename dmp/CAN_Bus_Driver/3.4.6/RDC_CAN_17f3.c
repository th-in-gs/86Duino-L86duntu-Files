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

#include <linux/pci.h>      // about PCI_DEVFN, PCI_SLOT, PCI_FUNC

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
#include "RDC_CAN_17f3.h"


//----
// Driver Infomation
//

//#define KernelVersion      "3.5"        // linux version
#define DRIVER_NAME         "RDC_CAN_pci"   // RDC CAN Driver
#define DRIVER_VERSION      "1.0.0"          //

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kevin.haung@rdc.com.tw");
MODULE_DESCRIPTION("RDC PCI CAN Controller Driver");
MODULE_VERSION(DRIVER_VERSION);
//MODULE_INFO(depends, "Module_RDC_CAN");

//----
// define module parameters: mp_
//

static unsigned int mp_Specific_Number[MAX_PCI_SPECIFIC] = {MAX_PCI_SPECIFIC, MAX_PCI_SPECIFIC};

static unsigned int mp_ExternalClock[2] = {0, 0};

MODULE_PARM_DESC(mp_Specific_Number, " Specific_Number[0-1]");
module_param_array(mp_Specific_Number, uint, NULL, 0);

MODULE_PARM_DESC(mp_ExternalClock, " ExternalClock[0-1]");
module_param_array(mp_ExternalClock, uint, NULL, 0);   // dont use "unsigned int", use one token name.

//----
// define multiple pci card for this module supports
//

static struct pci_specific ThisModule_pci_specifics[] = {
    [psn_17f3_1070] = {
        .vendor         = 0x17f3,
        .device         = 0x1070,
        .subvendor      = PCI_ANY_ID,
        .subdevice      = PCI_ANY_ID,
        
        .init           = pci_default_init,
        .setup          = pci_default_setup,
        .exit           = pci_default_exit,
        
        .BaseBarForIO   = 0,
        .num_ports      = 1,
        .first_offset   = 0,
        .reg_length     = 128,
        .sourceclock    = 20000000,
    },
    [psn_17f3_1071] = {
        .vendor         = 0x17f3,
        .device         = 0x1071,
        .subvendor      = PCI_ANY_ID,
        .subdevice      = PCI_ANY_ID,
        
        .init           = pci_default_init,
        .setup          = pci_default_setup,
        .exit           = pci_default_exit,
        
        .BaseBarForIO   = 0,
        .num_ports      = 1,
        .first_offset   = 0,
        .reg_length     = 128,
        .sourceclock    = 50000000,
    },
};


//----
// define pci device table for this module supports
//

static const struct pci_device_id pci_device_id_table[] = {
    // {vendor, device, subvendor, subdevice, class, classmask}
    { 0x17f3, 0x1070, PCI_ANY_ID, PCI_ANY_ID, 0, 0, psn_17f3_1070},
    { 0x17f3, 0x1071, PCI_ANY_ID, PCI_ANY_ID, 0, 0, psn_17f3_1071},
    { }    /* terminate list */
};
MODULE_DEVICE_TABLE(pci, pci_device_id_table);

//----
// define this pci driver
//

static struct pci_driver ThisModule_pci_driver = {
    .name                       = DRIVER_NAME,
    .id_table                   = pci_device_id_table,
    .probe                      = pcidevice_probe,
    .remove                     = pcidevice_remove,
#ifdef CONFIG_PM
    .suspend                    = pcidevice_suspend,
    .resume                     = pcidevice_resume,
#endif
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
    
    for (i = 0; i < MAX_PCI_SPECIFIC; i++)
    {
        if (mp_Specific_Number[i] != MAX_PCI_SPECIFIC)
        {
            if (mp_ExternalClock[i] != 0)
            {
                DrvDbgPrint(0, "    override ThisModule_pci_specifics[%u].sourceclock = %u \n", mp_Specific_Number[i], mp_ExternalClock[i]);
                ThisModule_pci_specifics[mp_Specific_Number[i]].sourceclock = mp_ExternalClock[i];
            }
        }
    }
    
    
    // initialize module level (global) data.
    
    retval = 0;
    
    retval = pci_register_driver(&ThisModule_pci_driver);
    if (retval < 0)
    {
        DrvDbgPrint(0, "    pci_register_driver failed\n");
        DrvDbgPrint(0, "    Module_init failed\n");
        return retval;
    }
    
    return 0;
}

void __exit Module_exit(void)
{
    DrvDbgPrint(0, "Module_exit: %s \n", DRIVER_NAME);
    pci_unregister_driver(&ThisModule_pci_driver);
    
}

module_init(Module_init);
module_exit(Module_exit);

unsigned int
ReadPCIConfiguration(
            unsigned int        BusNumber,
            unsigned int        DeviceNumber,
            unsigned int        FunctionNumber,
            unsigned int        Offset,
            unsigned int        Length, // 1 , 2 , 4
            void *              pBuffer
    )
{
    unsigned int                funcresult;
    
    unsigned int                result;
    
    unsigned int                devfn;
    
    struct  pci_dev     *hpci_dev;
        
    funcresult = TRUE;
    
    hpci_dev = NULL;
    
    devfn = PCI_DEVFN(DeviceNumber, FunctionNumber);
    
    //struct pci_dev * pci_get_bus_and_slot(unsigned int bus, unsigned int devfn)
    hpci_dev = pci_get_bus_and_slot(
        BusNumber,
        devfn
        );
    if (hpci_dev == NULL)
    {
        funcresult = FALSE;
        goto funcexit;
    }
    
    
    switch(Length)
    {
        case 1:
        {
            // pci_read_config_byte(struct pci_dev *dev, int where, u8 *val);
            result = pci_read_config_byte(
                hpci_dev,
                Offset,
                pBuffer
                );
            if (result != PCIBIOS_SUCCESSFUL)
            {
                funcresult = FALSE;
                goto funcexit;
            }
        }
        break;
        
        case 2:
        {
            result = pci_read_config_word(
                hpci_dev,
                Offset,
                pBuffer
                );
            if (result != PCIBIOS_SUCCESSFUL)
            {
                funcresult = FALSE;
                goto funcexit;
            }
        }
        break;
        
        case 4:
        {
            result = pci_read_config_dword(
                hpci_dev,
                Offset,
                pBuffer
                );
            if (result != PCIBIOS_SUCCESSFUL)
            {
                funcresult = FALSE;
                goto funcexit;
            }
        }
        break;
        
        default:
        {
            funcresult = FALSE;
            goto funcexit;
        }
        break;
    }
    
    funcresult = TRUE;
    
funcexit:

    if (hpci_dev != NULL)
    {
        pci_dev_put(hpci_dev);
    }
    
    if (funcresult == FALSE)
    {
        DrvDbgPrint(0, "    ReadPCIConfiguration result == FALSE \n");
    }
    
    return funcresult;
}


unsigned int
WritePCIConfiguration(
            unsigned int        BusNumber,
            unsigned int        DeviceNumber,
            unsigned int        FunctionNumber,
            unsigned int        Offset,
            unsigned int        Length, // 1 , 2 , 4
            void *              pBuffer
    )
{
    unsigned int                funcresult;
    
    unsigned int                result;
    
    unsigned int                devfn;
    
    struct  pci_dev     *hpci_dev;
        
    funcresult = TRUE;
    
    hpci_dev = NULL;
    
    devfn = PCI_DEVFN(DeviceNumber, FunctionNumber);
    
    //struct pci_dev * pci_get_bus_and_slot(unsigned int bus, unsigned int devfn)
    hpci_dev = pci_get_bus_and_slot(
        BusNumber,
        devfn
        );
    if (hpci_dev == NULL)
    {
        funcresult = FALSE;
        goto funcexit;
    }
    
    
    switch(Length)
    {
        case 1:
        {
            // pci_write_config_byte(struct pci_dev *dev, int where, u8 *val);
            result = pci_write_config_byte(
                hpci_dev,
                Offset,
                *(u8*)pBuffer
                );
            if (result != PCIBIOS_SUCCESSFUL)
            {
                funcresult = FALSE;
                goto funcexit;
            }
        }
        break;
        
        case 2:
        {
            result = pci_write_config_word(
                hpci_dev,
                Offset,
                *(u16*)pBuffer
                );
            if (result != PCIBIOS_SUCCESSFUL)
            {
                funcresult = FALSE;
                goto funcexit;
            }
        }
        break;
        
        case 4:
        {
            result = pci_write_config_dword(
                hpci_dev,
                Offset,
                *(u32*)pBuffer
                );
            if (result != PCIBIOS_SUCCESSFUL)
            {
                funcresult = FALSE;
                goto funcexit;
            }
        }
        break;
        
        default:
        {
            funcresult = FALSE;
            goto funcexit;
        }
        break;
    }
    
    funcresult = TRUE;
    
funcexit:

    if (hpci_dev != NULL)
    {
        pci_dev_put(hpci_dev);
    }
    
    if (funcresult == FALSE)
    {
        DrvDbgPrint(0, "WritePCIConfiguration result == FALSE \n");
    }
    
    return funcresult;
}

//----
// callback function for pci_driver
//

/**
 *    Register PCI device with kernel services
 *    @pdev: PCI device to register
 *    @pid: Entry in sch_pci_tbl matching with @pdev
 *
 *    LOCKING:
 *    Inherited from PCI layer (may sleep).
 *
 *    RETURNS:
 *    Zero on success, or -ERRNO value.
 */
int
pcidevice_probe(
            struct pci_dev *                pcidev, // this_dev
            const struct pci_device_id *    this_device_id
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
    
    
    unsigned int                i;
    
    struct pci_specific         *this_specific;
    struct pci_driver_data      *this_driver_data;
    
    unsigned int                offset;
    
    unsigned int                iospace;
    unsigned int                iolength;
    unsigned int                ioaddress;
    void *                      memaddress;
    unsigned int                assignedminor;  // assignedminor == port_minor
    
    DrvDbgPrint(0, "pcidevice_probe Entry \n");
    
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
    
    // check if we support this device, then get its specific information.
    DrvDbgPrint(0, "    find and check this pci specific, VendorID:0x%x DeviceID:0x%x \n",
        this_device_id->vendor, this_device_id->device);
    if (this_device_id->driver_data >= ARRAY_SIZE(ThisModule_pci_specifics))  // we take driver_data as specific number(index).
    {
        DrvDbgPrint(0, "    invalid device \n");
        retval = -EINVAL;
        goto funcexit;
    }
    
    this_specific = &ThisModule_pci_specifics[this_device_id->driver_data];
    
    // if this device is supported by this module,
    // we create releated driver data for it and create a port device.
    
    // create driver data
    this_driver_data = kzalloc(sizeof(struct pci_driver_data), GFP_ATOMIC);
    //this_driver_data = kmalloc(sizeof(struct pci_driver_data), GFP_KERNEL);
    if (this_driver_data == NULL)
    {
        DrvDbgPrint(0, "    this_driver_data == NULL \n");
        retval = -ENOMEM;
        goto funcexit;
    }
    
    // init driver data
    this_driver_data->pcidev = pcidev;
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
    
    // pci device init & setup
    DrvDbgPrint(0, "    pci device init() \n");
    if (this_specific->init)
    {
        this_specific->init(pcidev, this_driver_data);
    }
    
    DrvDbgPrint(0, "    pci device setup() \n");
    if (this_specific->setup)
    {
        result = this_specific->setup(pcidev, this_driver_data);
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
        {   // RDCCAN_register_can_device failed
            DrvDbgPrint(0, "    RDCCAN_register_port_device failed \n");
            this_driver_data->port_minor[i] = -1;
            retval = -ENOMEM;
            goto funcexit;
        }
        else
        {
            this_driver_data->port_minor[i] = assignedminor;
            
            // config & init port device
            result = RDCCAN_initialize_port_device(
                assignedminor,
                pcidev->bus->number,
                PCI_SLOT(pcidev->devfn),
                PCI_FUNC(pcidev->devfn)
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
                    &pcidev->dev,
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
    
    DrvDbgPrint(0, "    pcidevice_probe Ok \n");
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    if (retval != 0)    // error occur
    {
        if (this_driver_data != NULL)
        {   
            kfree(this_driver_data);
            this_driver_data = NULL;
        }
    }
    
    return retval;
}


void
pcidevice_remove(
            struct pci_dev *    pcidev  // this_dev
    )
{
    unsigned int                i;
    int                         port_minor;
    
    struct pci_driver_data      *this_driver_data;
    struct pci_specific         *this_specific;
    
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
    
    this_driver_data = pci_get_drvdata(pcidev);
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
        this_specific->exit(pcidev, this_driver_data);
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
    return;
}


int
pcidevice_suspend(
            struct pci_dev *    pcidev, // this_dev
            pm_message_t        state
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
    
    struct pci_driver_data      *this_driver_data;
    struct pci_specific         *this_specific;
    
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
    
    this_driver_data = pci_get_drvdata(pcidev);
    
    this_specific = this_driver_data->this_specific;
    
    DrvDbgPrint(0, "pcidevice_suspend Entry \n");
    
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
    
    // suspend pci device
    pci_enable_wake(pcidev, PCI_D3hot, 0);  // disable d3 wakeup
    
    retval = pci_save_state(pcidev);
    if (retval)
    {
        DrvDbgPrint(0, "    pci_save_state failed.\n");
    }
    else
    {   
        pci_disable_device(pcidev);
        
        pci_set_power_state(pcidev, pci_choose_state(pcidev, state));
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
pcidevice_resume(
            struct pci_dev *    pcidev  // this_dev
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
    
    struct pci_driver_data      *this_driver_data;
    struct pci_specific         *this_specific;
    
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
    
    this_driver_data = pci_get_drvdata(pcidev);
    
    this_specific = this_driver_data->this_specific;
    
    DrvDbgPrint(0, "pcidevice_resume Entry \n");
    
    // resume pci device
    pci_enable_wake(pcidev, PCI_D3hot, 0);  // disable d3 wakeup
    
    pci_restore_state(pcidev);
    
    retval = pci_enable_device(pcidev);
    if (retval)
    {
        DrvDbgPrint(0, "    pci_enable_device failed.\n");
        return retval;
    }
    
    pci_set_power_state(pcidev, PCI_D0);
    
    
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
pci_default_init(
    struct pci_dev *            pcidev,
    struct pci_driver_data *    this_driver_data
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    unsigned int                result;
                                // check if (result != 0)
    
    
    unsigned int                BusNumber;
    unsigned int                DeviceNumber;
    unsigned int                FunctionNumber;
    
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
    pci_set_drvdata(pcidev, this_driver_data);
    
    /* enable device and prepare host */
    result = pci_enable_device(pcidev);
    if (result != 0)
    {
        DrvDbgPrint(0, "    pci_enable_device failed \n");
        retval = result;
        goto funcexit;
    }
    
    // after pci_enable_device, pcidev->irq become routed irq.
    this_driver_data->irq = pcidev->irq;
    
    // pci_set_master(pcidev);
        // our serial pci card dont support bus master.
    
    // get pci bus information.
    BusNumber = pcidev->bus->number;
    DeviceNumber = PCI_SLOT(pcidev->devfn);
    FunctionNumber = PCI_FUNC(pcidev->devfn);
    
    // show some of pci configuration.
    DrvDbgPrint(0, "    BusNumber:%u DeviceNumber:%u FunctionNumber:%u \n",
        BusNumber, DeviceNumber, FunctionNumber);
    
    result = 0;
    ReadPCIConfiguration(
        BusNumber,
        DeviceNumber,
        FunctionNumber,
        4,
        2, // 1 , 2 , 4
        &result
        );
    DrvDbgPrint(0, "    Command Register:0x%x \n", result);
    
    result = 0;
    ReadPCIConfiguration(
        BusNumber,
        DeviceNumber,
        FunctionNumber,
        6,
        2, // 1 , 2 , 4
        &result
        );
    DrvDbgPrint(0, "    Status Register:0x%x \n", result);
    
    result = 0;
    ReadPCIConfiguration(
        BusNumber,
        DeviceNumber,
        FunctionNumber,
        0x10,
        4, // 1 , 2 , 4
        &result
        );
    DrvDbgPrint(0, "    Base0 Register:0x%x \n", result);
    
    result = 0;
    ReadPCIConfiguration(
        BusNumber,
        DeviceNumber,
        FunctionNumber,
        0x14,
        4, // 1 , 2 , 4
        &result
        );
    DrvDbgPrint(0, "    Base1 Register:0x%x \n", result);
    
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
pci_default_setup(
    struct pci_dev *            pcidev,
    struct pci_driver_data *    this_driver_data
    )
{
    int                         retval;
                                // return -ERRNO
    
    //int                         reterr;
                                // check if (reterr < 0)
    
    //unsigned int                result;
                                // check if (result != 0)
    
    unsigned int                i;
    
    struct pci_specific         *this_specific;
    
    unsigned int                BaseAddress_space;
    unsigned long               BaseAddress_start;
    unsigned long               BaseAddress_length;
    
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
            if (pci_resource_flags(pcidev, i) & IORESOURCE_MEM)
            {
                BaseAddress_space = 0;
            }
            else
            {
                BaseAddress_space = 1;
            }
            
            {
                BaseAddress_start = pci_resource_start(pcidev, i);
                BaseAddress_length = pci_resource_len(pcidev, i);
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
    
    // if this pci device provide one controller, we can use this_driver_data as a medium data for irq handle.
    // if this pci device provide more than one controller, we should use this_shared_irq_list to link all controllers
    //      and use it as as a medium data for irq handle.
    
    /* 
    this_driver_data->bRequestIRQ = FALSE;
    if (request_irq(pcidev->irq, handler, IRQF_SHARED, DRIVER_NAME, this_driver_data) != 0)
    {
        DrvDbgPrint(0, "request interrupt %i failed\n", pcidev->irq);
        retval = -EBUSY;
        goto done;
    }
    this_driver_data->bRequestIRQ = TRUE;
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
pci_default_exit(
    struct pci_dev *            pcidev,
    struct pci_driver_data *    this_driver_data
    )
{
    unsigned int                i;
    
    struct pci_specific         *this_specific;
    
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
    
    pci_disable_device(pcidev);
    
    pci_set_drvdata(pcidev, NULL);
    
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

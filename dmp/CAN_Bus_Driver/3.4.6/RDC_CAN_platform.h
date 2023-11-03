#ifndef RDC_CAN_platform_H
#define RDC_CAN_platform_H

//----
// platform device data

struct platform_device;
struct platform_specific;
struct platform_driver_data;

#define MAX_PLATFORM_SPECIFIC    2   // this module support
enum platform_specific_num_t {
    fixed_17f3_1070_1 = 0,  // fixed on board
    fixed_17f3_1071_1 = 1,
};

struct platform_specific {
    // we use this structure to do specific process of the supported platform devices.
    // those information is static and const.
    
    unsigned int                vendor;
    unsigned int                device;
    unsigned int                subvendor;
    unsigned int                subdevice;
    unsigned int                revision;
    
    // use the following callback to init/setup/exit for a pci board.
    int (*init)(struct platform_device *, struct platform_driver_data *);
                                // init pci_dev, init pci config
                                
    int (*setup)(struct platform_device *, struct platform_driver_data *);
                                // allocate resoures - io, mem, irq

    void (*exit)(struct platform_device *, struct platform_driver_data *);
                                // free resoures - io, mem, irq
    
    // we assume one IO BaseAddress can support all ports.
    unsigned int                BaseBarForIO;   // in platform always 0
    unsigned int                num_ports;
    unsigned int                first_offset;
    unsigned int                reg_length;     // per port
                                // start offset + port index * reg_length.
    unsigned int                sourceclock;
};

#define PCI_NUM_BAR_RESOURCES   5
#define max_port_id             16

struct platform_driver_data {    // pci_dev.drvdata
    
    // pci link data
    struct platform_device *    platformdev;    //alias: this_device
    struct platform_specific *  this_specific;
    
    // resources driver allocated
    struct resource *           hresource[PCI_NUM_BAR_RESOURCES];
    unsigned int                BaseAddressSpace[PCI_NUM_BAR_RESOURCES];    // 0: memory space, 1: io space
    unsigned long               BaseAddress[PCI_NUM_BAR_RESOURCES];
    unsigned long               BaseAddressLength[PCI_NUM_BAR_RESOURCES];
    void *                      MappedBaseAddress[PCI_NUM_BAR_RESOURCES];
    
    int                         irq;
    
    // port devices for this pci device
    int                         port_minor[max_port_id];
                                // the value -1 indicate that the port dont assign(allocate) a device file.
};

//----
// callback function for pci_driver
//

int
platformdevice_probe(
            struct platform_device *    platformdev // this_dev
    );
    
int
platformdevice_remove(
            struct platform_device *    platformdev // this_dev
    );

int
platformdevice_suspend(
            struct platform_device *    platformdev, // this_dev
            pm_message_t                state
    );
    
int
platformdevice_resume(
            struct platform_device *    platformdev // this_dev
    );

int
platform_default_init(
            struct platform_device *        platformdev, // this_dev
            struct platform_driver_data *   this_driver_data
    );

int
platform_default_setup(
            struct platform_device *        platformdev, // this_dev
            struct platform_driver_data *   this_driver_data
    );

void
platform_default_exit(
            struct platform_device *        platformdev, // this_dev
            struct platform_driver_data *   this_driver_data
    );


//----
// public for RDC_CAN_platform_add




#endif


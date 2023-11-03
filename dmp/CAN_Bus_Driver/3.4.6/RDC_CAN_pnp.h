#ifndef RDC_CAN_pnp_H
#define RDC_CAN_pnp_H

//----
// platform device data

struct pnp_dev;
struct pnp_specific;
struct pnp_driver_data;

#define MAX_PNP_SPECIFIC    2   // this module support
enum pnp_specific_num_t {
    fixed_17f3_1070_1 = 0,      // fixed on board
};

struct pnp_specific {
    // we use this structure to do specific process of the supported platform devices.
    // those information is static and const.
    
    unsigned int                vendor;
    unsigned int                device;
    unsigned int                subvendor;
    unsigned int                subdevice;
    unsigned int                revision;
    
    // use the following callback to init/setup/exit for a pci board.
    int (*init)(struct pnp_dev *, struct pnp_driver_data *);
                                // init pci_dev, init pci config
                                
    int (*setup)(struct pnp_dev *, struct pnp_driver_data *);
                                // allocate resoures - io, mem, irq

    void (*exit)(struct pnp_dev *, struct pnp_driver_data *);
                                // free resoures - io, mem, irq
    
    // we assume one IO BaseAddress can support all ports.
    unsigned int                BaseBarForIO;   // in pnp always 0
    unsigned int                num_ports;
    unsigned int                first_offset;
    unsigned int                reg_length;     // per port
                                // start offset + port index * reg_length.
    unsigned int                sourceclock;
};

#define PCI_NUM_BAR_RESOURCES   5
#define max_port_id             16

struct pnp_driver_data {    // pnp_dev.drvdata
    
    // pci link data
    struct pnp_dev *            pnpdev;     //alias: this_device
    struct pnp_specific *       this_specific;
    
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
pnpdevice_probe(
            struct pnp_dev *                pnpdev, // this_dev
            const struct pnp_device_id *    this_device_id
    );
    
void
pnpdevice_remove(
            struct pnp_dev *    pnpdev // this_dev
    );

int
pnpdevice_suspend(
            struct pnp_dev *    pnpdev, // this_dev
            pm_message_t        state
    );
    
int
pnpdevice_resume(
            struct pnp_dev *    pnpdev // this_dev
    );

int
pnp_default_init(
            struct pnp_dev *            pnpdev, // this_dev
            struct pnp_driver_data *    this_driver_data
    );

int
pnp_default_setup(
            struct pnp_dev *            pnpdev, // this_dev
            struct pnp_driver_data *    this_driver_data
    );

void
pnp_default_exit(
            struct pnp_dev *            pnpdev, // this_dev
            struct pnp_driver_data *    this_driver_data
    );


//----
// public for RDC_CAN_pnp_add



#endif


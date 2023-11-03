#ifndef RDC_CAN_17f3_H
#define RDC_CAN_17f3_H

unsigned int
ReadPCIConfiguration(
            unsigned int        BusNumber,
            unsigned int        DeviceNumber,
            unsigned int        FunctionNumber,
            unsigned int        Offset,
            unsigned int        Length, // 1 , 2 , 4
            void *              pBuffer
    );

unsigned int
WritePCIConfiguration(
            unsigned int        BusNumber,
            unsigned int        DeviceNumber,
            unsigned int        FunctionNumber,
            unsigned int        Offset,
            unsigned int        Length, // 1 , 2 , 4
            void *              pBuffer
    );

//----
// pci device data
// one pci card may have more than one pci device.
// each pci device need its specific init function and data.
// we use pci_specific to keep those specific init function and data.
// if kernel find a pci device on pci bus, it create a pci_dev and call probe().
// when probe(), we must create pci_driver_data and assign it to pci_dev.
// In general, a pci device has one pcidev, one pci_specific and one pci_driver_data.
//

struct pci_dev;
struct pci_specific;
struct pci_driver_data;

#define MAX_PCI_SPECIFIC    2   // this module support

enum pci_specific_num_t {
    psn_17f3_1070 = 0,
    psn_17f3_1071 = 1,
};

struct pci_specific {
    // we use this structure to do specific process of the supported pci devices.
    // those information is static and const.
    
    unsigned int                vendor;
    unsigned int                device;
    unsigned int                subvendor;
    unsigned int                subdevice;
    unsigned int                revision;
    
    // use the following callback to init/setup/exit for a pci board.
    int (*init)(struct pci_dev *, struct pci_driver_data *);
                                // init pci_dev, init pci config
                                
    int (*setup)(struct pci_dev *, struct pci_driver_data *);
                                // allocate resoures - io, mem, irq

    void (*exit)(struct pci_dev *, struct pci_driver_data *);
                                // free resoures - io, mem, irq
    
    // IO information for port devices
    // we assume one IO BaseAddress can support all ports.
    unsigned int                BaseBarForIO;   // BaseAddress Number
    unsigned int                num_ports;
    unsigned int                first_offset;
    unsigned int                reg_length;
                                // start offset + port index * reg_length.
    unsigned int                sourceclock;
};

#define PCI_NUM_BAR_RESOURCES   5
#define max_port_id             16

struct pci_driver_data {    // pci_dev.drvdata
    
    // pci link data
    struct pci_dev *            pcidev;     //alias: this_device
    struct pci_specific *       this_specific;
    
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
pcidevice_probe(
            struct pci_dev *                pcidev, // this_dev
            const struct pci_device_id *    this_device_id
    );
    
void
pcidevice_remove(
            struct pci_dev *    pcidev  // this_dev
    );

int
pcidevice_suspend(
            struct pci_dev *    pcidev, // this_dev
            pm_message_t        state
    );
    
int
pcidevice_resume(
            struct pci_dev *    pcidev  // this_dev
    );

int
pci_default_init(
    struct pci_dev *            pcidev,
    struct pci_driver_data *    this_driver_data
    );

int
pci_default_setup(
    struct pci_dev *            pcidev,
    struct pci_driver_data *    this_driver_data
    );

void
pci_default_exit(
    struct pci_dev *            pcidev,
    struct pci_driver_data *    this_driver_data
    );

/* PCI_INTERRUPT_LINE to irq
 * Read interrupt line and base address registers.
 * The architecture-dependent code can tweak these, of course.
 */
/*static void pci_read_irq(struct pci_dev *dev)
{
    unsigned char irq;

    pci_read_config_byte(dev, PCI_INTERRUPT_PIN, &irq);
    dev->pin = irq;
    if (irq)
        pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &irq);
    dev->irq = irq;
}*/

#endif


#ifndef RDC_CAN_Module_H
#define RDC_CAN_Module_H

#define is_real_interrupt(irq)    ((irq) != 0)

#define MAX_MODULE_PORTS     32

//----
// Module function
//


int
RDCCAN_shared_irq_reference(
            int                 irq
    );

int
RDCCAN_shared_irq_dereference(
            int                 irq
    );

int
RDCCAN_register_port_device(
            unsigned int                portid,
            unsigned int                iospace,
            unsigned int                iolength,
            unsigned int                ioaddress,
            void *                      memaddress,
            int                         irq,
            unsigned int                *pminor // module_port_device * this_port_device
    );

int
RDCCAN_initialize_port_device(
            unsigned int                minor,
            unsigned int                BusNumber,
            unsigned int                DeviceNumber,
            unsigned int                FunctionNumber
    );

int
RDCCAN_setup_port_device(
            unsigned int                minor
    );

int
RDCCAN_shutdown_port_device(
            unsigned int                minor
    );

int
RDCCAN_unregister_port_device(
            unsigned int                minor
    );

int
RDCCAN_unregister_port_device(
            unsigned int                minor
    );

int
RDCCAN_register_candev(
            unsigned int                minor,
            struct device               *dev,   // &pcidev->dev
            int                         irq,
            unsigned int                sourceclock
    );

int
RDCCAN_unregister_candev(
            unsigned int                minor
    );

int
RDCCAN_suspend_candev(
            unsigned int                minor
    );

int
RDCCAN_resume_candev(
            unsigned int                minor
    );


//----
// define this module.
// There is only one module_driver_data in this module.
// module_driver_data keep all devices that this module probe() and device file resource.
// a device on system bus can has more than one port for communication. we call it as module_port_device.
// in general, one module_port_device have one device file.
//

struct module;
struct module_driver_data;
struct module_port_device;
struct file_operations;
    // file_operations.ioctl be removed on kernel 3.5.
    // so we use unlocked_ioctl to replace original ioctl

struct module_driver_data {
    
    struct module *             owner;
    const char *                driver_name;
    const char *                dev_name;   // device file name
    
    // all devices that this module probe
    struct pci_dev *            allpcidev[5];
    // ACPI device
    // Plateform device
    
    // device file resource
    
    
    unsigned int                numberdevicefile;  // number of device file, chardev.count, count of char devices
                                // if there is one device file, the numberdevicefile should not equal to 0.
    
    unsigned int                major;          // MAJOR(chardev.dev);
    unsigned int                minor_start;    // MINOR(chardev.dev); minor_start
    
    //int                       device_port_minor[max_port_number];
                                // the value -1 indicate that the port dont assign(allocate) a device file.
    
    // all port devices
    
};

#define InterruptQueue_MaxCount     10
typedef struct _INTERRUPT_EXTENSION
{
    // DpcQueue; // Queue size depend on the frequency of interrupt.
                        // Dont let the queue be full
    // the following code should be modify with my ideal plans.
    // current code is a simple plan.
    CANInterrupt                losInterrupt[InterruptQueue_MaxCount];
    CANVolatileInfo             losVolatileInfo[InterruptQueue_MaxCount];
    ULONG                       Interrupt_Start;
    ULONG                       Interrupt_End;
    ULONG                       Interrupt_Count;        // number of interrupts
    ULONG                       Interrupt_Overrun;
    
    ULONG   dummy;
} INTERRUPT_EXTENSION, *PINTERRUPT_EXTENSION;

struct rdc_can_priv;

struct module_port_device {
    
    //  device link
    int                         bUsed;
    struct pci_dev *            pcidev;             // null means this device is unused.
    //struct platform_device *    platformdev;        // null means this device is unused.
    //struct acpi_device *        acpidev;            // null means this device is unused.
    
    unsigned int                BusNumber;
    unsigned int                DeviceNumber;
    unsigned int                FunctionNumber;
                                // vaild (B,D,F) if pcidev != Null
    
    // port and device minor
    unsigned int                portid;             // zero based, on a device.
    unsigned int                minor;              // minor number.
    
    // driver core
    struct list_head            shared_irq_entry;   /* IRQ entry on this port */
    //struct timer_list   timer;          /* "no irq" timer */
        // we dont support no irq handle for our multiple port.
    
    int                         irq;            // pcidev->irq, linux use "int irq;"
    unsigned int                iospace;
    unsigned int                iolength;
    unsigned int                ioaddress;
    void *                      memaddress;     // mapped address
    
    //struct cdev                 chardevicefile;
    //or struct bdev                 blockdev;
    
    SPSpinLock                  sSPSpinLock_candev;
                                // lock candev function
    struct net_device *         candev;        // candevicefile = alloc_candev
    
    struct rdc_can_priv *       rdccanpriv;     // netdev_priv(candev)
    
    unsigned int                EmptyTxBufferIndex;
    unsigned int                TxBufferFlag[3];
                                // rdc_candev_start_xmit set flag to 1;
                                // ISRDCP set flag to 0;
    unsigned int                TxBufferDLC[3];
    
    //CANFilter                   sCANFilter[32];
                                // hardware filter
    
    
    spinlock_t                  lock;
                                // OpenCount
    unsigned int                OpenCount;  // devcie file open
    
    // device handles and resources
    HwdCAN                      sHwdCAN;            // if the size of this structure is too large, you can use kzalloc.
    PHwdCAN                     hHwdCAN;
    HwdCANInterrupt             sHwdCANInterrupt;
    CANGlobalControl            sCANGlobalControl;
    
    // DPCRoutine should use tasklet mechanism to handle interrupt & event, because tasklet can not sleep.
    struct tasklet_struct       ISRDPCTasklet;
    
    // ISRDPC Queue and Rountine
    spinlock_t                  spinlock_ISRDPCQueue;
    INTERRUPT_EXTENSION         sInterruptExtension;
    
    
    // interrupt DPC events
    SPEvent                     sSPEventReceive;
    SPEvent                     sSPEventTxBuffer0Transmit;
    SPEvent                     sSPEventTxBuffer1Transmit;
    SPEvent                     sSPEventTxBuffer2Transmit;
    
    
    //unsigned short      capabilities;   /* port capabilities */
        // we dont use this variable to do(keep, note) a specific port.
        // like auto hardware flow contorl.
        
    //unsigned short      bugs;           /* port bugs */
        // we dont use this variable to do(keep, note) bugs for a specific port.
    
    // per-port callback function.
    int (*startup)(struct module_port_device *this_port_device);
                                // EvtDevicePrepareHardware, EvtDeviceD0Entry, EvtDeviceD0EntryPostInterruptsEnabled
                                
    void (*shutdown)(struct module_port_device *this_port_device);
                                // EvtDeviceD0ExitPreInterruptsDisabled, EvtDeviceD0Exit, EvtDeviceReleaseHardware
                                
    void (*pm)(struct module_port_device *this_port_device, unsigned int state, unsigned int oldstate);
};

#define RDC_Number_TxBuffer 1   // base 0
#define RDC_echo_skb_max    1   // this controller have 3 tx buffer.

struct rdc_can_priv {
    // alloc_candev need rdc_can_priv
    
    struct can_priv             can;
                                // fixed
    
    struct napi_struct          napi;   // for napi polling
                                
    int                         port_minor;

};

//----
// IRQHandler
//

irqreturn_t
IRQHandler(
            int                 irq,
            void *              devicedata
    );

int
rdc_candev_napi_poll(
            struct napi_struct *napi,
            int                 rxquota
    );

void
QuickHandleInterrupt(
            UINTPTR             minor,
            PCANInterrupt       pCANInterrupt,
            PCANVolatileInfo    pCANVolatileInfo
    );

void
ISRDPCRoutine(
            UINTPTR             uintptr  // in linux, this is unsigned long data.
    );



//----
// module_port_device callback
//

int
port_startup(
            struct module_port_device   *this_port_device
    );

void
port_shutdown(
            struct module_port_device   *this_port_device
    );

void
port_pm(
            struct module_port_device   *this_port_device,
            unsigned int                state,
            unsigned int                oldstate
    );


//----
// device core function
//

int
_rdc_candev_tx_aborted(
            struct  net_device          *candev,
            ULONG                       TxIndex
    );

int
_rdc_candev_tx_error(
            struct  net_device          *candev,
            ULONG                       TxIndex,
            PCANTransmitStatus          pCANTransmitStatus
    );

int
_rdc_candev_tx(
            struct  net_device          *candev,
            ULONG                       TxIndex,
            ULONG                       DataLengthCode
    );

int
_rdc_candev_rx_error(
            struct  net_device          *candev,
            PCANReceiveStatus           pCANReceiveStatus
    );

int
_rdc_candev_rx_overrun(
            struct  net_device          *candev
    );

int
_rdc_candev_rx(
            struct  net_device          *candev,
            PCANRxBuffer                pCANRxBuffer
    );

int
_rdc_candev_errorwarning(
            struct  net_device          *candev,
            ULONG                       TxErrorCounter,
            ULONG                       RxErrorCounter
    );

int
_rdc_candev_errorpassive(
            struct  net_device          *candev,
            ULONG                       TxErrorCounter,
            ULONG                       RxErrorCounter
    );

int
_rdc_candev_busoff(
            struct  net_device          *candev
    );

int
rdc_candev_start(
            struct  net_device          *candev
    );

int
rdc_candev_stop(
            struct  net_device          *candev
    );

int
rdc_candev_open(
            struct  net_device          *candev
    );

int
rdc_candev_close(
            struct  net_device          *candev
    );

netdev_tx_t
rdc_candev_start_xmit(
            struct sk_buff              *skb,
            struct net_device           *candev
    );

int
rdc_candev_set_bittiming(
            struct net_device           *candev
    );

int
rdc_candev_set_mode(
            struct net_device           *candev,
            enum can_mode               canmode
    );

int
rdc_candev_get_state(
            const struct net_device     *candev,
            enum can_state              *pcanstate
    );
    
int
rdc_candev_get_berr_counter(
            const struct net_device     *candev,
            struct can_berr_counter     *pcanberrcounter
    );




#endif


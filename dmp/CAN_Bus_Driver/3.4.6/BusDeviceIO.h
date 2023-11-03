#ifndef BusDeviceIO_H
#define BusDeviceIO_H

#define AddressSpace_Memory     0
#define AddressSpace_IO         1

typedef struct _MappedBusAddress
{
    ULONG               AddressSpace;       //0x0 indicates memory, 0x1 is I/O space
    PVOID               MappedAddress;  // Virtual Address or IO Address
    
}MappedBusAddress, *PMappedBusAddress;

FRSTATUS
BusDeviceIO_Read(
    IN      PMappedBusAddress   pMappedBusAddress,
    IN      ULONG               Offset,
    IN      ULONG               Size,   // 1(byte), 2(word), 4(dword)
    IN  OUT PULONG              pValue
    );
    
FRSTATUS
BusDeviceIO_Write(
    IN      PMappedBusAddress   pMappedBusAddress,
    IN      ULONG               Offset,
    IN      ULONG               Size,   // 1(byte), 2(word), 4(dword)
    IN      ULONG               Value
    );

#endif
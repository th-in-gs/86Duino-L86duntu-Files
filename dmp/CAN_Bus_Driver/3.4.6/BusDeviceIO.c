#include <linux/kernel.h>
#include <asm/io.h>

#include "SPtype.h"
#include "funcresult.h"

#include "BusDeviceIO.h"

FRSTATUS
BusDeviceIO_Read(
    IN      PMappedBusAddress   pMappedBusAddress,
    IN      ULONG               Offset,
    IN      ULONG               Size,   // 1(byte), 2(word), 4(dword)
    IN  OUT PULONG              pValue
    )
{
    FRSTATUS                    funcresult;
    //ULONG                     result;
    
    PVOID                       VirtalAddress;
    
    funcresult = FR_Success;
    
    VirtalAddress = (PUCHAR)pMappedBusAddress->MappedAddress + Offset;
    
    if (pMappedBusAddress->AddressSpace == AddressSpace_Memory)
    {   // memory space
        switch(Size)
        {
            case 1:
                {
                    //*pValue = READ_REGISTER_UCHAR(VirtalAddress);
                    *pValue = ioread8(VirtalAddress);
                }
                break;
            case 2:
                {
                    //*pValue = READ_REGISTER_USHORT(VirtalAddress);
                    *pValue = ioread16(VirtalAddress);
                }
                break;
            case 4:
                {
                    //*pValue = READ_REGISTER_ULONG(VirtalAddress);
                    *pValue = ioread32(VirtalAddress);
                }
                break;
            default:
                break;
        }
    }
    else
    {   // IO space
        switch(Size)
        {
            case 1:
                {
                    //*pValue = READ_PORT_UCHAR(VirtalAddress);
                    *pValue = inb((UINTPTR)VirtalAddress);
                }
                break;
            case 2:
                {
                    //*pValue = READ_PORT_USHORT(VirtalAddress);
                    *pValue = inw((UINTPTR)VirtalAddress);
                }
                break;
            case 4:
                {
                    //*pValue = READ_PORT_ULONG(VirtalAddress);
                    *pValue = inl((UINTPTR)VirtalAddress);
                }
                break;
            default:
                break;
        }
    }
    
//funcexit:
    
    return funcresult;
}


FRSTATUS
BusDeviceIO_Write(
    IN      PMappedBusAddress   pMappedBusAddress,
    IN      ULONG               Offset,
    IN      ULONG               Size,   // 1(byte), 2(word), 4(dword)
    IN      ULONG               Value
    )
{
    FRSTATUS                    funcresult;
    //ULONG                     result;
    
    PVOID                       VirtalAddress;
    
    funcresult = FR_Success;
    
    VirtalAddress = (PUCHAR)pMappedBusAddress->MappedAddress + Offset;
    
    if (pMappedBusAddress->AddressSpace == AddressSpace_Memory)
    {   // memory space
        switch(Size)
        {
            case 1:
                {
                    //WRITE_REGISTER_UCHAR(VirtalAddress, (UCHAR)Value);
                    iowrite8((UCHAR)Value, VirtalAddress);
                }
                break;
            case 2:
                {
                    //WRITE_REGISTER_USHORT(VirtalAddress, (USHORT)Value);
                    iowrite16((USHORT)Value, VirtalAddress);
                }
                break;
            case 4:
                {
                    //WRITE_REGISTER_ULONG(VirtalAddress, (ULONG)Value);
                    iowrite32((ULONG)Value, VirtalAddress);
                }
                break;
            default:
                
                break;
        }
    }
    else
    {   // IO space
        switch(Size)
        {
            case 1:
                {
                    //WRITE_PORT_UCHAR(VirtalAddress, (UCHAR)Value);
                    outb((UCHAR)Value, (UINTPTR)VirtalAddress);
                }
                break;
            case 2:
                {
                    //WRITE_PORT_USHORT(VirtalAddress, (USHORT)Value);
                    outw((USHORT)Value, (UINTPTR)VirtalAddress);
                }
                break;
            case 4:
                {
                    //WRITE_PORT_ULONG(VirtalAddress, (ULONG)Value);
                    outl((ULONG)Value, (UINTPTR)VirtalAddress);
                }
                break;
            default:
                
                break;
        }
    }
    
//funcexit:
    
    return funcresult;
}

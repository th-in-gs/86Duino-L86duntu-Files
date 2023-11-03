#ifndef HwdCAN_H
#define HwdCAN_H

#include "CANDef.h"


// Hwc: Hardware Core(Control)
//      this layer will check complex hardware behavior.
// Hwd: Hardware Driver Core(interface)
//      this layer will do a translation of parameters between app and hardware.
// Specific OS Driver Layer
//      this layer will implement the Asynchronous I/O Programming
//      between interrupt event and controller request.
// Library Layer
//      this layer will force on the control flow of the controller.


#define HwcCAN_MaxNumFilters 32  // depend on hardware specification.

//----
//  HwcCAN_func
//

// see CANDef.h
//typedef struct _CANGlobalConfigure HwcCANGlobalConfigure, *PHwcCANGlobalConfigure;

typedef struct _CANGlobalControl HwcCANGlobalControl, *PHwcCANGlobalControl;

typedef struct _CANInterrupt HwcCANInterrupt, *PHwcCANInterrupt;

typedef struct _CANInterrupt HwcCANInterruptStatus, *PHwcCANInterruptStatus;


//----
//  HwcCAN
//

#define HwcCAN_ErrorPassive    128  // depend on hardware specification.

typedef struct _HwcCAN
{
    PMappedBusAddress           hMappedRegisters;
    
    ULONG                       bByPass;
    
    ULONG                       bSilent;
    ULONG                       bTxRxPinLoopback;
    ULONG                       bTransmitWithNoACK;
    ULONG                       bSelfReception;
    ULONG                       TransmitPriorityMode;
    ULONG                       bArbitrationLostRetry;
    ULONG                       bBusErrorRetry;
    ULONG                       bPowerSaving;
    ULONG                       bRxBusError;
    
    
}HwcCAN, *PHwcCAN;

FRSTATUS
HwcCAN_Initialize(
    IN      PHwcCAN             hHwcCAN,
    IN      PMappedBusAddress   hMappedRegisters
    );
    
FRSTATUS
HwcCAN_InitHW(  // // off CAN & reset CAN
    IN      PHwcCAN             hHwcCAN
    );

FRSTATUS
HwcCAN_IFInitHW(
    IN      PHwcCAN             hHwcCAN
    );

FRSTATUS
HwcCAN_SetClockPrescaler(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               CKDIV,          // 0~0x3F
    IN      ULONG               bExternalInput
    );
    
FRSTATUS
HwcCAN_SetBusTiming(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               PropSEG,    // 0~7
    IN      ULONG               PSEG1,      // 0~7
    IN      ULONG               PSEG2,      // 0~7
    IN      ULONG               SJW,        // 0~3
    IN      ULONG               Sampling    // 0 or 1
    );

FRSTATUS
HwcCAN_SetGlobalControl(
    IN      PHwcCAN                 hHwcCAN,
    IN      PHwcCANGlobalControl    pHwcCANGlobalControl
    );
    
FRSTATUS
HwcCAN_GetGlobalControl(
    IN      PHwcCAN                 hHwcCAN,
        OUT PHwcCANGlobalControl    pHwcCANGlobalControl
    );

FRSTATUS
HwcCAN_SetInterruptEnable(
    IN      PHwcCAN             hHwcCAN,
    IN      PHwcCANInterrupt    pHwcCANInterrupt
    );

FRSTATUS
HwcCAN_GetInterruptEnable(
    IN      PHwcCAN             hHwcCAN,
        OUT PHwcCANInterrupt    pHwcCANInterrupt
    );

FRSTATUS
HwcCAN_GetInterruptStatus(
    IN      PHwcCAN                 hHwcCAN,
        OUT PHwcCANInterruptStatus  pHwcCANInterruptStatus,
        OUT PULONG                  pBusErrorDirection
    );

FRSTATUS
HwcCAN_ClearInterruptStatus(
    IN      PHwcCAN                 hHwcCAN,
    IN      PHwcCANInterruptStatus  pHwcCANInterruptStatus
    );
    
FRSTATUS
HwcCAN_GetGlobalStatus(
    IN      PHwcCAN             hHwcCAN,
        //OUT PULONG              pbControllerEnable,
        OUT PULONG              pbErrorCounterWarning,
        OUT PULONG              pbErrorPassive,
        OUT PULONG              pbBusOff
    );
    
FRSTATUS
HwcCAN_Start(
    IN      PHwcCAN             hHwcCAN
    );
    
FRSTATUS
HwcCAN_Stop(
    IN      PHwcCAN             hHwcCAN
    );

FRSTATUS
HwcCAN_IFStart(
    IN      PHwcCAN             hHwcCAN
    );

FRSTATUS
HwcCAN_IFStop(
    IN      PHwcCAN             hHwcCAN
    );

FRSTATUS
HwcCAN_IFEnableByPass(
    IN      PHwcCAN             hHwcCAN
    );
    
FRSTATUS
HwcCAN_IFDisableByPass(
    IN      PHwcCAN             hHwcCAN
    );

FRSTATUS
HwcCAN_IFSetFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex,    // 0 ~ 31
    IN      ULONG               bEnable,
    IN      ULONG               bExtendedID,
    IN      ULONG               ID,
    IN      ULONG               Mask
    );
    
FRSTATUS
HwcCAN_IFGetFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex,    // 0 ~ 31
        OUT PULONG              pbEnable,
        OUT PULONG              pbExtendedID,
        OUT PULONG              pID,
        OUT PULONG              pMask
    );
        
FRSTATUS
HwcCAN_IFEnableFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex
    );
    
FRSTATUS
HwcCAN_IFDisableFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex
    );
    
FRSTATUS
HwcCAN_IFRemoveAllFilter(
    IN      PHwcCAN             hHwcCAN
    );

FRSTATUS
HwcCAN_SetTxBuffer(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
    IN      PCANTxBuffer        pCANTxBuffer
    );

FRSTATUS
HwcCAN_GetTxBuffer(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
        OUT PCANTxBuffer        pCANTxBuffer
    );

FRSTATUS
HwcCAN_GetRxBuffer(
    IN      PHwcCAN             hHwcCAN,
        OUT PCANRxBuffer        pCANRxBuffer
    );


FRSTATUS
HwcCAN_GetTransmitStatus(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
        OUT PCANTransmitStatus  pCANTransmitStatus
    );

FRSTATUS
HwcCAN_GetReceiveStatus(
    IN      PHwcCAN             hHwcCAN,
        OUT PCANReceiveStatus   pCANReceiveStatus
    );
    
FRSTATUS
HwcCAN_TransmitRequest(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex
    );

FRSTATUS
HwcCAN_AbortRequest(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex
    );
    
FRSTATUS
HwcCAN_HaveRequestInQueue(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
        OUT PULONG              bHaveRequestInQueue
    );
    
FRSTATUS
HwcCAN_ReleaseReceivedMessage(
    IN      PHwcCAN             hHwcCAN
    );
    
FRSTATUS
HwcCAN_SetErrorWarningLimit(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               ErrorWarningLimit
    );
    
FRSTATUS
HwcCAN_GetErrorWarningLimit(
    IN      PHwcCAN             hHwcCAN,
        OUT PULONG              pErrorWarningLimit
    );

// if setting the ErrorCounter to 0x100, the device goes into the bus off state.
FRSTATUS
HwcCAN_SetErrorCounter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxErrorCounter,
    IN      ULONG               RxErrorCounter
    );

// at bus off state, the ErrorCounter is 0x100.
FRSTATUS
HwcCAN_GetErrorCounter(
    IN      PHwcCAN             hHwcCAN,
        OUT PULONG              pTxErrorCounter,
        OUT PULONG              pRxErrorCounter
    );

//----
//  CANMessage
//

// see MyCANDef.h


//----
//  CANFilter
//

// see MyCANDef.h


// Hwd: Hardware Driver Core(interface)
// in this level, we dont handle synchonizaiton between IO action and interrupt.
// the ioctl level will handle this. like waiting for interrut event.

//----
//  HwdCAN_func
//

// see CANDef.h
//typedef struct _CANGlobalConfigure HwcCANGlobalConfigure, *PHwcCANGlobalConfigure;

//typedef struct _CANGlobalControl HwcCANGlobalControl, *PHwcCANGlobalControl;

typedef struct _CANInterrupt HwdCANInterrupt, *PHwdCANInterrupt;

typedef struct _CANVolatileInfo HwdCANVolatileInfo, *PHwdCANVolatileInfo;


//----
//  HwdCAN
//  this is HCDI for CAN Driver
//

#define HwdCAN_MaxCANRxMessages     20


typedef struct _HwdCAN
{
    // System Bus Info
    ULONG                       BusNumber;
    ULONG                       DeviceNumber;
    ULONG                       FunctionNumber;
    
    ULONG                       PortID;
    
    // Hardware Interface
    //Base0Address.
    MappedBusAddress            sMappedRegisters0;
    PMappedBusAddress           hMappedRegisters0;
    
    //Base0Address.
    MappedBusAddress            sMappedRegisters1;
    PMappedBusAddress           hMappedRegisters1;
    
    ULONG                       UseIOSpace;
    PMappedBusAddress           hMappedRegisters;    // default mapped register.
    
    // Hardware Core(Control)
    HwcCAN                     sHwcCAN;
    PHwcCAN                    hHwcCAN;
    
    // Driver Core Data
    CANMessage                  CANRxMessages[HwdCAN_MaxCANRxMessages];
    ULONG                       CANRxMessages_Start;       // alter by DPC or Thread
    ULONG                       CANRxMessages_End;         // alter by DPC or Thread
    ULONG                       CANRxMessages_Overrun;
    
    ULONG                       BusOffHandleMethod;
        // 0 : the driver dont handle it.
        // 1 : the driver handle it according to the formal method.
        // 2 : the driver handle it according to the speedup method - to return Error Active
        // 3 : the driver handle it according to the speedup method - to return Error Passive
    
    // Synchronization
    // use an interrupt default spin lock to access share resource.
        // EvtInterruptSynchronize for large code segment.
        // WdfInterruptAcquireLock and WdfInterruptReleaseLock for a few lines.
    // use SpinLock if Task and DPC will access share resource.
    // use Semaphore if Task and Task will access share resource.
    
    // lock priority rule: Semaphore -> SpinLock -> interrupt
    // Each routine that holds an interrupt spin lock executes at the DIRQL must not call
    // KeAcquireSpinLock and KeReleaseSpinLock nor
    // any other routine that uses an executive spin lock from its ISR or SynchCritSection routines.
    // Such a call is an error that can cause a system deadlock. So we need a lock priority rule.
    
    //SPInterruptLock             sInterruptLock;
    //SPSpinLock                  sSPSpinLockDPC;
    //SPSemaphore                 sSPSemaphoreTask;
    
    SPSpinLock                  sSPSpinLockCANRxMessages;
        // to handle CANRxMessages, (i have moved the code to DPC, so we may use SPSemaphore to replace this)
    
    SPSemaphore                 sSPSemaphoreDPC;
        // to handel BusOffHandleMethod
    
    //debug
    CANRxBuffer                 sPreviousRxBuffer;
    
}HwdCAN, *PHwdCAN;


FRSTATUS
HwdCAN_Initialize(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               UseIOSpace
    );

// hardware resource open & close 

FRSTATUS
HwdCAN_OpenDevice(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               BusNumber,
    IN      ULONG               DeviceNumber,
    IN      ULONG               FunctionNumber,
    IN      ULONG               PortID
    );

FRSTATUS
HwdCAN_BaseAddress(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               Base0Space,
    IN      PHYSICAL_ADDRESS    Base0Address,
    IN      ULONG               Base0Length,
    IN      ULONG               Base1Space,
    IN      PHYSICAL_ADDRESS    Base1Address,
    IN      ULONG               Base1Length
    );
    
FRSTATUS
HwdCAN_CloseDevice(
    IN      PHwdCAN      hHwdCAN
    );

// hardware initialization

FRSTATUS
HwdCAN_InitHW(
    IN      PHwdCAN      hHwdCAN
    );

// interrupt ISR and DCPForISR

FRSTATUS
HwdCAN_CheckInterrupt(
    IN      PHwdCAN                 hHwdCAN,
        OUT PULONG                  pbInterrupt
    );

FRSTATUS
HwdCAN_ISR(
    IN      PHwdCAN                 hHwdCAN,
        OUT PULONG                  pbInterrupt,
        OUT PHwdCANInterrupt        pHwdCANInterrupt,
        OUT PHwdCANVolatileInfo     pHwdCANVolatileInfo
    );

FRSTATUS
HwdCAN_DCPForISR(
    IN      PHwdCAN                 hHwdCAN,
    IN      PHwdCANInterrupt        pHwdCANInterrupt,
    IN      PHwdCANVolatileInfo     pHwdCANVolatileInfo
    );

// interrupt controls
FRSTATUS
HwdCAN_EnableInterrupt(
    IN      PHwdCAN             hHwdCAN,
    IN      PHwdCANInterrupt    pHwdCANInterrupt // TRUE to Enable
    );
    
FRSTATUS
HwdCAN_DisableInterrupt(
    IN      PHwdCAN             hHwdCAN,
    IN      PHwdCANInterrupt    pHwdCANInterrupt // TRUE to Disable
    );
    
FRSTATUS
HwdCAN_GetInterruptEnable(
    IN      PHwdCAN             hHwdCAN,
        OUT PHwdCANInterrupt    pHwdCANInterrupt
    );

// configuration

FRSTATUS
HwdCAN_SetClockPrescaler(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               CKDIV,          // 0~0x3F
    IN      ULONG               bExternalInput
    );

FRSTATUS
HwdCAN_SetBusTiming(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               PropSEG,    // 0~7
    IN      ULONG               PSEG1,      // 0~7
    IN      ULONG               PSEG2,      // 0~7
    IN      ULONG               SJW,        // 0~3
    IN      ULONG               Sampling    // 0 or 1
    );

FRSTATUS
HwdCAN_SetGlobalControl(
    IN      PHwdCAN             hHwdCAN,
    IN      PCANGlobalControl   pCANGlobalControl
    );
    
FRSTATUS
HwdCAN_GetGlobalControl(
    IN      PHwdCAN             hHwdCAN,
        OUT PCANGlobalControl   pCANGlobalControl
    );
    
FRSTATUS
HwdCAN_SetBusOffHandle(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               BusOffHandleMethod
    );

// control function
    
/*FRSTATUS
HwdCAN_Start(
    IN      PHwdCAN      hHwdCAN
    );

FRSTATUS
HwdCAN_Stop(
    IN      PHwdCAN      hHwdCAN
    );*/
    
FRSTATUS
HwdCAN_ResetCAN(
    IN      PHwdCAN             hHwdCAN
    );
    
FRSTATUS
HwdCAN_StartCAN(
    IN      PHwdCAN             hHwdCAN
    );
    
FRSTATUS
HwdCAN_StopCAN(
    IN      PHwdCAN             hHwdCAN
    );

FRSTATUS
HwdCAN_ResetCANIF(
    IN      PHwdCAN             hHwdCAN
    );
    
FRSTATUS
HwdCAN_StartCANIF(
    IN      PHwdCAN             hHwdCAN
    );
    
FRSTATUS
HwdCAN_StopCANIF(
    IN      PHwdCAN             hHwdCAN
    );

FRSTATUS
HwdCAN_EnableCANIFByPass(
    IN      PHwdCAN             hHwdCAN
    );
    
FRSTATUS
HwdCAN_DisableCANIFByPass(
    IN      PHwdCAN             hHwdCAN
    );
    
FRSTATUS
HwdCAN_GetGlobalStatus(
    IN      PHwdCAN             hHwdCAN,
        //OUT PULONG              pbControllerEnable,
        OUT PULONG              pbErrorCounterWarning,
        OUT PULONG              pbErrorPassive,
        OUT PULONG              pbBusOff
    );

FRSTATUS
HwdCAN_SetFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex,
    IN      PCANFilter          pCANFilter
    );
    
FRSTATUS
HwdCAN_GetFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex,
        OUT PCANFilter          pCANFilter
    );
    
FRSTATUS
HwdCAN_EnableFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex
    );
    
FRSTATUS
HwdCAN_DisableFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex
    );
    
FRSTATUS
HwdCAN_RemoveAllFilter(
    IN      PHwdCAN             hHwdCAN
    );

FRSTATUS
HwdCAN_SendMessage(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
    IN      PCANMessage         pCANMessage
    );

FRSTATUS
HwdCAN_SendTxBuffer(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
    IN      PCANTxBuffer        pCANTxBuffer
    );
    
FRSTATUS
HwdCAN_AbortMessage(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
    IN      PULONG              pbAbort
    );
    
FRSTATUS
HwdCAN_GetTransmitStatus(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
        OUT PCANTransmitStatus  pCANTransmitStatus
    );

FRSTATUS
HwdCAN_HaveAnyRxMessage(
    IN      PHwdCAN             hHwdCAN,
        OUT PULONG              pHaveAnyRxMessage
    );

FRSTATUS
HwdCAN_GetRxMessage(
    IN      PHwdCAN             hHwdCAN,
        OUT PULONG              pNoRxMessage,
        OUT PCANMessage         pCANMessage
    );
    
FRSTATUS
HwdCAN_HaveRxFIFOOverrun(
    IN      PHwdCAN             hHwdCAN,
        OUT PULONG              pHaveRxFIFOOverrun
    );
    
FRSTATUS
HwdCAN_ClearRxFIFO(
    IN      PHwdCAN             hHwdCAN
    );

FRSTATUS
HwdCAN_RoundRobinMessage(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               Tx0Enable,
    IN      ULONG               Tx1Enable,
    IN      ULONG               Tx2Enable,
    IN      PCANMessage         losCANMessage
    );
    
FRSTATUS
HwdCAN_SetHwiReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
    IN      PVOID               pHwiReg,
    IN      ULONG               SizeOfHwiReg
    );
    
FRSTATUS
HwdCAN_GetHwiReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
        OUT PVOID               pHwiReg,
    IN      ULONG               SizeOfHwiReg
    );

FRSTATUS
HwdCAN_SetReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
    IN      ULONG               Value
    );
    
FRSTATUS
HwdCAN_GetReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
        OUT PULONG              pValue
    );
        
#endif

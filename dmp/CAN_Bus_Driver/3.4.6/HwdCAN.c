#include <linux/kernel.h>

#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/completion.h>

#include <linux/time.h>

#include "SPtype.h"
#include "funcresult.h"

#include "DrvDebug.h"
#include "SystemPorting.h"
#include "BusDeviceIO.h"

#include "HwiCAN.h"
#include "HwdCAN.h"

// in Hwc level, we dont check Hwi func.






FRSTATUS
HwcCAN_Initialize(
    IN      PHwcCAN             hHwcCAN,
    IN      PMappedBusAddress   hMappedRegisters
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    hHwcCAN->hMappedRegisters = hMappedRegisters;
    
    hHwcCAN->bSilent = FALSE;
    
    hHwcCAN->bSilent = FALSE;
    hHwcCAN->bTxRxPinLoopback = FALSE;
    hHwcCAN->bTransmitWithNoACK = FALSE;
    hHwcCAN->bSelfReception = FALSE;
    hHwcCAN->TransmitPriorityMode = 0;
    hHwcCAN->bPowerSaving = FALSE;
    hHwcCAN->bRxBusError = FALSE;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_InitHW(  // // off CAN & reset CAN
    IN      PHwcCAN             hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    hHwcCAN->bSilent = FALSE;
    hHwcCAN->bTxRxPinLoopback = FALSE;
    hHwcCAN->bTransmitWithNoACK = FALSE;
    hHwcCAN->bSelfReception = FALSE;
    hHwcCAN->TransmitPriorityMode = 0;
    hHwcCAN->bPowerSaving = FALSE;
    hHwcCAN->bRxBusError = FALSE;
    
    // reset CAN
    // trun off CAN and init
    
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    sHwiGlobalControlReg.Reset = 1;
    //sHwiGlobalControlReg.Active = 0;
    //sHwiGlobalControlReg.Silent = 0;
    //sHwiGlobalControlReg.Loopback = 0;
    //sHwiGlobalControlReg.TransmitWithNoAck = 0;
    //sHwiGlobalControlReg.SelfReception = 0;
    //sHwiGlobalControlReg.TransmitPriority = 0;
    //sHwiGlobalControlReg.PowerSaving = 0;
    //sHwiGlobalControlReg.RxBusError = 0;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiGlobalControlReg(
            hHwcCAN->hMappedRegisters,
            &sHwiGlobalControlReg
            );
        if (sHwiGlobalControlReg.Reset == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFInitHW(
    IN      PHwcCAN             hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    hHwcCAN->bByPass = FALSE;
    
    // reset CANIF
    // trun off CANIF and init Filter
    
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    sHwiGlobalControlReg.IdentifierReset = 1;
    sHwiGlobalControlReg.IdentifierEnable = 0;
    sHwiGlobalControlReg.IdentifierBypass = 0;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiGlobalControlReg(
            hHwcCAN->hMappedRegisters,
            &sHwiGlobalControlReg
            );
        if (sHwiGlobalControlReg.IdentifierReset == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_SetClockPrescaler(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               CKDIV,          // 0~0x3F
    IN      ULONG               bExternalInput
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiClockPrescalerReg        sHwiClockPrescalerReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiClockPrescalerReg, sizeof(HwiClockPrescalerReg));
    
    sHwiClockPrescalerReg.CKDIV = CKDIV;
    sHwiClockPrescalerReg.ExternalInput = bExternalInput;
    
    _SetHwiClockPrescalerReg(
        hHwcCAN->hMappedRegisters,
        &sHwiClockPrescalerReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_SetBusTiming(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               PropSEG,    // 0~7
    IN      ULONG               PSEG1,      // 0~7
    IN      ULONG               PSEG2,      // 0~7
    IN      ULONG               SJW,        // 0~3
    IN      ULONG               Sampling    // 0 or 1
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiBusTimingReg             sHwiBusTimingReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiBusTimingReg, sizeof(HwiBusTimingReg));
    
    sHwiBusTimingReg.PropagationSEG = PropSEG;
    sHwiBusTimingReg.PhaseSEG1 = PSEG1;
    sHwiBusTimingReg.PhaseSEG2 = PSEG2;
    sHwiBusTimingReg.SJW = SJW;
    sHwiBusTimingReg.Sampling = Sampling;
    
    _SetHwiBusTimingReg(
        hHwcCAN->hMappedRegisters,
        &sHwiBusTimingReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_SetGlobalControl(
    IN      PHwcCAN                 hHwcCAN,
    IN      PHwcCANGlobalControl    pHwcCANGlobalControl
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    hHwcCAN->bSilent = pHwcCANGlobalControl->bSilent;
    hHwcCAN->bTxRxPinLoopback = pHwcCANGlobalControl->bTxRxPinLoopback;
    hHwcCAN->bTransmitWithNoACK = pHwcCANGlobalControl->bTransmitWithNoACK;
    hHwcCAN->bSelfReception = pHwcCANGlobalControl->bSelfReception;
    hHwcCAN->TransmitPriorityMode = pHwcCANGlobalControl->TransmitPriorityMode;
    hHwcCAN->bArbitrationLostRetry = pHwcCANGlobalControl->bArbitrationLostRetry;
    hHwcCAN->bBusErrorRetry = pHwcCANGlobalControl->bBusErrorRetry;
    hHwcCAN->bPowerSaving = pHwcCANGlobalControl->bPowerSaving;
    
    sHwiGlobalControlReg.Silent = pHwcCANGlobalControl->bSilent;
    sHwiGlobalControlReg.Loopback = pHwcCANGlobalControl->bTxRxPinLoopback;
    sHwiGlobalControlReg.TransmitWithNoAck = pHwcCANGlobalControl->bTransmitWithNoACK;
    sHwiGlobalControlReg.SelfReception = pHwcCANGlobalControl->bSelfReception;
    sHwiGlobalControlReg.TransmitPriority = pHwcCANGlobalControl->TransmitPriorityMode;
    sHwiGlobalControlReg.ArbitrationLostRetry = pHwcCANGlobalControl->bArbitrationLostRetry;
    sHwiGlobalControlReg.BusErrorRetry = pHwcCANGlobalControl->bBusErrorRetry;
    sHwiGlobalControlReg.PowerSaving = pHwcCANGlobalControl->bPowerSaving;
    sHwiGlobalControlReg.RxBusError = pHwcCANGlobalControl->bRxBusError;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetGlobalControl(
    IN      PHwcCAN                 hHwcCAN,
        OUT PHwcCANGlobalControl    pHwcCANGlobalControl
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    pHwcCANGlobalControl->bSilent = hHwcCAN->bSilent;
    pHwcCANGlobalControl->bTransmitWithNoACK = hHwcCAN->bTransmitWithNoACK;
    pHwcCANGlobalControl->bTxRxPinLoopback = hHwcCAN->bTxRxPinLoopback;
    pHwcCANGlobalControl->bSelfReception = hHwcCAN->bSelfReception;
    pHwcCANGlobalControl->TransmitPriorityMode = hHwcCAN->TransmitPriorityMode;
    pHwcCANGlobalControl->bArbitrationLostRetry = hHwcCAN->bArbitrationLostRetry;
    pHwcCANGlobalControl->bBusErrorRetry = hHwcCAN->bBusErrorRetry;
    pHwcCANGlobalControl->bPowerSaving = hHwcCAN->bPowerSaving;
    pHwcCANGlobalControl->bRxBusError = hHwcCAN->bRxBusError;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_SetInterruptEnable(
    IN      PHwcCAN             hHwcCAN,
    IN      PHwcCANInterrupt    pHwcCANInterrupt
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiInterruptEnableReg       sHwiInterruptEnableReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiInterruptEnableReg, sizeof(HwiInterruptEnableReg));
    
    sHwiInterruptEnableReg.Receive = pHwcCANInterrupt->Receive;
    sHwiInterruptEnableReg.TxBuffer0Transmit = pHwcCANInterrupt->TxBuffer0Transmit;
    sHwiInterruptEnableReg.TxBuffer1Transmit = pHwcCANInterrupt->TxBuffer1Transmit;
    sHwiInterruptEnableReg.TxBuffer2Transmit = pHwcCANInterrupt->TxBuffer2Transmit;
    sHwiInterruptEnableReg.ErrorWarning = pHwcCANInterrupt->ErrorWarning;
    sHwiInterruptEnableReg.ErrorPassive = pHwcCANInterrupt->ErrorPassive;
    sHwiInterruptEnableReg.BusOff = pHwcCANInterrupt->BusOff;
    sHwiInterruptEnableReg.ArbitrationLost = pHwcCANInterrupt->ArbitrationLost;
    sHwiInterruptEnableReg.RxBusError = pHwcCANInterrupt->RxBusError;
    sHwiInterruptEnableReg.RxFIFOOverrun = pHwcCANInterrupt->RxFIFOOverrun;
    sHwiInterruptEnableReg.PowerSavingExit = pHwcCANInterrupt->PowerSavingExit;
    
    _SetHwiInterruptEnableReg(
        hHwcCAN->hMappedRegisters,
        &sHwiInterruptEnableReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetInterruptEnable(
    IN      PHwcCAN             hHwcCAN,
        OUT PHwcCANInterrupt    pHwcCANInterrupt
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiInterruptEnableReg       sHwiInterruptEnableReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiInterruptEnableReg, sizeof(HwiInterruptEnableReg));
    _GetHwiInterruptEnableReg(
        hHwcCAN->hMappedRegisters,
        &sHwiInterruptEnableReg
        );
    
    pHwcCANInterrupt->Receive = sHwiInterruptEnableReg.Receive;
    pHwcCANInterrupt->TxBuffer0Transmit = sHwiInterruptEnableReg.TxBuffer0Transmit;
    pHwcCANInterrupt->TxBuffer1Transmit = sHwiInterruptEnableReg.TxBuffer1Transmit;
    pHwcCANInterrupt->TxBuffer2Transmit = sHwiInterruptEnableReg.TxBuffer2Transmit;
    pHwcCANInterrupt->ErrorWarning = sHwiInterruptEnableReg.ErrorWarning;
    pHwcCANInterrupt->ErrorPassive = sHwiInterruptEnableReg.ErrorPassive;
    pHwcCANInterrupt->BusOff = sHwiInterruptEnableReg.BusOff;
    pHwcCANInterrupt->ArbitrationLost = sHwiInterruptEnableReg.ArbitrationLost;
    pHwcCANInterrupt->RxBusError = sHwiInterruptEnableReg.RxBusError;
    pHwcCANInterrupt->RxFIFOOverrun = sHwiInterruptEnableReg.RxFIFOOverrun;
    pHwcCANInterrupt->PowerSavingExit = sHwiInterruptEnableReg.PowerSavingExit;
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetInterruptStatus(
    IN      PHwcCAN                 hHwcCAN,
        OUT PHwcCANInterruptStatus  pHwcCANInterruptStatus,
        OUT PULONG                  pBusErrorDirection
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiInterruptStatusReg       sHwiInterruptStatusReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiInterruptStatusReg, sizeof(HwiInterruptStatusReg));
    _GetHwiInterruptStatusReg(
        hHwcCAN->hMappedRegisters,
        &sHwiInterruptStatusReg
        );
    
    pHwcCANInterruptStatus->Receive = sHwiInterruptStatusReg.Receive;
    pHwcCANInterruptStatus->TxBuffer0Transmit = sHwiInterruptStatusReg.TxBuffer0Transmit;
    pHwcCANInterruptStatus->TxBuffer1Transmit = sHwiInterruptStatusReg.TxBuffer1Transmit;
    pHwcCANInterruptStatus->TxBuffer2Transmit = sHwiInterruptStatusReg.TxBuffer2Transmit;
    pHwcCANInterruptStatus->ErrorWarning = sHwiInterruptStatusReg.ErrorWarning;
    pHwcCANInterruptStatus->ErrorPassive = sHwiInterruptStatusReg.ErrorPassive;
    pHwcCANInterruptStatus->BusOff = sHwiInterruptStatusReg.BusOff;
    pHwcCANInterruptStatus->ArbitrationLost = sHwiInterruptStatusReg.ArbitrationLost;
    pHwcCANInterruptStatus->RxBusError = sHwiInterruptStatusReg.RxBusError;
    pHwcCANInterruptStatus->RxFIFOOverrun = sHwiInterruptStatusReg.RxFIFOOverrun;
    pHwcCANInterruptStatus->PowerSavingExit = sHwiInterruptStatusReg.PowerSavingExit;
    
    *pBusErrorDirection = sHwiInterruptStatusReg.BusErrorDirection;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_ClearInterruptStatus(
    IN      PHwcCAN                 hHwcCAN,
    IN      PHwcCANInterruptStatus  pHwcCANInterruptStatus
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiInterruptStatusReg       sHwiInterruptStatusReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiInterruptStatusReg, sizeof(HwiInterruptStatusReg));
    
    
    sHwiInterruptStatusReg.Receive = pHwcCANInterruptStatus->Receive;
    sHwiInterruptStatusReg.TxBuffer0Transmit = pHwcCANInterruptStatus->TxBuffer0Transmit;
    sHwiInterruptStatusReg.TxBuffer1Transmit = pHwcCANInterruptStatus->TxBuffer1Transmit;
    sHwiInterruptStatusReg.TxBuffer2Transmit = pHwcCANInterruptStatus->TxBuffer2Transmit;
    sHwiInterruptStatusReg.ErrorWarning = pHwcCANInterruptStatus->ErrorWarning;
    sHwiInterruptStatusReg.ErrorPassive = pHwcCANInterruptStatus->ErrorPassive;
    sHwiInterruptStatusReg.BusOff = pHwcCANInterruptStatus->BusOff;
    sHwiInterruptStatusReg.ArbitrationLost = pHwcCANInterruptStatus->ArbitrationLost;
    sHwiInterruptStatusReg.RxBusError = pHwcCANInterruptStatus->RxBusError;
    sHwiInterruptStatusReg.RxFIFOOverrun = pHwcCANInterruptStatus->RxFIFOOverrun;
    sHwiInterruptStatusReg.PowerSavingExit = pHwcCANInterruptStatus->PowerSavingExit;
    
    
    
    _SetHwiInterruptStatusReg(
        hHwcCAN->hMappedRegisters,
        &sHwiInterruptStatusReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetGlobalStatus(
    IN      PHwcCAN            hHwcCAN,
        //OUT PULONG              pbControllerEnable,
        OUT PULONG              pbErrorCounterWarning,
        OUT PULONG              pbErrorPassive,
        OUT PULONG              pbBusOff
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalStatusReg          sHwiGlobalStatusReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiGlobalStatusReg, sizeof(HwiGlobalStatusReg));
    _GetHwiGlobalStatusReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalStatusReg
        );
    *pbErrorCounterWarning = sHwiGlobalStatusReg.ErrorCounterWarning;
    
    *pbErrorPassive = sHwiGlobalStatusReg.ErrorPassiveState;
    
    *pbBusOff = sHwiGlobalStatusReg.BusOff;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_Start(
    IN      PHwcCAN            hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    sHwiGlobalControlReg.Active = 1;
    sHwiGlobalControlReg.Silent = hHwcCAN->bSilent;
    sHwiGlobalControlReg.Loopback = hHwcCAN->bTxRxPinLoopback;
    sHwiGlobalControlReg.TransmitWithNoAck = hHwcCAN->bTransmitWithNoACK;
    sHwiGlobalControlReg.SelfReception = hHwcCAN->bSelfReception;
    sHwiGlobalControlReg.TransmitPriority = hHwcCAN->TransmitPriorityMode;
    sHwiGlobalControlReg.PowerSaving = hHwcCAN->bPowerSaving;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiGlobalControlReg(
            hHwcCAN->hMappedRegisters,
            &sHwiGlobalControlReg
            );
        if (sHwiGlobalControlReg.Active == 1)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_Stop(
    IN      PHwcCAN            hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    sHwiGlobalControlReg.Active = 0;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiGlobalControlReg(
            hHwcCAN->hMappedRegisters,
            &sHwiGlobalControlReg
            );
        if (sHwiGlobalControlReg.Active == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFStart(
    IN      PHwcCAN          hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    // trun on CANIF
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    sHwiGlobalControlReg.IdentifierEnable = 1;
    sHwiGlobalControlReg.IdentifierBypass = hHwcCAN->bByPass;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiGlobalControlReg(
            hHwcCAN->hMappedRegisters,
            &sHwiGlobalControlReg
            );
        if (sHwiGlobalControlReg.IdentifierEnable == 1)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFStop(
    IN      PHwcCAN          hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    // trun on CANIF
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    sHwiGlobalControlReg.IdentifierEnable = 0;
    sHwiGlobalControlReg.IdentifierBypass = hHwcCAN->bByPass;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiGlobalControlReg(
            hHwcCAN->hMappedRegisters,
            &sHwiGlobalControlReg
            );
        if (sHwiGlobalControlReg.IdentifierEnable == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFEnableByPass(
    IN      PHwcCAN          hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    // trun on CANIF
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    hHwcCAN->bByPass = TRUE;
    
    sHwiGlobalControlReg.IdentifierBypass = 1;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFDisableByPass(
    IN      PHwcCAN          hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    // trun on CANIF
    SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
    _GetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    hHwcCAN->bByPass = FALSE;
    
    sHwiGlobalControlReg.IdentifierBypass = 0;
    
    _SetHwiGlobalControlReg(
        hHwcCAN->hMappedRegisters,
        &sHwiGlobalControlReg
        );
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFSetFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex,    // 0 ~ 31
    IN      ULONG               bEnable,
    IN      ULONG               bExtendedID,
    IN      ULONG               ID,
    IN      ULONG               Mask
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiIdentifierIndexReg       sHwiIdentifierIndexReg;
    HwiIdentifierFilterReg      sHwiIdentifierFilterReg;
    HwiIdentifierMaskReg        sHwiIdentifierMaskReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    // select filter
    SPZeroMemory(&sHwiIdentifierIndexReg, sizeof(HwiIdentifierIndexReg));
    sHwiIdentifierIndexReg.FilterIndex = FilterIndex;
    sHwiIdentifierIndexReg.FilterUpdate = 0;
    _SetHwiIdentifierIndexReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierIndexReg
        );
    
    // set ID and mask
    SPZeroMemory(&sHwiIdentifierFilterReg, sizeof(HwiIdentifierFilterReg));
    sHwiIdentifierFilterReg.ID = ID;
    sHwiIdentifierFilterReg.ExtendedID = bExtendedID;
    sHwiIdentifierFilterReg.Enable = bEnable;
    _SetHwiIdentifierFilterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierFilterReg
        );
    
    SPZeroMemory(&sHwiIdentifierMaskReg, sizeof(HwiIdentifierMaskReg));
    sHwiIdentifierMaskReg.Mask = Mask;
    _SetHwiIdentifierMaskReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierMaskReg
        );
    
    // update filter
    sHwiIdentifierIndexReg.FilterUpdate = 1;
    _SetHwiIdentifierIndexReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierIndexReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiIdentifierIndexReg(
            hHwcCAN->hMappedRegisters,
            &sHwiIdentifierIndexReg
            );
        if (sHwiIdentifierIndexReg.FilterUpdate == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFGetFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex,    // 0 ~ 31
        OUT PULONG              pbEnable,
        OUT PULONG              pbExtendedID,
        OUT PULONG              pID,
        OUT PULONG              pMask
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiIdentifierIndexReg       sHwiIdentifierIndexReg;
    HwiIdentifierFilterReg      sHwiIdentifierFilterReg;
    HwiIdentifierMaskReg        sHwiIdentifierMaskReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    // select filter
    SPZeroMemory(&sHwiIdentifierIndexReg, sizeof(HwiIdentifierIndexReg));
    sHwiIdentifierIndexReg.FilterIndex = FilterIndex;
    sHwiIdentifierIndexReg.FilterUpdate = 0;
    _SetHwiIdentifierIndexReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierIndexReg
        );
    
    // get ID and mask
    SPZeroMemory(&sHwiIdentifierFilterReg, sizeof(HwiIdentifierFilterReg));
    _GetHwiIdentifierFilterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierFilterReg
        );
    
    *pID = sHwiIdentifierFilterReg.ID;
    *pbExtendedID = sHwiIdentifierFilterReg.ExtendedID;
    *pbEnable = sHwiIdentifierFilterReg.Enable;
    
    SPZeroMemory(&sHwiIdentifierMaskReg, sizeof(HwiIdentifierMaskReg));
    _GetHwiIdentifierMaskReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierMaskReg
        );
    *pMask = sHwiIdentifierMaskReg.Mask;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFEnableFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiIdentifierIndexReg       sHwiIdentifierIndexReg;
    HwiIdentifierFilterReg      sHwiIdentifierFilterReg;
    //HwiIdentifierMaskReg        sHwiIdentifierMaskReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    // select filter
    SPZeroMemory(&sHwiIdentifierIndexReg, sizeof(HwiIdentifierIndexReg));
    sHwiIdentifierIndexReg.FilterIndex = FilterIndex;
    sHwiIdentifierIndexReg.FilterUpdate = 0;
    _SetHwiIdentifierIndexReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierIndexReg
        );
    
    // set enable bit
    SPZeroMemory(&sHwiIdentifierFilterReg, sizeof(HwiIdentifierFilterReg));
    _GetHwiIdentifierFilterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierFilterReg
        );
    
    sHwiIdentifierFilterReg.Enable = 1;
    _SetHwiIdentifierFilterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierFilterReg
        );
    
    // update filter
    sHwiIdentifierIndexReg.FilterUpdate = 1;
    _SetHwiIdentifierIndexReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierIndexReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiIdentifierIndexReg(
            hHwcCAN->hMappedRegisters,
            &sHwiIdentifierIndexReg
            );
        if (sHwiIdentifierIndexReg.FilterUpdate == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFDisableFilter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               FilterIndex
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiIdentifierIndexReg        sHwiIdentifierIndexReg;
    HwiIdentifierFilterReg       sHwiIdentifierFilterReg;
    //HwiIdentifierMaskReg         sHwiIdentifierMaskReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    // select filter
    SPZeroMemory(&sHwiIdentifierIndexReg, sizeof(HwiIdentifierIndexReg));
    sHwiIdentifierIndexReg.FilterIndex = FilterIndex;
    sHwiIdentifierIndexReg.FilterUpdate = 0;
    _SetHwiIdentifierIndexReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierIndexReg
        );
    
    // set enable bit
    SPZeroMemory(&sHwiIdentifierFilterReg, sizeof(HwiIdentifierFilterReg));
    _GetHwiIdentifierFilterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierFilterReg
        );
    
    sHwiIdentifierFilterReg.Enable = 0;
    _SetHwiIdentifierFilterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierFilterReg
        );
    
    // update filter
    sHwiIdentifierIndexReg.FilterUpdate = 1;
    _SetHwiIdentifierIndexReg(
        hHwcCAN->hMappedRegisters,
        &sHwiIdentifierIndexReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiIdentifierIndexReg(
            hHwcCAN->hMappedRegisters,
            &sHwiIdentifierIndexReg
            );
        if (sHwiIdentifierIndexReg.FilterUpdate == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_IFRemoveAllFilter(
    IN      PHwcCAN          hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       i;
    
    HwiIdentifierIndexReg       sHwiIdentifierIndexReg;
    HwiIdentifierFilterReg      sHwiIdentifierFilterReg;
    HwiIdentifierMaskReg        sHwiIdentifierMaskReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    for (i = 0; i < HwcCAN_MaxNumFilters; i++)
    {
        // the following code may be replaced by identifier reset.
        
        // select filter
        SPZeroMemory(&sHwiIdentifierIndexReg, sizeof(HwiIdentifierIndexReg));
        sHwiIdentifierIndexReg.FilterIndex = i;
        sHwiIdentifierIndexReg.FilterUpdate = 0;
        _SetHwiIdentifierIndexReg(
            hHwcCAN->hMappedRegisters,
            &sHwiIdentifierIndexReg
            );
        
        // zero ID and Mask
        SPZeroMemory(&sHwiIdentifierFilterReg, sizeof(HwiIdentifierFilterReg));
        sHwiIdentifierFilterReg.Enable = 0;
        _SetHwiIdentifierFilterReg(
            hHwcCAN->hMappedRegisters,
            &sHwiIdentifierFilterReg
            );
        
        SPZeroMemory(&sHwiIdentifierMaskReg, sizeof(HwiIdentifierMaskReg));
        sHwiIdentifierMaskReg.Mask = 0;
        _SetHwiIdentifierMaskReg(
            hHwcCAN->hMappedRegisters,
            &sHwiIdentifierMaskReg
            );
        
        // update filter
        sHwiIdentifierIndexReg.FilterUpdate = 1;
        _SetHwiIdentifierIndexReg(
            hHwcCAN->hMappedRegisters,
            &sHwiIdentifierIndexReg
            );
        
        // wait for the action done.
        SPTime_StartTime(&sSPTime);
        while(1)
        {
            _GetHwiIdentifierIndexReg(
                hHwcCAN->hMappedRegisters,
                &sHwiIdentifierIndexReg
                );
            if (sHwiIdentifierIndexReg.FilterUpdate == 0)
            {
                break;
            }
            
            SPTime_EndTime(&sSPTime);
            ms = 0;
            SPTime_EscapeTime(&sSPTime, &ms);
            if (ms >= 1000) // 1000 ms
            {
                funcresult = FR_Error;
                break;
            }
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_SetTxBuffer(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
    IN      PCANTxBuffer        pCANTxBuffer
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiTFIReg                   sHwiTFIReg;
    HwiTIDReg                   sHwiTIDReg;
    HwiTDLReg                   sHwiTDLReg;
    HwiTDHReg                   sHwiTDHReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiTFIReg, sizeof(HwiTFIReg));
    SPZeroMemory(&sHwiTIDReg, sizeof(HwiTIDReg));
    SPZeroMemory(&sHwiTDLReg, sizeof(HwiTDLReg));
    SPZeroMemory(&sHwiTDHReg, sizeof(HwiTDHReg));
    
    sHwiTFIReg.ExtendedFrame = pCANTxBuffer->bExtendedFrame;
    sHwiTFIReg.RTRBit = pCANTxBuffer->RTRBit;
    sHwiTFIReg.DataLengthCode = pCANTxBuffer->DataLengthCode;
    
    sHwiTIDReg.ID = pCANTxBuffer->ID;
    
    sHwiTDLReg.Data = pCANTxBuffer->Byte0_3;
    sHwiTDHReg.Data = pCANTxBuffer->Byte4_7;
    
    _SetHwiTFIReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTFIReg
        );
    
    _SetHwiTIDReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTIDReg
        );
    
    _SetHwiTDLReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTDLReg
        );
    
    _SetHwiTDHReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTDHReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetTxBuffer(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
        OUT PCANTxBuffer        pCANTxBuffer
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiTFIReg                   sHwiTFIReg;
    HwiTIDReg                   sHwiTIDReg;
    HwiTDLReg                   sHwiTDLReg;
    HwiTDHReg                   sHwiTDHReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiTFIReg, sizeof(HwiTFIReg));
    SPZeroMemory(&sHwiTIDReg, sizeof(HwiTIDReg));
    SPZeroMemory(&sHwiTDLReg, sizeof(HwiTDLReg));
    SPZeroMemory(&sHwiTDHReg, sizeof(HwiTDHReg));
    
    _GetHwiTFIReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTFIReg
        );
    
    _GetHwiTIDReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTIDReg
        );
    
    _GetHwiTDLReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTDLReg
        );
    
    _GetHwiTDHReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTDHReg
        );
    
    pCANTxBuffer->bExtendedFrame = sHwiTFIReg.ExtendedFrame;
    pCANTxBuffer->RTRBit = sHwiTFIReg.RTRBit;
    pCANTxBuffer->DataLengthCode = sHwiTFIReg.DataLengthCode;
    
    if (sHwiTFIReg.ExtendedFrame == 0)
    {
        pCANTxBuffer->ID = sHwiTIDReg.ID & CAN_MaskStardardID;
    }
    else
    {
        pCANTxBuffer->ID = sHwiTIDReg.ID & CAN_MaskExtendedID;
    }
    
    pCANTxBuffer->Byte0_3 = sHwiTDLReg.Data;
    pCANTxBuffer->Byte4_7 = sHwiTDHReg.Data;
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetRxBuffer(
    IN      PHwcCAN             hHwcCAN,
        OUT PCANRxBuffer        pCANRxBuffer
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiRFIReg                   sHwiRFIReg;
    HwiRIDReg                   sHwiRIDReg;
    HwiRDLReg                   sHwiRDLReg;
    HwiRDHReg                   sHwiRDHReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiRFIReg, sizeof(HwiRFIReg));
    SPZeroMemory(&sHwiRIDReg, sizeof(HwiRIDReg));
    SPZeroMemory(&sHwiRDLReg, sizeof(HwiRDLReg));
    SPZeroMemory(&sHwiRDHReg, sizeof(HwiRDHReg));
    
    _GetHwiRFIReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRFIReg
        );
    
    _GetHwiRIDReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRIDReg
        );
    
    _GetHwiRDLReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRDLReg
        );
    
    _GetHwiRDHReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRDHReg
        );
    
    
    pCANRxBuffer->bExtendedFrame = sHwiRFIReg.ExtendedFrame;
    pCANRxBuffer->RTRBit = sHwiRFIReg.RTRBit;
    pCANRxBuffer->DataLengthCode = sHwiRFIReg.DataLengthCode;
    
    if (sHwiRFIReg.ExtendedFrame == 0)
    {
        pCANRxBuffer->ID = sHwiRIDReg.ID & CAN_MaskStardardID;
    }
    else
    {
        pCANRxBuffer->ID = sHwiRIDReg.ID & CAN_MaskExtendedID;
    }
    
    pCANRxBuffer->Byte0_3 = sHwiRDLReg.Data;
    pCANRxBuffer->Byte4_7 = sHwiRDHReg.Data;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetTransmitStatus(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
        OUT PCANTransmitStatus  pCANTransmitStatus
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiTxBufferTransmitStatusReg    sHwiTxBufferTransmitStatusReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiTxBufferTransmitStatusReg, sizeof(HwiTxBufferTransmitStatusReg));
    _GetHwiTxBufferTransmitStatusReg(
        hHwcCAN->hMappedRegisters,
        TxBufferIndex,
        &sHwiTxBufferTransmitStatusReg
        );
    
    pCANTransmitStatus->RequestCompleted = sHwiTxBufferTransmitStatusReg.RequestCompleted;
    pCANTransmitStatus->RequestAborted = sHwiTxBufferTransmitStatusReg.RequestAborted;
    pCANTransmitStatus->TransmissionComplete = sHwiTxBufferTransmitStatusReg.TransmissionComplete;
    pCANTransmitStatus->ArbitrationLost = sHwiTxBufferTransmitStatusReg.ArbitrationLost;
    pCANTransmitStatus->BusError = sHwiTxBufferTransmitStatusReg.BusError;
    pCANTransmitStatus->BusOff = sHwiTxBufferTransmitStatusReg.BusOff;
    pCANTransmitStatus->ArbitrationLostLocation = sHwiTxBufferTransmitStatusReg.ArbitrationLostLocation;
    pCANTransmitStatus->BusErrorType = sHwiTxBufferTransmitStatusReg.BusErrorType;
    pCANTransmitStatus->BusErrorLocation = sHwiTxBufferTransmitStatusReg.BusErrorLocation;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetReceiveStatus(
    IN      PHwcCAN             hHwcCAN,
        OUT PCANReceiveStatus   pCANReceiveStatus
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiReceiveStatusReg         sHwiReceiveStatusReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiReceiveStatusReg, sizeof(HwiReceiveStatusReg));
    _GetHwiReceiveStatusReg(
        hHwcCAN->hMappedRegisters,
        &sHwiReceiveStatusReg
        );
    
    pCANReceiveStatus->RxFIFOPendingMessages = sHwiReceiveStatusReg.RxFIFOPendingMessages;
    pCANReceiveStatus->RxFIFOOverrun = sHwiReceiveStatusReg.RxFIFOOverrun;
    pCANReceiveStatus->BusError = sHwiReceiveStatusReg.BusError;
    pCANReceiveStatus->BusErrorType = sHwiReceiveStatusReg.BusErrorType;
    pCANReceiveStatus->BusErrorLocation = sHwiReceiveStatusReg.BusErrorLocation;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_TransmitRequest(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiRequestReg                 sHwiRequestReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiRequestReg, sizeof(HwiRequestReg));
    
    switch(TxBufferIndex)
    {
        case 0:
        {
            sHwiRequestReg.TransmitTxBuffer0 = 1;
        }
        break;
        case 1:
        {
            sHwiRequestReg.TransmitTxBuffer1 = 1;
        }
        break;
        case 2:
        {
            sHwiRequestReg.TransmitTxBuffer2 = 1;
        }
        break;
    }
    
    _SetHwiRequestReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRequestReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_AbortRequest(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiRequestReg               sHwiRequestReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiRequestReg, sizeof(HwiRequestReg));
    
    switch(TxBufferIndex)
    {
        case 0:
        {
            sHwiRequestReg.AbortTxBuffer0 = 1;
        }
        break;
        case 1:
        {
            sHwiRequestReg.AbortTxBuffer1 = 1;
        }
        break;
        case 2:
        {
            sHwiRequestReg.AbortTxBuffer2 = 1;
        }
        break;
    }
    
    _SetHwiRequestReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRequestReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_HaveRequestInQueue(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxBufferIndex,
        OUT PULONG              pHaveRequestInQueue
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiRequestReg               sHwiRequestReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiRequestReg, sizeof(HwiRequestReg));
    _GetHwiRequestReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRequestReg
        );
    
    switch(TxBufferIndex)
    {
        case 0:
        {
            *pHaveRequestInQueue = sHwiRequestReg.TransmitTxBuffer0;
        }
        break;
        case 1:
        {
            *pHaveRequestInQueue = sHwiRequestReg.TransmitTxBuffer1;
        }
        break;
        case 2:
        {
            *pHaveRequestInQueue = sHwiRequestReg.TransmitTxBuffer2;
        }
        break;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_ReleaseReceivedMessage(
    IN      PHwcCAN             hHwcCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiRequestReg               sHwiRequestReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiRequestReg, sizeof(HwiRequestReg));
    
    sHwiRequestReg.ReleaseReceivedMessage = 1;
    
    _SetHwiRequestReg(
        hHwcCAN->hMappedRegisters,
        &sHwiRequestReg
        );
    
    // wait for the action done.
    SPTime_StartTime(&sSPTime);
    while(1)
    {
        _GetHwiRequestReg(
            hHwcCAN->hMappedRegisters,
            &sHwiRequestReg
            );
        if (sHwiRequestReg.ReleaseReceivedMessage == 0)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
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
    
    return funcresult;
}


FRSTATUS
HwcCAN_SetErrorWarningLimit(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               ErrorWarningLimit
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiErrorWarningLimitReg     sHwiErrorWarningLimitReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiErrorWarningLimitReg, sizeof(HwiErrorWarningLimitReg));
    
    sHwiErrorWarningLimitReg.WarningLimit = ErrorWarningLimit;
    
    _SetHwiErrorWarningLimitReg(
        hHwcCAN->hMappedRegisters,
        &sHwiErrorWarningLimitReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetErrorWarningLimit(
    IN      PHwcCAN             hHwcCAN,
        OUT PULONG              pErrorWarningLimit
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiErrorWarningLimitReg     sHwiErrorWarningLimitReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    if (pErrorWarningLimit == NULL)
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiErrorWarningLimitReg, sizeof(HwiErrorWarningLimitReg));
    _GetHwiErrorWarningLimitReg(
        hHwcCAN->hMappedRegisters,
        &sHwiErrorWarningLimitReg
        );
    
    *pErrorWarningLimit = sHwiErrorWarningLimitReg.WarningLimit;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_SetErrorCounter(
    IN      PHwcCAN             hHwcCAN,
    IN      ULONG               TxErrorCounter,
    IN      ULONG               RxErrorCounter
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiErrorCounterReg          sHwiErrorCounterReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiErrorCounterReg, sizeof(HwiErrorCounterReg));
    
    sHwiErrorCounterReg.TxErrorCounter = TxErrorCounter;
    sHwiErrorCounterReg.RxErrorCounter = RxErrorCounter;
    
    _SetHwiErrorCounterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiErrorCounterReg
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwcCAN_GetErrorCounter(
    IN      PHwcCAN             hHwcCAN,
        OUT PULONG              pTxErrorCounter,
        OUT PULONG              pRxErrorCounter
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    HwiErrorCounterReg          sHwiErrorCounterReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwc level, we dont validate the input parameter.
    
    //
    // function body
    //
    
    SPZeroMemory(&sHwiErrorCounterReg, sizeof(HwiErrorCounterReg));
    _GetHwiErrorCounterReg(
        hHwcCAN->hMappedRegisters,
        &sHwiErrorCounterReg
        );
    
    *pTxErrorCounter = sHwiErrorCounterReg.TxErrorCounter;
    
    *pRxErrorCounter = sHwiErrorCounterReg.RxErrorCounter;

    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_Initialize(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               UseIOSpace
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    // System Bus Info
    hHwdCAN->BusNumber = 0;
    hHwdCAN->DeviceNumber = 0;
    hHwdCAN->FunctionNumber = 0;
    
    // Hardware Interface
    hHwdCAN->hMappedRegisters0 = &hHwdCAN->sMappedRegisters0;
    hHwdCAN->hMappedRegisters1 = &hHwdCAN->sMappedRegisters1;
    hHwdCAN->UseIOSpace = UseIOSpace;
    hHwdCAN->hMappedRegisters = NULL;
    
    // Hardware Core(Control)
    hHwdCAN->hHwcCAN = &hHwdCAN->sHwcCAN;
    
    HwcCAN_Initialize(
        hHwdCAN->hHwcCAN,
        NULL
        );
    
    hHwdCAN->CANRxMessages_Start = 0;
    hHwdCAN->CANRxMessages_End = 0;
    hHwdCAN->CANRxMessages_Overrun = FALSE;
    
    // Synchronization
    SPSpinLock_Initialize(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    SPSemaphore_Initialize(&hHwdCAN->sSPSemaphoreDPC, 1);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_OpenDevice(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               BusNumber,
    IN      ULONG               DeviceNumber,
    IN      ULONG               FunctionNumber,
    IN      ULONG               PortID
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    //ULONG                       BAR_CANReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    hHwdCAN->BusNumber = BusNumber;
    hHwdCAN->DeviceNumber = DeviceNumber;
    hHwdCAN->FunctionNumber = FunctionNumber;
    
    hHwdCAN->PortID = PortID;
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_BaseAddress(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               Base0Space,
    IN      PHYSICAL_ADDRESS    Base0Address,
    IN      ULONG               Base0Length,
    IN      ULONG               Base1Space,
    IN      PHYSICAL_ADDRESS    Base1Address,
    IN      ULONG               Base1Length
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //ULONG                       BAR_CANReg;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    // get & map registers
    /*result = BusDeviceIO_MapBusAddress(
        hHwdCAN->hMappedRegisters0,
        Base0Space,    //0x0 indicates memory, 0x1 is I/O space
        Base0Address,
        Base0Length
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    if (Base1Length != 0)
    {
        result = BusDeviceIO_MapBusAddress(
            hHwdCAN->hMappedRegisters1,
            Base1Space,    //0x0 indicates memory, 0x1 is I/O space
            Base1Address,
            Base1Length
            );
        if (!FR_SUCCESS(result))
        {
            funcresult = FR_Error;
            goto funcexit;
        }
    }
    else
    {
        hHwdCAN->hMappedRegisters1 = NULL;
    }*/
    
    hHwdCAN->hMappedRegisters0->AddressSpace = Base0Space;
    hHwdCAN->hMappedRegisters0->MappedAddress = (void*)Base0Address.QuadPart;
    
    hHwdCAN->hMappedRegisters1->AddressSpace = Base1Space;
    hHwdCAN->hMappedRegisters1->MappedAddress = (void*)Base1Address.QuadPart;
    
    if (hHwdCAN->UseIOSpace == TRUE)
    {
        if (Base0Space == 1)
        {
            hHwdCAN->hMappedRegisters = hHwdCAN->hMappedRegisters0;
        }
        else if (Base1Space == 1 && Base1Length != 0)
        {
            hHwdCAN->hMappedRegisters = hHwdCAN->hMappedRegisters1;
        }
        else
        {
            hHwdCAN->hMappedRegisters = hHwdCAN->hMappedRegisters0;
        }
    }
    else
    {
        if (Base0Space == 0)
        {
            hHwdCAN->hMappedRegisters = hHwdCAN->hMappedRegisters0;
        }
        else if (Base1Space == 0 && Base1Length != 0)
        {
            hHwdCAN->hMappedRegisters = hHwdCAN->hMappedRegisters1;
        }
        else
        {
            hHwdCAN->hMappedRegisters = hHwdCAN->hMappedRegisters0;
        }
    }
    
    result = HwcCAN_Initialize(
        hHwdCAN->hHwcCAN,
        hHwdCAN->hMappedRegisters
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_CloseDevice(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwdCAN->BusNumber = 0;
    hHwdCAN->DeviceNumber = 0;
    hHwdCAN->FunctionNumber = 0;
    
    HwcCAN_Initialize(
        hHwdCAN->hHwcCAN,
        NULL
        );
    
    // unmap registers
    /*result = BusDeviceIO_UnmapBusAddress(
        hHwdCAN->hMappedRegisters0
        );
    
    // unmap registers
    result = BusDeviceIO_UnmapBusAddress(
        hHwdCAN->hMappedRegisters1
        );*/
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_InitHW(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    result = HwcCAN_InitHW(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    result = HwcCAN_IFInitHW(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_CheckInterrupt(
    IN      PHwdCAN                 hHwdCAN,
        OUT PULONG                  pbInterrupt
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    HwcCANInterruptStatus       sHwcCANInterruptStatus;
    ULONG                       BusErrorDirection;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    *pbInterrupt = FALSE;
    
    // get interrupt status and volatile info
        
    result = HwcCAN_GetInterruptStatus(
        hHwcCAN,
        &sHwcCANInterruptStatus,
        &BusErrorDirection
        );
    
    // to quickly handle interrupts.
    if (sHwcCANInterruptStatus.Receive == FALSE  &&
        sHwcCANInterruptStatus.TxBuffer0Transmit == FALSE &&
        sHwcCANInterruptStatus.TxBuffer1Transmit == FALSE &&
        sHwcCANInterruptStatus.TxBuffer2Transmit == FALSE &&
        sHwcCANInterruptStatus.ErrorWarning == FALSE &&
        sHwcCANInterruptStatus.ErrorPassive == FALSE &&
        sHwcCANInterruptStatus.BusOff == FALSE &&
        sHwcCANInterruptStatus.ArbitrationLost == FALSE &&
        sHwcCANInterruptStatus.RxBusError == FALSE &&
        sHwcCANInterruptStatus.RxFIFOOverrun == FALSE &&
        sHwcCANInterruptStatus.PowerSavingExit == FALSE
        )
    {
        goto funcexit;
    }
    else
    {
        *pbInterrupt = TRUE;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}
    


FRSTATUS
HwdCAN_ISR(
    IN      PHwdCAN                 hHwdCAN,
        OUT PULONG                  pbInterrupt,
        OUT PHwdCANInterrupt        pHwdCANInterrupt,
        OUT PHwdCANVolatileInfo     pHwdCANVolatileInfo
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    HwcCANInterruptStatus       sHwcCANInterruptStatus;
    ULONG                       BusErrorDirection;
    
    CANReceiveStatus            sCANReceiveStatus;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    *pbInterrupt = FALSE;
    
    // get interrupt status and volatile info
        
    result = HwcCAN_GetInterruptStatus(
        hHwcCAN,
        &sHwcCANInterruptStatus,
        &BusErrorDirection
        );
    
    // to quickly handle interrupts.
    if (sHwcCANInterruptStatus.Receive == FALSE  &&
        sHwcCANInterruptStatus.TxBuffer0Transmit == FALSE &&
        sHwcCANInterruptStatus.TxBuffer1Transmit == FALSE &&
        sHwcCANInterruptStatus.TxBuffer2Transmit == FALSE &&
        sHwcCANInterruptStatus.ErrorWarning == FALSE &&
        sHwcCANInterruptStatus.ErrorPassive == FALSE &&
        sHwcCANInterruptStatus.BusOff == FALSE &&
        sHwcCANInterruptStatus.ArbitrationLost == FALSE &&
        sHwcCANInterruptStatus.RxBusError == FALSE &&
        sHwcCANInterruptStatus.RxFIFOOverrun == FALSE &&
        sHwcCANInterruptStatus.PowerSavingExit == FALSE
        )
    {
        goto funcexit;
    }
    
    pHwdCANVolatileInfo->NumRxCANMessage = 0;
    result = HwcCAN_GetReceiveStatus(
        hHwcCAN,
        &pHwdCANVolatileInfo->sCANReceiveStatus
        );
    
    if (sHwcCANInterruptStatus.Receive == TRUE)
    {
        result = HwcCAN_GetRxBuffer(
            hHwcCAN,
            &pHwdCANVolatileInfo->sCANRxBuffer
            );
        
        /*if (sCANRxBuffer.ID == hHwdCAN->sPreviousRxBuffer.ID
            && sCANRxBuffer.bExtendedFrame == hHwdCAN->sPreviousRxBuffer.bExtendedFrame
            && sCANRxBuffer.RTRBit == hHwdCAN->sPreviousRxBuffer.RTRBit
            && sCANRxBuffer.DataLengthCode == hHwdCAN->sPreviousRxBuffer.DataLengthCode
            )
        {
            DbgBreakPoint();
        }*/
        
        result = HwcCAN_ReleaseReceivedMessage(hHwcCAN);
        pHwdCANVolatileInfo->NumRxCANMessage = 1;
        
        /*
        result = HwcCAN_GetReceiveStatus(
            hHwcCAN,
            &pHwdCANVolatileInfo->sCANReceiveStatus
            );
        if (pHwdCANVolatileInfo->sCANReceiveStatus.RxFIFOPendingMessages == 1)
        {
            result = HwcCAN_GetRxBuffer(
                hHwcCAN,
                &pHwdCANVolatileInfo->sCANRxBuffer2
                );
            result = HwcCAN_ReleaseReceivedMessage(hHwcCAN);
            pHwdCANVolatileInfo->NumRxCANMessage = 2;
        }
        */
        /*
        result = HwcCAN_GetReceiveStatus(
            hHwcCAN,
            &pHwdCANVolatileInfo->sCANReceiveStatus
            );
        if (pHwdCANVolatileInfo->sCANReceiveStatus.RxFIFOPendingMessages == 1)
        {
            result = HwcCAN_GetRxBuffer(
                hHwcCAN,
                &pHwdCANVolatileInfo->sCANRxBuffer3
                );
            result = HwcCAN_ReleaseReceivedMessage(hHwcCAN);
            pHwdCANVolatileInfo->NumRxCANMessage = 3;
        }*/
        
        // for debug of repeat send message.
        //hHwdCAN->sPreviousRxBuffer = sCANRxBuffer;
        
        /*result = HwcCAN_GetRxBuffer(
            hHwcCAN,
            &sCANRxBuffer
            );
        
        if (sCANRxBuffer.ID == hHwdCAN->sPreviousRxBuffer.ID
            && sCANRxBuffer.bExtendedFrame == hHwdCAN->sPreviousRxBuffer.bExtendedFrame
            && sCANRxBuffer.RTRBit == hHwdCAN->sPreviousRxBuffer.RTRBit
            && sCANRxBuffer.DataLengthCode == hHwdCAN->sPreviousRxBuffer.DataLengthCode
            )
        {
            DbgBreakPoint();
        }*/
    }
    
    // quickly clear interrupt that are handled.
    result = HwcCAN_ClearInterruptStatus(
        hHwcCAN,
        &sHwcCANInterruptStatus
        );
        
    *pHwdCANInterrupt = sHwcCANInterruptStatus;
        
    pHwdCANVolatileInfo->BusErrorDirection = BusErrorDirection;
    
    pHwdCANVolatileInfo->sCANReceiveStatus = sCANReceiveStatus;
    
    if (sHwcCANInterruptStatus.Receive == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.TxBuffer0Transmit == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.TxBuffer1Transmit == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.TxBuffer2Transmit == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.ErrorWarning == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.ErrorPassive == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.BusOff == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.ArbitrationLost == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.RxBusError == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.RxFIFOOverrun == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    if (sHwcCANInterruptStatus.PowerSavingExit == TRUE)
    {
        *pbInterrupt = TRUE;
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_DCPForISR(
    IN      PHwdCAN                 hHwdCAN,
    IN      PHwdCANInterrupt        pHwdCANInterrupt,
    IN      PHwdCANVolatileInfo     pHwdCANVolatileInfo
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    ULONG                       IndexStart;
    ULONG                       IndexEnd;
    ULONG                       bFull;
    
    HwiGlobalControlReg         sHwiGlobalControlReg;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    UINT                        flags;
    
    PCANMessage                 pCANRxMessage;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    if (pHwdCANInterrupt->Receive == TRUE)
    {
        SPSpinLock_AcquireAtDpcLevel(&hHwdCAN->sSPSpinLockCANRxMessages, &flags);
        
        // check if FIFO Full
        IndexStart = hHwdCAN->CANRxMessages_Start;
        IndexEnd = hHwdCAN->CANRxMessages_End;
        bFull = FALSE;
        if (((IndexEnd + 1)%HwdCAN_MaxCANRxMessages) == IndexStart)
        {
            // overrun
            bFull = TRUE;
        }
        
        if (bFull == FALSE)
        {
            hHwdCAN->CANRxMessages_Overrun = FALSE;
            
            // put into RxMessages Queue
            //SPCopyMemory(&hHwdCAN->CANRxMessages[IndexEnd], &pHwdCANVolatileInfo->sRxCANMessage, sizeof(CANMessage));
            
            pCANRxMessage = &hHwdCAN->CANRxMessages[IndexEnd];
            pCANRxMessage->FrameFormat = pHwdCANVolatileInfo->sCANRxBuffer.bExtendedFrame;
            pCANRxMessage->ID = pHwdCANVolatileInfo->sCANRxBuffer.ID;
            pCANRxMessage->FrameType = pHwdCANVolatileInfo->sCANRxBuffer.RTRBit;
            pCANRxMessage->DataLength = pHwdCANVolatileInfo->sCANRxBuffer.DataLengthCode;
            *(PULONG)&pCANRxMessage->Data[0] = pHwdCANVolatileInfo->sCANRxBuffer.Byte0_3;
            *(PULONG)&pCANRxMessage->Data[4] = pHwdCANVolatileInfo->sCANRxBuffer.Byte4_7;
            
            // update CANRxMessages_End
            hHwdCAN->CANRxMessages_End = (IndexEnd + 1)%HwdCAN_MaxCANRxMessages;
        }
        else
        {
            hHwdCAN->CANRxMessages_Overrun = TRUE;
        }
        
        SPSpinLock_ReleaseFromDpcLevel(&hHwdCAN->sSPSpinLockCANRxMessages, &flags);
        
    }
    
    if (pHwdCANInterrupt->TxBuffer0Transmit == TRUE)
    {
    
    }
    
    if (pHwdCANInterrupt->TxBuffer1Transmit == TRUE)
    {
    
    }
    
    if (pHwdCANInterrupt->TxBuffer2Transmit == TRUE)
    {
    
    }
    
    if (pHwdCANInterrupt->ErrorWarning == TRUE)
    {
        
    }
    
    if (pHwdCANInterrupt->ErrorPassive == TRUE)
    {
        
    }
    
    if (pHwdCANInterrupt->BusOff == TRUE)
    {
        // may be check which TxBuffer is in Bus Off
        
        // the controller will be disable.
        SPZeroMemory(&sHwiGlobalControlReg, sizeof(HwiGlobalControlReg));
        /*sHwiGlobalControlReg.Active = 1;
        while(sHwiGlobalControlReg.Active == 1)
        {
            _GetHwiGlobalControlReg(
                hHwcCAN->hMappedRegisters,
                &sHwiGlobalControlReg
                );
        }*/
        SPTime_StartTime(&sSPTime);
        while(1)
        {
            _GetHwiGlobalControlReg(
                hHwcCAN->hMappedRegisters,
                &sHwiGlobalControlReg
                );
            if (sHwiGlobalControlReg.Active == 0)
            {
                break;
            }
            
            SPTime_EndTime(&sSPTime);
            ms = 0;
            SPTime_EscapeTime(&sSPTime, &ms);
            if (ms >= 1000) // 1000 ms
            {
                funcresult = FR_Error;
                break;
            }
        }
                
        SPSemaphore_Acquire(&hHwdCAN->sSPSemaphoreDPC);
        switch(hHwdCAN->BusOffHandleMethod)
        {
            case 1:
            {
                result = HwcCAN_Start(hHwcCAN);
            }
            break;
            case 2:
            {
                result = HwcCAN_SetErrorCounter(
                    hHwcCAN,
                    0,
                    0
                    );
                result = HwcCAN_Start(hHwcCAN);
            }
            break;
            case 3:
            {
                result = HwcCAN_SetErrorCounter(
                    hHwcCAN,
                    HwcCAN_ErrorPassive,
                    HwcCAN_ErrorPassive
                    );
                result = HwcCAN_Start(hHwcCAN);
            }
            break;
            default:
            {
            }
            break;
        }
        SPSemaphore_Release(&hHwdCAN->sSPSemaphoreDPC);
    }
    
    if (pHwdCANInterrupt->ArbitrationLost == TRUE)
    {
        
    }
    
    if (pHwdCANInterrupt->RxBusError == TRUE)
    {
        
    }
    
    if (pHwdCANInterrupt->RxFIFOOverrun == TRUE)
    {
        
    }
    
    if (pHwdCANInterrupt->PowerSavingExit == TRUE)
    {
        
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_EnableInterrupt(
    IN      PHwdCAN             hHwdCAN,
    IN      PHwdCANInterrupt    pHwdCANInterrupt    // TRUE to Enable
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    HwcCANInterrupt             sHwcCANInterrupt;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_GetInterruptEnable(
        hHwcCAN,
        &sHwcCANInterrupt
        );
    
    if (pHwdCANInterrupt->Receive == TRUE)
    {
        sHwcCANInterrupt.Receive = TRUE;
    }
    if (pHwdCANInterrupt->TxBuffer0Transmit == TRUE)
    {
        sHwcCANInterrupt.TxBuffer0Transmit = TRUE;
    }
    if (pHwdCANInterrupt->TxBuffer1Transmit == TRUE)
    {
        sHwcCANInterrupt.TxBuffer1Transmit = TRUE;
    }
    if (pHwdCANInterrupt->TxBuffer2Transmit == TRUE)
    {
        sHwcCANInterrupt.TxBuffer2Transmit = TRUE;
    }
    if (pHwdCANInterrupt->ErrorWarning == TRUE)
    {
        sHwcCANInterrupt.ErrorWarning = TRUE;
    }
    if (pHwdCANInterrupt->ErrorPassive == TRUE)
    {
        sHwcCANInterrupt.ErrorPassive = TRUE;
    }
    if (pHwdCANInterrupt->BusOff == TRUE)
    {
        sHwcCANInterrupt.BusOff = TRUE;
    }
    if (pHwdCANInterrupt->ArbitrationLost == TRUE)
    {
        sHwcCANInterrupt.ArbitrationLost = TRUE;
    }
    if (pHwdCANInterrupt->RxBusError == TRUE)
    {
        sHwcCANInterrupt.RxBusError = TRUE;
    }
    if (pHwdCANInterrupt->RxFIFOOverrun == TRUE)
    {
        sHwcCANInterrupt.RxFIFOOverrun = TRUE;
    }
    if (pHwdCANInterrupt->PowerSavingExit == TRUE)
    {
        sHwcCANInterrupt.PowerSavingExit = TRUE;
    }
    
    result = HwcCAN_SetInterruptEnable(
        hHwcCAN,
        &sHwcCANInterrupt
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_DisableInterrupt(
    IN      PHwdCAN             hHwdCAN,
    IN      PHwdCANInterrupt    pHwdCANInterrupt    // TRUE to Disable
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    HwcCANInterrupt             sHwcCANInterrupt;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_GetInterruptEnable(
        hHwcCAN,
        &sHwcCANInterrupt
        );
    
    if (pHwdCANInterrupt->Receive == TRUE)
    {
        sHwcCANInterrupt.Receive = FALSE;
    }
    if (pHwdCANInterrupt->TxBuffer0Transmit == TRUE)
    {
        sHwcCANInterrupt.TxBuffer0Transmit = FALSE;
    }
    if (pHwdCANInterrupt->TxBuffer1Transmit == TRUE)
    {
        sHwcCANInterrupt.TxBuffer1Transmit = FALSE;
    }
    if (pHwdCANInterrupt->TxBuffer2Transmit == TRUE)
    {
        sHwcCANInterrupt.TxBuffer2Transmit = FALSE;
    }
    if (pHwdCANInterrupt->ErrorWarning == TRUE)
    {
        sHwcCANInterrupt.ErrorWarning = FALSE;
    }
    if (pHwdCANInterrupt->ErrorPassive == TRUE)
    {
        sHwcCANInterrupt.ErrorPassive = FALSE;
    }
    if (pHwdCANInterrupt->BusOff == TRUE)
    {
        sHwcCANInterrupt.BusOff = FALSE;
    }
    if (pHwdCANInterrupt->ArbitrationLost == TRUE)
    {
        sHwcCANInterrupt.ArbitrationLost = FALSE;
    }
    if (pHwdCANInterrupt->RxBusError == TRUE)
    {
        sHwcCANInterrupt.RxBusError = FALSE;
    }
    if (pHwdCANInterrupt->RxFIFOOverrun == TRUE)
    {
        sHwcCANInterrupt.RxFIFOOverrun = FALSE;
    }
    if (pHwdCANInterrupt->PowerSavingExit == TRUE)
    {
        sHwcCANInterrupt.PowerSavingExit = FALSE;
    }
    
    result = HwcCAN_SetInterruptEnable(
        hHwcCAN,
        &sHwcCANInterrupt
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_GetInterruptEnable(
    IN      PHwdCAN             hHwdCAN,
        OUT PHwdCANInterrupt    pHwdCANInterrupt
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    HwcCANInterrupt             sHwcCANInterrupt;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_GetInterruptEnable(
        hHwcCAN,
        &sHwcCANInterrupt
        );
    
    pHwdCANInterrupt->Receive = sHwcCANInterrupt.Receive;
    pHwdCANInterrupt->TxBuffer0Transmit = sHwcCANInterrupt.TxBuffer0Transmit;
    pHwdCANInterrupt->TxBuffer1Transmit = sHwcCANInterrupt.TxBuffer1Transmit;
    pHwdCANInterrupt->TxBuffer2Transmit = sHwcCANInterrupt.TxBuffer2Transmit;
    pHwdCANInterrupt->ErrorWarning = sHwcCANInterrupt.ErrorWarning;
    pHwdCANInterrupt->ErrorPassive = sHwcCANInterrupt.ErrorPassive;
    pHwdCANInterrupt->BusOff = sHwcCANInterrupt.BusOff;
    pHwdCANInterrupt->ArbitrationLost = sHwcCANInterrupt.ArbitrationLost;
    pHwdCANInterrupt->RxBusError = sHwcCANInterrupt.RxBusError;
    pHwdCANInterrupt->RxFIFOOverrun = sHwcCANInterrupt.RxFIFOOverrun;
    pHwdCANInterrupt->PowerSavingExit = sHwcCANInterrupt.PowerSavingExit;
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SetClockPrescaler(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               CKDIV,          // 0~0x3F
    IN      ULONG               bExternalInput
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_SetClockPrescaler(
        hHwcCAN,
        CKDIV,
        bExternalInput
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SetBusTiming(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               PropSEG,    // 0~7
    IN      ULONG               PSEG1,      // 0~7
    IN      ULONG               PSEG2,      // 0~7
    IN      ULONG               SJW,        // 0~3
    IN      ULONG               Sampling    // 0 or 1
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_SetBusTiming(
        hHwcCAN,
        PropSEG,
        PSEG1,
        PSEG2,
        SJW,
        Sampling
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SetGlobalControl(
    IN      PHwdCAN             hHwdCAN,
    IN      PCANGlobalControl   pCANGlobalControl
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_SetGlobalControl(
        hHwcCAN,
        pCANGlobalControl
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_GetGlobalControl(
    IN      PHwdCAN             hHwdCAN,
        OUT PCANGlobalControl   pCANGlobalControl
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_GetGlobalControl(
        hHwcCAN,
        pCANGlobalControl
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SetBusOffHandle(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               BusOffHandleMethod
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    SPSemaphore_Acquire(&hHwdCAN->sSPSemaphoreDPC);
    
    hHwdCAN->BusOffHandleMethod = BusOffHandleMethod;
    
    SPSemaphore_Release(&hHwdCAN->sSPSemaphoreDPC);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_ResetCAN(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    result = HwcCAN_InitHW(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_StartCAN(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_Start(
        hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_StopCAN(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_Stop(
        hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_ResetCANIF(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    result = HwcCAN_IFInitHW(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_StartCANIF(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    result = HwcCAN_IFStart(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_StopCANIF(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    result = HwcCAN_IFStop(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_EnableCANIFByPass(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    result = HwcCAN_IFEnableByPass(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_DisableCANIFByPass(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    result = HwcCAN_IFDisableByPass(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_GetGlobalStatus(
    IN      PHwdCAN             hHwdCAN,
        OUT PULONG              pbErrorCounterWarning,
        OUT PULONG              pbErrorPassive,
        OUT PULONG              pbBusOff
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_GetGlobalStatus(
        hHwcCAN,
        pbErrorCounterWarning,
        pbErrorPassive,
        pbBusOff
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SetFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex,
    IN      PCANFilter          pCANFilter
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    result = HwcCAN_IFSetFilter(
        hHwdCAN->hHwcCAN,
        FilterIndex,    // 0 ~ 31
        pCANFilter->bEnable,
        pCANFilter->bExtendedID,
        pCANFilter->ID,
        pCANFilter->Mask
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_GetFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex,
        OUT PCANFilter          pCANFilter
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    result = HwcCAN_IFGetFilter(
        hHwdCAN->hHwcCAN,
        FilterIndex,    // 0 ~ 31
        &pCANFilter->bEnable,
        &pCANFilter->bExtendedID,
        &pCANFilter->ID,
        &pCANFilter->Mask
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_EnableFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    result = HwcCAN_IFEnableFilter(
        hHwdCAN->hHwcCAN,
        FilterIndex     // 0 ~ 31
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_DisableFilter(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               FilterIndex
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    result = HwcCAN_IFDisableFilter(
        hHwdCAN->hHwcCAN,
        FilterIndex     // 0 ~ 31
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_RemoveAllFilter(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    result = HwcCAN_IFRemoveAllFilter(
        hHwdCAN->hHwcCAN
        );
    if (!FR_SUCCESS(result))
    {
        funcresult = FR_Error;
        goto funcexit;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SendMessage(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
    IN      PCANMessage         pCANMessage
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    CANTxBuffer                 sCANTxBuffer;
    
    ULONG                       HaveRequestInQueue;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    sCANTxBuffer.bExtendedFrame = pCANMessage->FrameFormat;
    sCANTxBuffer.ID = pCANMessage->ID;
    sCANTxBuffer.RTRBit = pCANMessage->FrameType;
    sCANTxBuffer.DataLengthCode = pCANMessage->DataLength;
    sCANTxBuffer.Byte0_3 = *(PULONG)&pCANMessage->Data[0];
    sCANTxBuffer.Byte4_7 = *(PULONG)&pCANMessage->Data[4];
    
    HaveRequestInQueue = TRUE;
    SPTime_StartTime(&sSPTime);
    while(HaveRequestInQueue == TRUE)
    {
        result = HwcCAN_HaveRequestInQueue(
            hHwcCAN,
            TxBufferIndex,
            &HaveRequestInQueue
            );
        // in this driver design, HaveRequestInQueue should FALSE
        if (HaveRequestInQueue == FALSE)
        {
            break;
        }
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
        }
    }
    
    result = HwcCAN_SetTxBuffer(
        hHwcCAN,
        TxBufferIndex,
        &sCANTxBuffer
        );
    
    result = HwcCAN_TransmitRequest(
        hHwcCAN,
        TxBufferIndex
        );
    
    // after calling this function, OS Driver framework should handle interrupt event and the transmit status.
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}

FRSTATUS
HwdCAN_SendTxBuffer(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
    IN      PCANTxBuffer        pCANTxBuffer
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_SetTxBuffer(
        hHwcCAN,
        TxBufferIndex,
        pCANTxBuffer
        );
    
    result = HwcCAN_TransmitRequest(
        hHwcCAN,
        TxBufferIndex
        );
    
    // after calling this function, OS Driver framework should handle interrupt event and the transmit status.
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_AbortMessage(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
    IN      PULONG              pbAbort
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //CANTxBuffer                 sCANTxBuffer;
    
    ULONG                       HaveRequestInQueue;
    CANTransmitStatus           sCANTransmitStatus;
    
    SPTime                      sSPTime;
    ULONGLONG                   ms;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_AbortRequest(
        hHwcCAN,
        TxBufferIndex
        );
    
    HaveRequestInQueue = TRUE;
    SPTime_StartTime(&sSPTime);
    while(HaveRequestInQueue == TRUE)
    {
        result = HwcCAN_HaveRequestInQueue(
            hHwcCAN,
            TxBufferIndex,
            &HaveRequestInQueue
            );
        // in this driver design, HaveRequestInQueue should FALSE
        
        SPTime_EndTime(&sSPTime);
        ms = 0;
        SPTime_EscapeTime(&sSPTime, &ms);
        if (ms >= 1000) // 1000 ms
        {
            funcresult = FR_Error;
            break;
        }
    }
    
    result = HwcCAN_GetTransmitStatus(
        hHwcCAN,
        TxBufferIndex,
        &sCANTransmitStatus
        );
    
    if (sCANTransmitStatus.RequestAborted == TRUE)
    {
        *pbAbort = TRUE;
    }
    else
    {
        *pbAbort = FALSE;
    }
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_GetTransmitStatus(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               TxBufferIndex,
        OUT PCANTransmitStatus  pCANTransmitStatus
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    result = HwcCAN_GetTransmitStatus(
        hHwcCAN,
        TxBufferIndex,
        pCANTransmitStatus
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_HaveAnyRxMessage(
    IN      PHwdCAN             hHwdCAN,
        OUT PULONG              pHaveAnyRxMessage
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       CANRxMessages_Start;
    ULONG                       CANRxMessages_End;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    SPSpinLock_Acquire(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    CANRxMessages_Start = hHwdCAN->CANRxMessages_Start;
    CANRxMessages_End =  hHwdCAN->CANRxMessages_End;
    
    // check if pending message is in FIFO
    if (CANRxMessages_Start == CANRxMessages_End)
    {   // empty
        *pHaveAnyRxMessage = FALSE;
    }
    else
    {
        *pHaveAnyRxMessage = TRUE;
    }
    
    SPSpinLock_Release(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_GetRxMessage(
    IN      PHwdCAN             hHwdCAN,
        OUT PULONG              pNoRxMessage,
        OUT PCANMessage         pCANMessage
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    
    ULONG                       IndexStart;
    ULONG                       IndexEnd;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    SPSpinLock_Acquire(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    IndexStart = hHwdCAN->CANRxMessages_Start;
    IndexEnd = hHwdCAN->CANRxMessages_End;
    
    // check if pending message is in FIFO
    if (IndexStart == IndexEnd)
    {   // empty
        *pNoRxMessage = TRUE;
    }
    else
    {
        *pNoRxMessage = FALSE;
                
        // get pending message
        SPCopyMemory(pCANMessage, &hHwdCAN->CANRxMessages[IndexStart], sizeof(CANMessage));
        
        // update CANRxMessages_Start
        hHwdCAN->CANRxMessages_Start = (IndexStart + 1)%HwdCAN_MaxCANRxMessages;
        
        /*if ((CANRxMessages_Start + 1) == HwdCAN_MaxCANRxMessages)
        {
            *pCANRxMessages_Start = 0;
        }
        else
        {
            *pCANRxMessages_Start += 1;
        }*/        
    }
    
    SPSpinLock_Release(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_HaveRxFIFOOverrun(
    IN      PHwdCAN             hHwdCAN,
        OUT PULONG              pHaveRxFIFOOverrun
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    SPSpinLock_Acquire(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    *pHaveRxFIFOOverrun = hHwdCAN->CANRxMessages_Overrun;
    
    SPSpinLock_Release(&hHwdCAN->sSPSpinLockCANRxMessages);        
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_ClearRxFIFO(
    IN      PHwdCAN             hHwdCAN
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    SPSpinLock_Acquire(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    hHwdCAN->CANRxMessages_Start = 0;
    hHwdCAN->CANRxMessages_End = 0;
    hHwdCAN->CANRxMessages_Overrun = FALSE;
        
    SPSpinLock_Release(&hHwdCAN->sSPSpinLockCANRxMessages);
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_RoundRobinMessage(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               Tx0Enable,
    IN      ULONG               Tx1Enable,
    IN      ULONG               Tx2Enable,
    IN      PCANMessage         losCANMessage
    )
{
    FRSTATUS                    funcresult;
    ULONG                       result;
    
    PHwcCAN                     hHwcCAN;
    
    CANTxBuffer                 sCANTxBuffer;
    
    //ULONG                       HaveRequestInQueue;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    hHwcCAN = hHwdCAN->hHwcCAN;
    
    if (Tx0Enable == TRUE)
    {
        sCANTxBuffer.bExtendedFrame = losCANMessage[0].FrameFormat;
        sCANTxBuffer.ID = losCANMessage[0].ID;
        sCANTxBuffer.RTRBit = losCANMessage[0].FrameType;
        sCANTxBuffer.DataLengthCode = losCANMessage[0].DataLength;
        sCANTxBuffer.Byte0_3 = *(PULONG)&losCANMessage[0].Data[0];
        sCANTxBuffer.Byte4_7 = *(PULONG)&losCANMessage[0].Data[4];
        
        result = HwcCAN_SetTxBuffer(
            hHwcCAN,
            0,
            &sCANTxBuffer
            );
    }
    
    if (Tx1Enable == TRUE)
    {
        sCANTxBuffer.bExtendedFrame = losCANMessage[1].FrameFormat;
        sCANTxBuffer.ID = losCANMessage[1].ID;
        sCANTxBuffer.RTRBit = losCANMessage[1].FrameType;
        sCANTxBuffer.DataLengthCode = losCANMessage[1].DataLength;
        sCANTxBuffer.Byte0_3 = *(PULONG)&losCANMessage[1].Data[0];
        sCANTxBuffer.Byte4_7 = *(PULONG)&losCANMessage[1].Data[4];
        
        result = HwcCAN_SetTxBuffer(
            hHwcCAN,
            1,
            &sCANTxBuffer
            );
    }
    
    if (Tx2Enable == TRUE)
    {
        sCANTxBuffer.bExtendedFrame = losCANMessage[2].FrameFormat;
        sCANTxBuffer.ID = losCANMessage[2].ID;
        sCANTxBuffer.RTRBit = losCANMessage[2].FrameType;
        sCANTxBuffer.DataLengthCode = losCANMessage[2].DataLength;
        sCANTxBuffer.Byte0_3 = *(PULONG)&losCANMessage[2].Data[0];
        sCANTxBuffer.Byte4_7 = *(PULONG)&losCANMessage[2].Data[4];
        
        result = HwcCAN_SetTxBuffer(
            hHwcCAN,
            2,
            &sCANTxBuffer
            );
    }
    
    if (Tx0Enable == TRUE)
    {
        result = HwcCAN_TransmitRequest(
            hHwcCAN,
            0
            );
    }
    
    if (Tx1Enable == TRUE)
    {
        result = HwcCAN_TransmitRequest(
            hHwcCAN,
            1
            );
    }
    
    if (Tx2Enable == TRUE)
    {
        result = HwcCAN_TransmitRequest(
            hHwcCAN,
            2
            );
    }
    
    // after calling this function, OS Driver framework should handle interrupt event and the transmit status.
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SetHwiReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
    IN      PVOID               pHwiReg,
    IN      ULONG               SizeOfHwiReg
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    PMappedBusAddress           hMappedRegisters;
    
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hMappedRegisters = hHwdCAN->hMappedRegisters;
    
    switch(RegID)
    {
        case CANRegID_HwiGlobalControlReg:
        {
            if (SizeOfHwiReg != sizeof(HwiGlobalControlReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiGlobalControlReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiClockPrescalerReg:
        {
            if (SizeOfHwiReg != sizeof(HwiClockPrescalerReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiClockPrescalerReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiBusTimingReg:
        {
            if (SizeOfHwiReg != sizeof(HwiBusTimingReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiBusTimingReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiInterruptEnableReg:
        {
            if (SizeOfHwiReg != sizeof(HwiInterruptEnableReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiInterruptEnableReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiInterruptStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiInterruptStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiInterruptStatusReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiGlobalStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiGlobalStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            //_SetHwiGlobalStatusReg(
            //    hMappedRegisters,
            //    pHwiReg
            //    );
        }
        break;
        
        case CANRegID_HwiRequestReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRequestReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiRequestReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTxBuffer0TransmitStatusReg:
        case CANRegID_HwiTxBuffer1TransmitStatusReg:
        case CANRegID_HwiTxBuffer2TransmitStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiTxBufferTransmitStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            /*_SetHwiTxBufferTransmitStatusReg(
                hMappedRegisters,
                pHwiReg
                );*/
        }
        break;
        
        case CANRegID_HwiReceiveStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiReceiveStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            //_SetHwiReceiveStatusReg(
            //    hMappedRegisters,
            //    pHwiReg
            //    );
        }
        break;
        
        case CANRegID_HwiErrorWarningLimitReg:
        {
            if (SizeOfHwiReg != sizeof(HwiErrorWarningLimitReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiErrorWarningLimitReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiErrorCounterReg:
        {
            if (SizeOfHwiReg != sizeof(HwiErrorCounterReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiErrorCounterReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiIdentifierIndexReg:
        {
            if (SizeOfHwiReg != sizeof(HwiIdentifierIndexReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiIdentifierIndexReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiIdentifierFilterReg:
        {
            if (SizeOfHwiReg != sizeof(HwiIdentifierFilterReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiIdentifierFilterReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiIdentifierMaskReg:
        {
            if (SizeOfHwiReg != sizeof(HwiIdentifierMaskReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiIdentifierMaskReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTFI0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTFIReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTID0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTIDReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDL0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTDLReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDH0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTDHReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTFI1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTFIReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTID1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTIDReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDL1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTDLReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDH1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTDHReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTFI2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTFIReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTID2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTIDReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDL2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTDLReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDH2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _SetHwiTDHReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiRFIReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            //_SetHwiRFIReg(
            //    hMappedRegisters,
            //    pHwiReg
            //    );
        }
        break;
        
        case CANRegID_HwiRIDReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            //_SetHwiRIDReg(
            //    hMappedRegisters,
            //    pHwiReg
            //    );
        }
        break;
        
        case CANRegID_HwiRDLReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            //_SetHwiRDLReg(
            //    hMappedRegisters,
            //    pHwiReg
            //    );
        }
        break;
        
        case CANRegID_HwiRDHReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            //_SetHwiRDHReg(
            //    hMappedRegisters,
            //    pHwiReg
            //    );
        }
        break;
        
        default:
        {
            funcresult = FR_Error;
        }
        break;
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_GetHwiReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
        OUT PVOID               pHwiReg,
    IN      ULONG               SizeOfHwiReg
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    PMappedBusAddress           hMappedRegisters;
    
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    hMappedRegisters = hHwdCAN->hMappedRegisters;
    
    switch(RegID)
    {
        case CANRegID_HwiGlobalControlReg:
        {
            if (SizeOfHwiReg != sizeof(HwiGlobalControlReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiGlobalControlReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiClockPrescalerReg:
        {
            if (SizeOfHwiReg != sizeof(HwiClockPrescalerReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiClockPrescalerReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiBusTimingReg:
        {
            if (SizeOfHwiReg != sizeof(HwiBusTimingReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiBusTimingReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiInterruptEnableReg:
        {
            if (SizeOfHwiReg != sizeof(HwiInterruptEnableReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiInterruptEnableReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiInterruptStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiInterruptStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiInterruptStatusReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiGlobalStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiGlobalStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiGlobalStatusReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiRequestReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRequestReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiRequestReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTxBuffer0TransmitStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiTxBufferTransmitStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTxBufferTransmitStatusReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTxBuffer1TransmitStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiTxBufferTransmitStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTxBufferTransmitStatusReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTxBuffer2TransmitStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiTxBufferTransmitStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTxBufferTransmitStatusReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiReceiveStatusReg:
        {
            if (SizeOfHwiReg != sizeof(HwiReceiveStatusReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiReceiveStatusReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiErrorWarningLimitReg:
        {
            if (SizeOfHwiReg != sizeof(HwiErrorWarningLimitReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiErrorWarningLimitReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiErrorCounterReg:
        {
            if (SizeOfHwiReg != sizeof(HwiErrorCounterReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiErrorCounterReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiIdentifierIndexReg:
        {
            if (SizeOfHwiReg != sizeof(HwiIdentifierIndexReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiIdentifierIndexReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiIdentifierFilterReg:
        {
            if (SizeOfHwiReg != sizeof(HwiIdentifierFilterReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiIdentifierFilterReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiIdentifierMaskReg:
        {
            if (SizeOfHwiReg != sizeof(HwiIdentifierMaskReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiIdentifierMaskReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTFI0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTFIReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTID0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTIDReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDL0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTDLReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDH0Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTDHReg(
                hMappedRegisters,
                0,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTFI1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTFIReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTID1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTIDReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDL1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTDLReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDH1Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTDHReg(
                hMappedRegisters,
                1,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTFI2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTFIReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTID2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTIDReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDL2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTDLReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiTDH2Reg:
        {
            if (SizeOfHwiReg != sizeof(HwiTDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiTDHReg(
                hMappedRegisters,
                2,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiRFIReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRFIReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiRFIReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiRIDReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRIDReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiRIDReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiRDLReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRDLReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiRDLReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        case CANRegID_HwiRDHReg:
        {
            if (SizeOfHwiReg != sizeof(HwiRDHReg))
            {
                funcresult = FR_Error;
                goto funcexit;
            }
            _GetHwiRDHReg(
                hMappedRegisters,
                pHwiReg
                );
        }
        break;
        
        default:
        {
            funcresult = FR_Error;
        }
        break;
        
        
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}


FRSTATUS
HwdCAN_SetReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
    IN      ULONG               Value
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    PMappedBusAddress           hMappedRegisters;
    ULONG                       RegOffset;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    hMappedRegisters = hHwdCAN->hMappedRegisters;
    
    RegOffset = 0xFFFF;
    
    switch(RegID)
    {
        case CANRegID_HwiGlobalControlReg:
        {
            RegOffset = CANRegID_HwiGlobalControlReg;
        }
        break;
        
        case CANRegID_HwiClockPrescalerReg:
        {
            RegOffset = CANRegOffset_HwiClockPrescalerReg;
        }
        break;
        
        case CANRegID_HwiBusTimingReg:
        {
            RegOffset = CANRegOffset_HwiBusTimingReg;
        }
        break;
        
        case CANRegID_HwiInterruptEnableReg:
        {
            RegOffset = CANRegOffset_HwiInterruptEnableReg;
        }
        break;
        
        case CANRegID_HwiInterruptStatusReg:
        {
            RegOffset = CANRegOffset_HwiInterruptStatusReg;
        }
        break;
        
        case CANRegID_HwiGlobalStatusReg:
        {
            RegOffset = CANRegOffset_HwiGlobalStatusReg;
        }
        break;
        
        case CANRegID_HwiRequestReg:
        {
            RegOffset = CANRegOffset_HwiRequestReg;
        }
        break;
        
        case CANRegID_HwiTxBuffer0TransmitStatusReg:
        {
            RegOffset = CANRegOffset_HwiTxBuffer0TransmitStatusReg;
        }
        break;
        
        case CANRegID_HwiTxBuffer1TransmitStatusReg:
        {
            RegOffset = CANRegOffset_HwiTxBuffer1TransmitStatusReg;
        }
        break;
        
        case CANRegID_HwiTxBuffer2TransmitStatusReg:
        {
            RegOffset = CANRegOffset_HwiTxBuffer2TransmitStatusReg;
        }
        break;
        
        case CANRegID_HwiReceiveStatusReg:
        {
            RegOffset = CANRegOffset_HwiReceiveStatusReg;
        }
        break;
        
        case CANRegID_HwiErrorWarningLimitReg:
        {
            RegOffset = CANRegOffset_HwiErrorWarningLimitReg;
        }
        break;
        
        case CANRegID_HwiErrorCounterReg:
        {
            RegOffset = CANRegOffset_HwiErrorCounterReg;
        }
        break;
        
        case CANRegID_HwiIdentifierIndexReg:
        {
            RegOffset = CANRegOffset_HwiIdentifierIndexReg;
        }
        break;
        
        case CANRegID_HwiIdentifierFilterReg:
        {
            RegOffset = CANRegOffset_HwiIdentifierFilterReg;
        }
        break;
        
        case CANRegID_HwiIdentifierMaskReg:
        {
            RegOffset = CANRegOffset_HwiIdentifierMaskReg;
        }
        break;
        
        case CANRegID_HwiTFI0Reg:
        {
            RegOffset = CANRegOffset_HwiTFI0Reg;
        }
        break;
        
        case CANRegID_HwiTID0Reg:
        {
            RegOffset = CANRegOffset_HwiTID0Reg;
        }
        break;
        
        case CANRegID_HwiTDL0Reg:
        {
            RegOffset = CANRegOffset_HwiTDL0Reg;
        }
        break;
        
        case CANRegID_HwiTDH0Reg:
        {
            RegOffset = CANRegOffset_HwiTDH0Reg;
        }
        break;
        
        case CANRegID_HwiTFI1Reg:
        {
            RegOffset = CANRegOffset_HwiTFI1Reg;
        }
        break;
        
        case CANRegID_HwiTID1Reg:
        {
            RegOffset = CANRegOffset_HwiTID1Reg;
        }
        break;
        
        case CANRegID_HwiTDL1Reg:
        {
            RegOffset = CANRegOffset_HwiTDL1Reg;
        }
        break;
        
        case CANRegID_HwiTDH1Reg:
        {
            RegOffset = CANRegOffset_HwiTDH1Reg;
        }
        break;
        
        case CANRegID_HwiTFI2Reg:
        {
            RegOffset = CANRegOffset_HwiTFI2Reg;
        }
        break;
        
        case CANRegID_HwiTID2Reg:
        {
            RegOffset = CANRegOffset_HwiTID2Reg;
        }
        break;
        
        case CANRegID_HwiTDL2Reg:
        {
            RegOffset = CANRegOffset_HwiTDL2Reg;
        }
        break;
        
        case CANRegID_HwiTDH2Reg:
        {
            RegOffset = CANRegOffset_HwiTDH2Reg;
        }
        break;
        
        case CANRegID_HwiRFIReg:
        {
            RegOffset = CANRegOffset_HwiRFIReg;
        }
        break;
        
        case CANRegID_HwiRIDReg:
        {
            RegOffset = CANRegOffset_HwiRIDReg;
        }
        break;
        
        case CANRegID_HwiRDLReg:
        {
            RegOffset = CANRegOffset_HwiRDLReg;
        }
        break;
        
        case CANRegID_HwiRDHReg:
        {
            RegOffset = CANRegOffset_HwiRDHReg;
        }
        break;
        
        default:
        {
            funcresult = FR_Error;
        }
        break;
    }
    
    if (RegOffset != 0xFFFF)
    {
        BusDeviceIO_Write(
            hMappedRegisters,
            RegOffset,
            CANRegOffset_Size,
            Value
            );
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}
    
FRSTATUS
HwdCAN_GetReg(
    IN      PHwdCAN             hHwdCAN,
    IN      ULONG               RegID,
        OUT PULONG              pValue
    )
{
    FRSTATUS                    funcresult;
    //ULONG                       result;
    
    PMappedBusAddress           hMappedRegisters;
    ULONG                       RegOffset;
    
    //
    // initialize local variables
    //
    
    funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    
    //
    // function body
    //
    
    hMappedRegisters = hHwdCAN->hMappedRegisters;
    
    RegOffset = 0xFFFF;
    
    switch(RegID)
    {
        case CANRegID_HwiGlobalControlReg:
        {
            RegOffset = CANRegOffset_HwiGlobalControlReg;
        }
        break;
        
        case CANRegID_HwiClockPrescalerReg:
        {
            RegOffset = CANRegOffset_HwiClockPrescalerReg;
        }
        break;
        
        case CANRegID_HwiBusTimingReg:
        {
            RegOffset = CANRegOffset_HwiBusTimingReg;
        }
        break;
        
        case CANRegID_HwiInterruptEnableReg:
        {
            RegOffset = CANRegOffset_HwiInterruptEnableReg;
        }
        break;
        
        case CANRegID_HwiInterruptStatusReg:
        {
            RegOffset = CANRegOffset_HwiInterruptStatusReg;
        }
        break;
        
        case CANRegID_HwiGlobalStatusReg:
        {
            RegOffset = CANRegOffset_HwiGlobalStatusReg;
        }
        break;
        
        case CANRegID_HwiRequestReg:
        {
            RegOffset = CANRegOffset_HwiRequestReg;
        }
        break;
        
        case CANRegID_HwiTxBuffer0TransmitStatusReg:
        {
            RegOffset = CANRegOffset_HwiTxBuffer0TransmitStatusReg;
        }
        break;
        
        case CANRegID_HwiTxBuffer1TransmitStatusReg:
        {
            RegOffset = CANRegOffset_HwiTxBuffer1TransmitStatusReg;
        }
        break;
        
        case CANRegID_HwiTxBuffer2TransmitStatusReg:
        {
            RegOffset = CANRegOffset_HwiTxBuffer2TransmitStatusReg;
        }
        break;
        
        case CANRegID_HwiReceiveStatusReg:
        {
            RegOffset = CANRegOffset_HwiReceiveStatusReg;
        }
        break;
        
        case CANRegID_HwiErrorWarningLimitReg:
        {
            RegOffset = CANRegOffset_HwiErrorWarningLimitReg;
        }
        break;
        
        case CANRegID_HwiErrorCounterReg:
        {
            RegOffset = CANRegOffset_HwiErrorCounterReg;
        }
        break;
        
        case CANRegID_HwiIdentifierIndexReg:
        {
            RegOffset = CANRegOffset_HwiIdentifierIndexReg;
        }
        break;
        
        case CANRegID_HwiIdentifierFilterReg:
        {
            RegOffset = CANRegOffset_HwiIdentifierFilterReg;
        }
        break;
        
        case CANRegID_HwiIdentifierMaskReg:
        {
            RegOffset = CANRegOffset_HwiIdentifierMaskReg;
        }
        break;
        
        case CANRegID_HwiTFI0Reg:
        {
            RegOffset = CANRegOffset_HwiTFI0Reg;
        }
        break;
        
        case CANRegID_HwiTID0Reg:
        {
            RegOffset = CANRegOffset_HwiTID0Reg;
        }
        break;
        
        case CANRegID_HwiTDL0Reg:
        {
            RegOffset = CANRegOffset_HwiTDL0Reg;
        }
        break;
        
        case CANRegID_HwiTDH0Reg:
        {
            RegOffset = CANRegOffset_HwiTDH0Reg;
        }
        break;
        
        case CANRegID_HwiTFI1Reg:
        {
            RegOffset = CANRegOffset_HwiTFI1Reg;
        }
        break;
        
        case CANRegID_HwiTID1Reg:
        {
            RegOffset = CANRegOffset_HwiTID1Reg;
        }
        break;
        
        case CANRegID_HwiTDL1Reg:
        {
            RegOffset = CANRegOffset_HwiTDL1Reg;
        }
        break;
        
        case CANRegID_HwiTDH1Reg:
        {
            RegOffset = CANRegOffset_HwiTDH1Reg;
        }
        break;
        
        case CANRegID_HwiTFI2Reg:
        {
            RegOffset = CANRegOffset_HwiTFI2Reg;
        }
        break;
        
        case CANRegID_HwiTID2Reg:
        {
            RegOffset = CANRegOffset_HwiTID2Reg;
        }
        break;
        
        case CANRegID_HwiTDL2Reg:
        {
            RegOffset = CANRegOffset_HwiTDL2Reg;
        }
        break;
        
        case CANRegID_HwiTDH2Reg:
        {
            RegOffset = CANRegOffset_HwiTDH2Reg;
        }
        break;
        
        case CANRegID_HwiRFIReg:
        {
            RegOffset = CANRegOffset_HwiRFIReg;
        }
        break;
        
        case CANRegID_HwiRIDReg:
        {
            RegOffset = CANRegOffset_HwiRIDReg;
        }
        break;
        
        case CANRegID_HwiRDLReg:
        {
            RegOffset = CANRegOffset_HwiRDLReg;
        }
        break;
        
        case CANRegID_HwiRDHReg:
        {
            RegOffset = CANRegOffset_HwiRDHReg;
        }
        break;
        
        default:
        {
            funcresult = FR_Error;
        }
        break;
    }
    
    if (RegOffset != 0xFFFF)
    {
        BusDeviceIO_Read(
            hMappedRegisters,
            RegOffset,
            CANRegOffset_Size,
            pValue
            );
    }
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    return funcresult;
}
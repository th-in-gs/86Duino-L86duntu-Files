#include <linux/kernel.h>

#include "SPtype.h"
#include "funcresult.h"


//#include "SystemPorting.h"
#include "BusDeviceIO.h"

#include "HwiCAN.h"

// in Hwi level, we dont check the result of BusDeviceIO_func.

VOID
_SetHwiGlobalControlReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiGlobalControlReg         pHwiGlobalControlReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiGlobalControlReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiGlobalControlReg_Reset, pHwiGlobalControlReg->Reset);
    _SetDataField(Value, HwiGlobalControlReg_Active, pHwiGlobalControlReg->Active);
    _SetDataField(Value, HwiGlobalControlReg_Silent, pHwiGlobalControlReg->Silent);
    _SetDataField(Value, HwiGlobalControlReg_Loopback, pHwiGlobalControlReg->Loopback);
    _SetDataField(Value, HwiGlobalControlReg_TransmitWithNoAck, pHwiGlobalControlReg->TransmitWithNoAck);
    _SetDataField(Value, HwiGlobalControlReg_SelfReception, pHwiGlobalControlReg->SelfReception);
    _SetDataField(Value, HwiGlobalControlReg_TransmitPriority, pHwiGlobalControlReg->TransmitPriority);
    _SetDataField(Value, HwiGlobalControlReg_ArbitrationLostRetry, pHwiGlobalControlReg->ArbitrationLostRetry);
    _SetDataField(Value, HwiGlobalControlReg_BusErrorRetry, pHwiGlobalControlReg->BusErrorRetry);
    _SetDataField(Value, HwiGlobalControlReg_IdentifierReset, pHwiGlobalControlReg->IdentifierReset);
    _SetDataField(Value, HwiGlobalControlReg_IdentifierEnable, pHwiGlobalControlReg->IdentifierEnable);
    _SetDataField(Value, HwiGlobalControlReg_IdentifierBypass, pHwiGlobalControlReg->IdentifierBypass);
    _SetDataField(Value, HwiGlobalControlReg_PowerSaving, pHwiGlobalControlReg->PowerSaving);
    _SetDataField(Value, HwiGlobalControlReg_RxBusError, pHwiGlobalControlReg->RxBusError);
    
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiGlobalControlReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiGlobalControlReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiGlobalControlReg         pHwiGlobalControlReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiGlobalControlReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiGlobalControlReg_Reset, pHwiGlobalControlReg->Reset);
    _GetDataField(Value, HwiGlobalControlReg_Active, pHwiGlobalControlReg->Active);
    _GetDataField(Value, HwiGlobalControlReg_Silent, pHwiGlobalControlReg->Silent);
    _GetDataField(Value, HwiGlobalControlReg_Loopback, pHwiGlobalControlReg->Loopback);
    _GetDataField(Value, HwiGlobalControlReg_TransmitWithNoAck, pHwiGlobalControlReg->TransmitWithNoAck);
    _GetDataField(Value, HwiGlobalControlReg_SelfReception, pHwiGlobalControlReg->SelfReception);
    _GetDataField(Value, HwiGlobalControlReg_TransmitPriority, pHwiGlobalControlReg->TransmitPriority);
    _GetDataField(Value, HwiGlobalControlReg_ArbitrationLostRetry, pHwiGlobalControlReg->ArbitrationLostRetry);
    _GetDataField(Value, HwiGlobalControlReg_BusErrorRetry, pHwiGlobalControlReg->BusErrorRetry);
    _GetDataField(Value, HwiGlobalControlReg_IdentifierReset, pHwiGlobalControlReg->IdentifierReset);
    _GetDataField(Value, HwiGlobalControlReg_IdentifierEnable, pHwiGlobalControlReg->IdentifierEnable);
    _GetDataField(Value, HwiGlobalControlReg_IdentifierBypass, pHwiGlobalControlReg->IdentifierBypass);
    _GetDataField(Value, HwiGlobalControlReg_PowerSaving, pHwiGlobalControlReg->PowerSaving);
    _GetDataField(Value, HwiGlobalControlReg_RxBusError, pHwiGlobalControlReg->RxBusError);
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiClockPrescalerReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiClockPrescalerReg        pHwiClockPrescalerReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiClockPrescalerReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiClockPrescalerReg_CKDIV, pHwiClockPrescalerReg->CKDIV);
    _SetDataField(Value, HwiClockPrescalerReg_ExternalInput, pHwiClockPrescalerReg->ExternalInput);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiClockPrescalerReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiClockPrescalerReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiClockPrescalerReg        pHwiClockPrescalerReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiClockPrescalerReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiClockPrescalerReg_CKDIV, pHwiClockPrescalerReg->CKDIV);
    _GetDataField(Value, HwiClockPrescalerReg_ExternalInput, pHwiClockPrescalerReg->ExternalInput);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiBusTimingReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiBusTimingReg             pHwiBusTimingReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiBusTimingReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiBusTimingReg_PropagationSEG, pHwiBusTimingReg->PropagationSEG);
    _SetDataField(Value, HwiBusTimingReg_PhaseSEG1, pHwiBusTimingReg->PhaseSEG1);
    _SetDataField(Value, HwiBusTimingReg_PhaseSEG2, pHwiBusTimingReg->PhaseSEG2);
    _SetDataField(Value, HwiBusTimingReg_SJW, pHwiBusTimingReg->SJW);
    _SetDataField(Value, HwiBusTimingReg_Sampling, pHwiBusTimingReg->Sampling);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiBusTimingReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiBusTimingReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiBusTimingReg             pHwiBusTimingReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiBusTimingReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiBusTimingReg_PropagationSEG, pHwiBusTimingReg->PropagationSEG);
    _GetDataField(Value, HwiBusTimingReg_PhaseSEG1, pHwiBusTimingReg->PhaseSEG1);
    _GetDataField(Value, HwiBusTimingReg_PhaseSEG2, pHwiBusTimingReg->PhaseSEG2);
    _GetDataField(Value, HwiBusTimingReg_SJW, pHwiBusTimingReg->SJW);
    _GetDataField(Value, HwiBusTimingReg_Sampling, pHwiBusTimingReg->Sampling);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiInterruptEnableReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiInterruptEnableReg       pHwiInterruptEnableReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiInterruptEnableReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiInterruptEnableReg_Receive, pHwiInterruptEnableReg->Receive);
    _SetDataField(Value, HwiInterruptEnableReg_TxBuffer0Transmit, pHwiInterruptEnableReg->TxBuffer0Transmit);
    _SetDataField(Value, HwiInterruptEnableReg_TxBuffer1Transmit, pHwiInterruptEnableReg->TxBuffer1Transmit);
    _SetDataField(Value, HwiInterruptEnableReg_TxBuffer2Transmit, pHwiInterruptEnableReg->TxBuffer2Transmit);
    _SetDataField(Value, HwiInterruptEnableReg_ErrorWarning, pHwiInterruptEnableReg->ErrorWarning);
    _SetDataField(Value, HwiInterruptEnableReg_ErrorPassive, pHwiInterruptEnableReg->ErrorPassive);
    _SetDataField(Value, HwiInterruptEnableReg_BusOff, pHwiInterruptEnableReg->BusOff);
    _SetDataField(Value, HwiInterruptEnableReg_ArbitrationLost, pHwiInterruptEnableReg->ArbitrationLost);
    _SetDataField(Value, HwiInterruptEnableReg_RxBusError, pHwiInterruptEnableReg->RxBusError);
    _SetDataField(Value, HwiInterruptEnableReg_RxFIFOOverrun, pHwiInterruptEnableReg->RxFIFOOverrun);
    _SetDataField(Value, HwiInterruptEnableReg_PowerSavingExit, pHwiInterruptEnableReg->PowerSavingExit);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiInterruptEnableReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiInterruptEnableReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiInterruptEnableReg       pHwiInterruptEnableReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiInterruptEnableReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiInterruptEnableReg_Receive, pHwiInterruptEnableReg->Receive);
    _GetDataField(Value, HwiInterruptEnableReg_TxBuffer0Transmit, pHwiInterruptEnableReg->TxBuffer0Transmit);
    _GetDataField(Value, HwiInterruptEnableReg_TxBuffer1Transmit, pHwiInterruptEnableReg->TxBuffer1Transmit);
    _GetDataField(Value, HwiInterruptEnableReg_TxBuffer2Transmit, pHwiInterruptEnableReg->TxBuffer2Transmit);
    _GetDataField(Value, HwiInterruptEnableReg_ErrorWarning, pHwiInterruptEnableReg->ErrorWarning);
    _GetDataField(Value, HwiInterruptEnableReg_ErrorPassive, pHwiInterruptEnableReg->ErrorPassive);
    _GetDataField(Value, HwiInterruptEnableReg_BusOff, pHwiInterruptEnableReg->BusOff);
    _GetDataField(Value, HwiInterruptEnableReg_ArbitrationLost, pHwiInterruptEnableReg->ArbitrationLost);
    _GetDataField(Value, HwiInterruptEnableReg_RxBusError, pHwiInterruptEnableReg->RxBusError);
    _GetDataField(Value, HwiInterruptEnableReg_RxFIFOOverrun, pHwiInterruptEnableReg->RxFIFOOverrun);
    _GetDataField(Value, HwiInterruptEnableReg_PowerSavingExit, pHwiInterruptEnableReg->PowerSavingExit);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiInterruptStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiInterruptStatusReg       pHwiInterruptStatusReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiInterruptStatusReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiInterruptStatusReg_Receive, pHwiInterruptStatusReg->Receive);
    _SetDataField(Value, HwiInterruptStatusReg_TxBuffer0Transmit, pHwiInterruptStatusReg->TxBuffer0Transmit);
    _SetDataField(Value, HwiInterruptStatusReg_TxBuffer1Transmit, pHwiInterruptStatusReg->TxBuffer1Transmit);
    _SetDataField(Value, HwiInterruptStatusReg_TxBuffer2Transmit, pHwiInterruptStatusReg->TxBuffer2Transmit);
    _SetDataField(Value, HwiInterruptStatusReg_ErrorWarning, pHwiInterruptStatusReg->ErrorWarning);
    _SetDataField(Value, HwiInterruptStatusReg_ErrorPassive, pHwiInterruptStatusReg->ErrorPassive);
    _SetDataField(Value, HwiInterruptStatusReg_BusOff, pHwiInterruptStatusReg->BusOff);
    _SetDataField(Value, HwiInterruptStatusReg_ArbitrationLost, pHwiInterruptStatusReg->ArbitrationLost);
    _SetDataField(Value, HwiInterruptStatusReg_RxBusError, pHwiInterruptStatusReg->RxBusError);
    _SetDataField(Value, HwiInterruptStatusReg_RxFIFOOverrun, pHwiInterruptStatusReg->RxFIFOOverrun);
    _SetDataField(Value, HwiInterruptStatusReg_PowerSavingExit, pHwiInterruptStatusReg->PowerSavingExit);
    _SetDataField(Value, HwiInterruptStatusReg_BusErrorDirection, pHwiInterruptStatusReg->BusErrorDirection);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiInterruptStatusReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiInterruptStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiInterruptStatusReg       pHwiInterruptStatusReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiInterruptStatusReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiInterruptStatusReg_Receive, pHwiInterruptStatusReg->Receive);
    _GetDataField(Value, HwiInterruptStatusReg_TxBuffer0Transmit, pHwiInterruptStatusReg->TxBuffer0Transmit);
    _GetDataField(Value, HwiInterruptStatusReg_TxBuffer1Transmit, pHwiInterruptStatusReg->TxBuffer1Transmit);
    _GetDataField(Value, HwiInterruptStatusReg_TxBuffer2Transmit, pHwiInterruptStatusReg->TxBuffer2Transmit);
    _GetDataField(Value, HwiInterruptStatusReg_ErrorWarning, pHwiInterruptStatusReg->ErrorWarning);
    _GetDataField(Value, HwiInterruptStatusReg_ErrorPassive, pHwiInterruptStatusReg->ErrorPassive);
    _GetDataField(Value, HwiInterruptStatusReg_BusOff, pHwiInterruptStatusReg->BusOff);
    _GetDataField(Value, HwiInterruptStatusReg_ArbitrationLost, pHwiInterruptStatusReg->ArbitrationLost);
    _GetDataField(Value, HwiInterruptStatusReg_RxBusError, pHwiInterruptStatusReg->RxBusError);
    _GetDataField(Value, HwiInterruptStatusReg_RxFIFOOverrun, pHwiInterruptStatusReg->RxFIFOOverrun);
    _GetDataField(Value, HwiInterruptStatusReg_PowerSavingExit, pHwiInterruptStatusReg->PowerSavingExit);
    _GetDataField(Value, HwiInterruptStatusReg_BusErrorDirection, pHwiInterruptStatusReg->BusErrorDirection);
        
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiGlobalStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiGlobalStatusReg          pHwiGlobalStatusReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiGlobalStatusReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiGlobalStatusReg_ReceivingMessage, pHwiGlobalStatusReg->ReceivingMessage);
    _GetDataField(Value, HwiGlobalStatusReg_TransmittingMessage, pHwiGlobalStatusReg->TransmittingMessage);
    _GetDataField(Value, HwiGlobalStatusReg_BusOff, pHwiGlobalStatusReg->BusOff);
    _GetDataField(Value, HwiGlobalStatusReg_PowerSavingState, pHwiGlobalStatusReg->PowerSavingState);
    _GetDataField(Value, HwiGlobalStatusReg_ErrorPassiveState, pHwiGlobalStatusReg->ErrorPassiveState);
    _GetDataField(Value, HwiGlobalStatusReg_ErrorCounterWarning, pHwiGlobalStatusReg->ErrorCounterWarning);
    _GetDataField(Value, HwiGlobalStatusReg_ControllerState, pHwiGlobalStatusReg->ControllerState);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiRequestReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiRequestReg               pHwiRequestReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiRequestReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiRequestReg_TransmitTxBuffer0, pHwiRequestReg->TransmitTxBuffer0);
    _SetDataField(Value, HwiRequestReg_AbortTxBuffer0, pHwiRequestReg->AbortTxBuffer0);
    _SetDataField(Value, HwiRequestReg_TransmitTxBuffer1, pHwiRequestReg->TransmitTxBuffer1);
    _SetDataField(Value, HwiRequestReg_AbortTxBuffer1, pHwiRequestReg->AbortTxBuffer1);
    _SetDataField(Value, HwiRequestReg_TransmitTxBuffer2, pHwiRequestReg->TransmitTxBuffer2);
    _SetDataField(Value, HwiRequestReg_AbortTxBuffer2, pHwiRequestReg->AbortTxBuffer2);
    _SetDataField(Value, HwiRequestReg_ReleaseReceivedMessage, pHwiRequestReg->ReleaseReceivedMessage);
    
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiRequestReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiRequestReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRequestReg               pHwiRequestReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiRequestReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiRequestReg_TransmitTxBuffer0, pHwiRequestReg->TransmitTxBuffer0);
    _GetDataField(Value, HwiRequestReg_AbortTxBuffer0, pHwiRequestReg->AbortTxBuffer0);
    _GetDataField(Value, HwiRequestReg_TransmitTxBuffer1, pHwiRequestReg->TransmitTxBuffer1);
    _GetDataField(Value, HwiRequestReg_AbortTxBuffer1, pHwiRequestReg->AbortTxBuffer1);
    _GetDataField(Value, HwiRequestReg_TransmitTxBuffer2, pHwiRequestReg->TransmitTxBuffer2);
    _GetDataField(Value, HwiRequestReg_AbortTxBuffer2, pHwiRequestReg->AbortTxBuffer2);
    _GetDataField(Value, HwiRequestReg_ReleaseReceivedMessage, pHwiRequestReg->ReleaseReceivedMessage);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiTxBufferTransmitStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
    IN      PHwiTxBufferTransmitStatusReg        pHwiTxBufferTransmitStatusReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTxBuffer0TransmitStatusReg + TxBufferIndex*4,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_RequestCompleted, pHwiTxBufferTransmitStatusReg->RequestCompleted);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_RequestAborted, pHwiTxBufferTransmitStatusReg->RequestAborted);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_TransmissionComplete, pHwiTxBufferTransmitStatusReg->TransmissionComplete);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_ArbitrationLost, pHwiTxBufferTransmitStatusReg->ArbitrationLost);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_BusError, pHwiTxBufferTransmitStatusReg->BusError);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_BusOff, pHwiTxBufferTransmitStatusReg->BusOff);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_ArbitrationLostLocation, pHwiTxBufferTransmitStatusReg->ArbitrationLostLocation);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_BusErrorType, pHwiTxBufferTransmitStatusReg->BusErrorType);
    _SetDataField(Value, HwiTxBufferTransmitStatusReg_BusErrorLocation, pHwiTxBufferTransmitStatusReg->BusErrorLocation);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiTxBuffer0TransmitStatusReg + TxBufferIndex*4,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiTxBufferTransmitStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
        OUT PHwiTxBufferTransmitStatusReg        pHwiTxBufferTransmitStatusReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTxBuffer0TransmitStatusReg + TxBufferIndex*4,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_RequestCompleted, pHwiTxBufferTransmitStatusReg->RequestCompleted);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_RequestAborted, pHwiTxBufferTransmitStatusReg->RequestAborted);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_TransmissionComplete, pHwiTxBufferTransmitStatusReg->TransmissionComplete);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_ArbitrationLost, pHwiTxBufferTransmitStatusReg->ArbitrationLost);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_BusError, pHwiTxBufferTransmitStatusReg->BusError);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_BusOff, pHwiTxBufferTransmitStatusReg->BusOff);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_ArbitrationLostLocation, pHwiTxBufferTransmitStatusReg->ArbitrationLostLocation);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_BusErrorType, pHwiTxBufferTransmitStatusReg->BusErrorType);
    _GetDataField(Value, HwiTxBufferTransmitStatusReg_BusErrorLocation, pHwiTxBufferTransmitStatusReg->BusErrorLocation);
    
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiReceiveStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiReceiveStatusReg         pHwiReceiveStatusReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiReceiveStatusReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiReceiveStatusReg_RxFIFOPendingMessages, pHwiReceiveStatusReg->RxFIFOPendingMessages);
    _GetDataField(Value, HwiReceiveStatusReg_RxFIFOOverrun, pHwiReceiveStatusReg->RxFIFOOverrun);
    _GetDataField(Value, HwiReceiveStatusReg_BusError, pHwiReceiveStatusReg->BusError);
    _GetDataField(Value, HwiReceiveStatusReg_BusErrorType, pHwiReceiveStatusReg->BusErrorType);
    _GetDataField(Value, HwiReceiveStatusReg_BusErrorLocation, pHwiReceiveStatusReg->BusErrorLocation);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiErrorWarningLimitReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiErrorWarningLimitReg     pHwiErrorWarningLimitReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiErrorWarningLimitReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiErrorWarningLimitReg_WarningLimit, pHwiErrorWarningLimitReg->WarningLimit);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiErrorWarningLimitReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiErrorWarningLimitReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiErrorWarningLimitReg     pHwiErrorWarningLimitReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiErrorWarningLimitReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiErrorWarningLimitReg_WarningLimit, pHwiErrorWarningLimitReg->WarningLimit);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiErrorCounterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiErrorCounterReg          pHwiErrorCounterReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiErrorCounterReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiErrorCounterReg_TxErrorCounter, pHwiErrorCounterReg->TxErrorCounter);
    _SetDataField(Value, HwiErrorCounterReg_RxErrorCounter, pHwiErrorCounterReg->RxErrorCounter);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiErrorCounterReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiErrorCounterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiErrorCounterReg          pHwiErrorCounterReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiErrorCounterReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiErrorCounterReg_TxErrorCounter, pHwiErrorCounterReg->TxErrorCounter);
    _GetDataField(Value, HwiErrorCounterReg_RxErrorCounter, pHwiErrorCounterReg->RxErrorCounter);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiIdentifierIndexReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiIdentifierIndexReg       pHwiIdentifierIndexReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierIndexReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiIdentifierIndexReg_FilterIndex, pHwiIdentifierIndexReg->FilterIndex);
    _SetDataField(Value, HwiIdentifierIndexReg_FilterUpdate, pHwiIdentifierIndexReg->FilterUpdate);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierIndexReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiIdentifierIndexReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiIdentifierIndexReg       pHwiIdentifierIndexReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierIndexReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiIdentifierIndexReg_FilterIndex, pHwiIdentifierIndexReg->FilterIndex);
    _GetDataField(Value, HwiIdentifierIndexReg_FilterUpdate, pHwiIdentifierIndexReg->FilterUpdate);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiIdentifierFilterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiIdentifierFilterReg      pHwiIdentifierFilterReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierFilterReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiIdentifierFilterReg_ID, pHwiIdentifierFilterReg->ID);
    _SetDataField(Value, HwiIdentifierFilterReg_ExtendedID, pHwiIdentifierFilterReg->ExtendedID);
    _SetDataField(Value, HwiIdentifierFilterReg_Enable, pHwiIdentifierFilterReg->Enable);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierFilterReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiIdentifierFilterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiIdentifierFilterReg      pHwiIdentifierFilterReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierFilterReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiIdentifierFilterReg_ID, pHwiIdentifierFilterReg->ID);
    _GetDataField(Value, HwiIdentifierFilterReg_ExtendedID, pHwiIdentifierFilterReg->ExtendedID);
    _GetDataField(Value, HwiIdentifierFilterReg_Enable, pHwiIdentifierFilterReg->Enable);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiIdentifierMaskReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiIdentifierMaskReg        pHwiIdentifierMaskReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierMaskReg,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiIdentifierMaskReg_Mask, pHwiIdentifierMaskReg->Mask);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierMaskReg,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiIdentifierMaskReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiIdentifierMaskReg        pHwiIdentifierMaskReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiIdentifierMaskReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiIdentifierMaskReg_Mask, pHwiIdentifierMaskReg->Mask);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiTFIReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
    IN      PHwiTFIReg                   pHwiTFIReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTFI0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiTFIReg_ExtendedFrame, pHwiTFIReg->ExtendedFrame);
    _SetDataField(Value, HwiTFIReg_RTRBit, pHwiTFIReg->RTRBit);
    _SetDataField(Value, HwiTFIReg_DataLengthCode, pHwiTFIReg->DataLengthCode);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiTFI0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiTFIReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
        OUT PHwiTFIReg                   pHwiTFIReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTFI0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiTFIReg_ExtendedFrame, pHwiTFIReg->ExtendedFrame);
    _GetDataField(Value, HwiTFIReg_RTRBit, pHwiTFIReg->RTRBit);
    _GetDataField(Value, HwiTFIReg_DataLengthCode, pHwiTFIReg->DataLengthCode);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiTIDReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
    IN      PHwiTIDReg                   pHwiTIDReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTID0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    _SetDataField(Value, HwiTIDReg_ExtendedID, pHwiTIDReg->ID);
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiTID0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiTIDReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
        OUT PHwiTIDReg                   pHwiTIDReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTID0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiTIDReg_ExtendedID, pHwiTIDReg->ID);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiTDLReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
    IN      PHwiTDLReg                   pHwiTDLReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTDL0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    Value = pHwiTDLReg->Data;
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiTDL0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiTDLReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
        OUT PHwiTDLReg                   pHwiTDLReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTDL0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    
    pHwiTDLReg->Data = Value;
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_SetHwiTDHReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
    IN      PHwiTDHReg                   pHwiTDHReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTDH0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    Value = pHwiTDHReg->Data;
    
    BusDeviceIO_Write(
        hMappedCANRegisters,
        CANRegOffset_HwiTDH0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        Value
        );
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiTDHReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
        OUT PHwiTDHReg                   pHwiTDHReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiTDH0Reg + TxBufferIndex*16,
        CANRegOffset_Size,
        &Value
        );
    
    
    pHwiTDHReg->Data = Value;
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiRFIReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRFIReg                   pHwiRFIReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiRFIReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiRFIReg_ExtendedFrame, pHwiRFIReg->ExtendedFrame);
    _GetDataField(Value, HwiRFIReg_RTRBit, pHwiRFIReg->RTRBit);
    _GetDataField(Value, HwiRFIReg_DataLengthCode, pHwiRFIReg->DataLengthCode);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiRIDReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRIDReg                   pHwiRIDReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiRIDReg,
        CANRegOffset_Size,
        &Value
        );
    
    _GetDataField(Value, HwiRIDReg_ExtendedID, pHwiRIDReg->ID);
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiRDLReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRDLReg                   pHwiRDLReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiRDLReg,
        CANRegOffset_Size,
        &Value
        );
    
    pHwiRDLReg->Data = Value;
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}


VOID
_GetHwiRDHReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRDHReg                   pHwiRDHReg
    )
{
    //FRSTATUS                    funcresult;
    //ULONG                       result;
    
    ULONG                       Value;
    
    //
    // initialize local variables
    //
    
    //funcresult = FR_Success;;
    
    //
    // check parameters
    //
    
    // at the Hwi level, we dont validate the input parameter.
    //if (hMappedRegisters == NULL)
    //{
        //funcFR_Error;
        //goto funcexit;
    //}
    
    //
    // function body
    //
    
    Value = 0;
    
    BusDeviceIO_Read(
        hMappedCANRegisters,
        CANRegOffset_HwiRDHReg,
        CANRegOffset_Size,
        &Value
        );
    
    pHwiRDHReg->Data = Value;
    
    
    
    goto funcexit;
funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
    return ;
}






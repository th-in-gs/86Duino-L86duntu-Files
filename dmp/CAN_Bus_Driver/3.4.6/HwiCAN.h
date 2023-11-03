#ifndef HwiCAN_H
#define HwiCAN_H

//----
// define Data Field Access
//

// Data can be 8bit 16bit, 32bit
// FieldDefine is bit field mask, Value Range and location.
// FieldDefine_Range is value range.
// FieldDefine_Shift is bit location.

#define _SetDataField(Data, FieldDefine, Value) \
    Data = (Data & ~ FieldDefine ) | ((Value & FieldDefine##_Range) << FieldDefine##_Shift);
    
    // if Data is register. the following code have some bug.
    // because register value should alter one time, not two assign.
    //Data &= ~##FieldDefine##;
    //Data |= ((Value) & ##FieldDefine##_Range) << ##FieldDefine##_Shift;
    
#define _GetDataField(Data, FieldDefine, Variable) \
    Variable = ((Data) & FieldDefine) >> FieldDefine##_Shift;
    
#define _OrDataField(Data, FieldDefine, Value) \
    Data |= ((Value) & FieldDefine##_Range) << FieldDefine##_Shift;

#define _SetPointerField(Data, FieldDefine, Pointer) \
    Data = (Data & ~ FieldDefine) | (((Pointer >> FieldDefine##_Shift) & FieldDefine##_Range) << FieldDefine##_Shift);
    //Data &= ~##FieldDefine##;
    //Data |= ((Pointer>>##FieldDefine##_Shift) & ##FieldDefine##_Range) << ##FieldDefine##_Shift;
    
#define _GetPointerField(Data, FieldDefine, Variable) \
    Variable = ((Data) & FieldDefine );

//----
//  Register Interface
//

#define CAN_MaskStardardID  0x000007FF
#define CAN_MaskExtendedID  0x1FFFFFFF

#define CANRegisters_Length                                     128

#define CANRegOffset_Size                                       4

#define CANRegOffset_HwiGlobalControlReg                      0
#define CANRegOffset_HwiClockPrescalerReg                     4
#define CANRegOffset_HwiBusTimingReg                          8
#define CANRegOffset_HwiInterruptEnableReg                    12
#define CANRegOffset_HwiInterruptStatusReg                    16
#define CANRegOffset_HwiGlobalStatusReg                       20
#define CANRegOffset_HwiRequestReg                            24
#define CANRegOffset_HwiTxBuffer0TransmitStatusReg            28
#define CANRegOffset_HwiTxBuffer1TransmitStatusReg            32
#define CANRegOffset_HwiTxBuffer2TransmitStatusReg            36
#define CANRegOffset_HwiReceiveStatusReg                      40
#define CANRegOffset_HwiErrorWarningLimitReg                  44
#define CANRegOffset_HwiErrorCounterReg                       48
#define CANRegOffset_HwiIdentifierIndexReg                    52
#define CANRegOffset_HwiIdentifierFilterReg                   56
#define CANRegOffset_HwiIdentifierMaskReg                     60
#define CANRegOffset_HwiTFI0Reg                               64
#define CANRegOffset_HwiTID0Reg                               68
#define CANRegOffset_HwiTDL0Reg                               72
#define CANRegOffset_HwiTDH0Reg                               76
#define CANRegOffset_HwiTFI1Reg                               80
#define CANRegOffset_HwiTID1Reg                               84
#define CANRegOffset_HwiTDL1Reg                               88
#define CANRegOffset_HwiTDH1Reg                               92
#define CANRegOffset_HwiTFI2Reg                               96
#define CANRegOffset_HwiTID2Reg                               100
#define CANRegOffset_HwiTDL2Reg                               104
#define CANRegOffset_HwiTDH2Reg                               108
#define CANRegOffset_HwiRFIReg                                112
#define CANRegOffset_HwiRIDReg                                116
#define CANRegOffset_HwiRDLReg                                120
#define CANRegOffset_HwiRDHReg                                124

#define CANRegID_HwiGlobalControlReg                      0
#define CANRegID_HwiClockPrescalerReg                     1
#define CANRegID_HwiBusTimingReg                          2
#define CANRegID_HwiInterruptEnableReg                    3
#define CANRegID_HwiInterruptStatusReg                    4
#define CANRegID_HwiGlobalStatusReg                       5
#define CANRegID_HwiRequestReg                            6
#define CANRegID_HwiTxBuffer0TransmitStatusReg            7
#define CANRegID_HwiTxBuffer1TransmitStatusReg            8
#define CANRegID_HwiTxBuffer2TransmitStatusReg            9
#define CANRegID_HwiReceiveStatusReg                      10
#define CANRegID_HwiErrorWarningLimitReg                  11
#define CANRegID_HwiErrorCounterReg                       12
#define CANRegID_HwiIdentifierIndexReg                    13
#define CANRegID_HwiIdentifierFilterReg                   14
#define CANRegID_HwiIdentifierMaskReg                     15
#define CANRegID_HwiTFI0Reg                               16
#define CANRegID_HwiTID0Reg                               17
#define CANRegID_HwiTDL0Reg                               18
#define CANRegID_HwiTDH0Reg                               19
#define CANRegID_HwiTFI1Reg                               20
#define CANRegID_HwiTID1Reg                               21
#define CANRegID_HwiTDL1Reg                               22
#define CANRegID_HwiTDH1Reg                               23
#define CANRegID_HwiTFI2Reg                               24
#define CANRegID_HwiTID2Reg                               25
#define CANRegID_HwiTDL2Reg                               26
#define CANRegID_HwiTDH2Reg                               27
#define CANRegID_HwiRFIReg                                28
#define CANRegID_HwiRIDReg                                29
#define CANRegID_HwiRDLReg                                30
#define CANRegID_HwiRDHReg                                31

#define HwiGlobalControlReg_Reset                            0x00000001
#define HwiGlobalControlReg_Active                           0x00000002
#define HwiGlobalControlReg_Silent                           0x00000004
#define HwiGlobalControlReg_Loopback                         0x00000008
#define HwiGlobalControlReg_TransmitWithNoAck                0x00000010
#define HwiGlobalControlReg_SelfReception                    0x00000020
#define HwiGlobalControlReg_TransmitPriority                 0x000000C0
#define HwiGlobalControlReg_ArbitrationLostRetry             0x00000100
#define HwiGlobalControlReg_BusErrorRetry                    0x00000200
#define HwiGlobalControlReg_IdentifierReset                  0x00010000
#define HwiGlobalControlReg_IdentifierEnable                 0x00020000
#define HwiGlobalControlReg_IdentifierBypass                 0x00040000
#define HwiGlobalControlReg_PowerSaving                      0x01000000
#define HwiGlobalControlReg_RxBusError                       0x02000000

#define HwiGlobalControlReg_Reset_Shift                      (0)
#define HwiGlobalControlReg_Active_Shift                     (1)
#define HwiGlobalControlReg_Silent_Shift                     (2)
#define HwiGlobalControlReg_Loopback_Shift                   (3)
#define HwiGlobalControlReg_TransmitWithNoAck_Shift          (4)
#define HwiGlobalControlReg_SelfReception_Shift              (5)
#define HwiGlobalControlReg_TransmitPriority_Shift           (6)
#define HwiGlobalControlReg_ArbitrationLostRetry_Shift       (8)
#define HwiGlobalControlReg_BusErrorRetry_Shift              (9)
#define HwiGlobalControlReg_IdentifierReset_Shift            (16)
#define HwiGlobalControlReg_IdentifierEnable_Shift           (17)
#define HwiGlobalControlReg_IdentifierBypass_Shift           (18)
#define HwiGlobalControlReg_PowerSaving_Shift                (24)
#define HwiGlobalControlReg_RxBusError_Shift                 (25)

#define HwiGlobalControlReg_Reset_Range                      0x00000001
#define HwiGlobalControlReg_Active_Range                     0x00000001
#define HwiGlobalControlReg_Silent_Range                     0x00000001
#define HwiGlobalControlReg_Loopback_Range                   0x00000001
#define HwiGlobalControlReg_TransmitWithNoAck_Range          0x00000001
#define HwiGlobalControlReg_SelfReception_Range              0x00000001
#define HwiGlobalControlReg_TransmitPriority_Range           0x00000003
#define HwiGlobalControlReg_ArbitrationLostRetry_Range       0x00000001
#define HwiGlobalControlReg_BusErrorRetry_Range              0x00000001
#define HwiGlobalControlReg_IdentifierReset_Range            0x00000001
#define HwiGlobalControlReg_IdentifierEnable_Range           0x00000001
#define HwiGlobalControlReg_IdentifierBypass_Range           0x00000001
#define HwiGlobalControlReg_PowerSaving_Range                0x00000001
#define HwiGlobalControlReg_RxBusError_Range                 0x00000001

#define HwiClockPrescalerReg_CKDIV               0x0000FFFF
#define HwiClockPrescalerReg_ExternalInput       0x80000000

#define HwiClockPrescalerReg_CKDIV_Shift         (0)
#define HwiClockPrescalerReg_ExternalInput_Shift (31)

#define HwiClockPrescalerReg_CKDIV_Range         0x0000FFFF
#define HwiClockPrescalerReg_ExternalInput_Range 0x00000001

#define HwiBusTimingReg_PropagationSEG           0x00000007
#define HwiBusTimingReg_PhaseSEG1                0x00000070
#define HwiBusTimingReg_PhaseSEG2                0x00000700
#define HwiBusTimingReg_SJW                      0x00003000
#define HwiBusTimingReg_Sampling                 0x00008000

#define HwiBusTimingReg_PropagationSEG_Shift     (0)
#define HwiBusTimingReg_PhaseSEG1_Shift          (4)
#define HwiBusTimingReg_PhaseSEG2_Shift          (8)
#define HwiBusTimingReg_SJW_Shift                (12)
#define HwiBusTimingReg_Sampling_Shift           (15)

#define HwiBusTimingReg_PropagationSEG_Range     0x00000007
#define HwiBusTimingReg_PhaseSEG1_Range          0x00000007
#define HwiBusTimingReg_PhaseSEG2_Range          0x00000007
#define HwiBusTimingReg_SJW_Range                0x00000003
#define HwiBusTimingReg_Sampling_Range           0x00000001

#define HwiInterruptEnableReg_Receive                        0x00000001
#define HwiInterruptEnableReg_TxBuffer0Transmit              0x00000002
#define HwiInterruptEnableReg_TxBuffer1Transmit              0x00000004
#define HwiInterruptEnableReg_TxBuffer2Transmit              0x00000008
#define HwiInterruptEnableReg_ErrorWarning                   0x00000010
#define HwiInterruptEnableReg_ErrorPassive                   0x00000020
#define HwiInterruptEnableReg_BusOff                         0x00000040
#define HwiInterruptEnableReg_ArbitrationLost                0x00000080
#define HwiInterruptEnableReg_RxBusError                     0x00000100
#define HwiInterruptEnableReg_RxFIFOOverrun                  0x00000200
#define HwiInterruptEnableReg_PowerSavingExit                0x00000400

#define HwiInterruptEnableReg_Receive_Shift                  (0)
#define HwiInterruptEnableReg_TxBuffer0Transmit_Shift        (1)
#define HwiInterruptEnableReg_TxBuffer1Transmit_Shift        (2)
#define HwiInterruptEnableReg_TxBuffer2Transmit_Shift        (3)
#define HwiInterruptEnableReg_ErrorWarning_Shift             (4)
#define HwiInterruptEnableReg_ErrorPassive_Shift             (5)
#define HwiInterruptEnableReg_BusOff_Shift                   (6)
#define HwiInterruptEnableReg_ArbitrationLost_Shift          (7)
#define HwiInterruptEnableReg_RxBusError_Shift               (8)
#define HwiInterruptEnableReg_RxFIFOOverrun_Shift            (9)
#define HwiInterruptEnableReg_PowerSavingExit_Shift          (10)

#define HwiInterruptEnableReg_Receive_Range                  0x00000001
#define HwiInterruptEnableReg_TxBuffer0Transmit_Range        0x00000001
#define HwiInterruptEnableReg_TxBuffer1Transmit_Range        0x00000001
#define HwiInterruptEnableReg_TxBuffer2Transmit_Range        0x00000001
#define HwiInterruptEnableReg_ErrorWarning_Range             0x00000001
#define HwiInterruptEnableReg_ErrorPassive_Range             0x00000001
#define HwiInterruptEnableReg_BusOff_Range                   0x00000001
#define HwiInterruptEnableReg_ArbitrationLost_Range          0x00000001
#define HwiInterruptEnableReg_RxBusError_Range               0x00000001
#define HwiInterruptEnableReg_RxFIFOOverrun_Range            0x00000001
#define HwiInterruptEnableReg_PowerSavingExit_Range          0x00000001

#define HwiInterruptStatusReg_Receive                        0x00000001
#define HwiInterruptStatusReg_TxBuffer0Transmit              0x00000002
#define HwiInterruptStatusReg_TxBuffer1Transmit              0x00000004
#define HwiInterruptStatusReg_TxBuffer2Transmit              0x00000008
#define HwiInterruptStatusReg_ErrorWarning                   0x00000010
#define HwiInterruptStatusReg_ErrorPassive                   0x00000020
#define HwiInterruptStatusReg_BusOff                         0x00000040
#define HwiInterruptStatusReg_ArbitrationLost                0x00000080
#define HwiInterruptStatusReg_RxBusError                     0x00000100
#define HwiInterruptStatusReg_RxFIFOOverrun                  0x00000200
#define HwiInterruptStatusReg_PowerSavingExit                0x00000400
#define HwiInterruptStatusReg_BusErrorDirection              0x00010000

#define HwiInterruptStatusReg_Receive_Shift                  (0)
#define HwiInterruptStatusReg_TxBuffer0Transmit_Shift        (1)
#define HwiInterruptStatusReg_TxBuffer1Transmit_Shift        (2)
#define HwiInterruptStatusReg_TxBuffer2Transmit_Shift        (3)
#define HwiInterruptStatusReg_ErrorWarning_Shift             (4)
#define HwiInterruptStatusReg_ErrorPassive_Shift             (5)
#define HwiInterruptStatusReg_BusOff_Shift                   (6)
#define HwiInterruptStatusReg_ArbitrationLost_Shift          (7)
#define HwiInterruptStatusReg_RxBusError_Shift               (8)
#define HwiInterruptStatusReg_RxFIFOOverrun_Shift            (9)
#define HwiInterruptStatusReg_PowerSavingExit_Shift          (10)
#define HwiInterruptStatusReg_BusErrorDirection_Shift        (16)

#define HwiInterruptStatusReg_Receive_Range                  0x00000001
#define HwiInterruptStatusReg_TxBuffer0Transmit_Range        0x00000001
#define HwiInterruptStatusReg_TxBuffer1Transmit_Range        0x00000001
#define HwiInterruptStatusReg_TxBuffer2Transmit_Range        0x00000001
#define HwiInterruptStatusReg_ErrorWarning_Range             0x00000001
#define HwiInterruptStatusReg_ErrorPassive_Range             0x00000001
#define HwiInterruptStatusReg_BusOff_Range                   0x00000001
#define HwiInterruptStatusReg_ArbitrationLost_Range          0x00000001
#define HwiInterruptStatusReg_RxBusError_Range               0x00000001
#define HwiInterruptStatusReg_RxFIFOOverrun_Range            0x00000001
#define HwiInterruptStatusReg_PowerSavingExit_Range          0x00000001
#define HwiInterruptStatusReg_BusErrorDirection_Range        0x00000001

#define BusErrorDirection_DuringTransmitting    0
#define BusErrorDirection_DuringReceiving       1

#define HwiGlobalStatusReg_ReceivingMessage                  0x00000001
#define HwiGlobalStatusReg_TransmittingMessage               0x00000002
#define HwiGlobalStatusReg_BusOff                            0x00000004
#define HwiGlobalStatusReg_PowerSavingState                  0x00000008
#define HwiGlobalStatusReg_ErrorPassiveState                 0x00000010
#define HwiGlobalStatusReg_ErrorCounterWarning               0x00000020
#define HwiGlobalStatusReg_ControllerState                   0x00000700

#define HwiGlobalStatusReg_ReceivingMessage_Shift            (0)
#define HwiGlobalStatusReg_TransmittingMessage_Shift         (1)
#define HwiGlobalStatusReg_BusOff_Shift                      (2)
#define HwiGlobalStatusReg_PowerSavingState_Shift            (3)
#define HwiGlobalStatusReg_ErrorPassiveState_Shift           (4)
#define HwiGlobalStatusReg_ErrorCounterWarning_Shift         (5)
#define HwiGlobalStatusReg_ControllerState_Shift             (8)

#define HwiGlobalStatusReg_ReceivingMessage_Range            0x00000001
#define HwiGlobalStatusReg_TransmittingMessage_Range         0x00000001
#define HwiGlobalStatusReg_BusOff_Range                      0x00000001
#define HwiGlobalStatusReg_PowerSavingState_Range            0x00000001
#define HwiGlobalStatusReg_ErrorPassiveState_Range           0x00000001
#define HwiGlobalStatusReg_ErrorCounterWarning_Range         0x00000001
#define HwiGlobalStatusReg_ControllerState_Range             0x00000007

#define HwiRequestReg_TransmitTxBuffer0              0x00000001
#define HwiRequestReg_AbortTxBuffer0                 0x00000002
#define HwiRequestReg_TransmitTxBuffer1              0x00000004
#define HwiRequestReg_AbortTxBuffer1                 0x00000008
#define HwiRequestReg_TransmitTxBuffer2              0x00000010
#define HwiRequestReg_AbortTxBuffer2                 0x00000020
#define HwiRequestReg_ReleaseReceivedMessage         0x00000100

#define HwiRequestReg_TransmitTxBuffer0_Shift        (0)
#define HwiRequestReg_AbortTxBuffer0_Shift           (1)
#define HwiRequestReg_TransmitTxBuffer1_Shift        (2)
#define HwiRequestReg_AbortTxBuffer1_Shift           (3)
#define HwiRequestReg_TransmitTxBuffer2_Shift        (4)
#define HwiRequestReg_AbortTxBuffer2_Shift           (5)
#define HwiRequestReg_ReleaseReceivedMessage_Shift   (8)

#define HwiRequestReg_TransmitTxBuffer0_Range        0x00000001
#define HwiRequestReg_AbortTxBuffer0_Range           0x00000001
#define HwiRequestReg_TransmitTxBuffer1_Range        0x00000001
#define HwiRequestReg_AbortTxBuffer1_Range           0x00000001
#define HwiRequestReg_TransmitTxBuffer2_Range        0x00000001
#define HwiRequestReg_AbortTxBuffer2_Range           0x00000001
#define HwiRequestReg_ReleaseReceivedMessage_Range   0x00000001

#define HwiTxBufferTransmitStatusReg_RequestCompleted                0x00000001
#define HwiTxBufferTransmitStatusReg_RequestAborted                  0x00000002
#define HwiTxBufferTransmitStatusReg_TransmissionComplete            0x00000004
#define HwiTxBufferTransmitStatusReg_ArbitrationLost                 0x00000010
#define HwiTxBufferTransmitStatusReg_BusError                        0x00000020
#define HwiTxBufferTransmitStatusReg_BusOff                          0x00000040
#define HwiTxBufferTransmitStatusReg_ArbitrationLostLocation         0x00001F00
#define HwiTxBufferTransmitStatusReg_BusErrorType                    0x00070000
#define HwiTxBufferTransmitStatusReg_BusErrorLocation                0x1F000000

#define HwiTxBufferTransmitStatusReg_RequestCompleted_Shift          (0)
#define HwiTxBufferTransmitStatusReg_RequestAborted_Shift            (1)
#define HwiTxBufferTransmitStatusReg_TransmissionComplete_Shift      (2)
#define HwiTxBufferTransmitStatusReg_ArbitrationLost_Shift           (4)
#define HwiTxBufferTransmitStatusReg_BusError_Shift                  (5)
#define HwiTxBufferTransmitStatusReg_BusOff_Shift                    (6)
#define HwiTxBufferTransmitStatusReg_ArbitrationLostLocation_Shift   (8)
#define HwiTxBufferTransmitStatusReg_BusErrorType_Shift              (16)
#define HwiTxBufferTransmitStatusReg_BusErrorLocation_Shift          (24)

#define HwiTxBufferTransmitStatusReg_RequestCompleted_Range          0x00000001
#define HwiTxBufferTransmitStatusReg_RequestAborted_Range            0x00000001
#define HwiTxBufferTransmitStatusReg_TransmissionComplete_Range      0x00000001
#define HwiTxBufferTransmitStatusReg_ArbitrationLost_Range           0x00000001
#define HwiTxBufferTransmitStatusReg_BusError_Range                  0x00000001
#define HwiTxBufferTransmitStatusReg_BusOff_Range                    0x00000001
#define HwiTxBufferTransmitStatusReg_ArbitrationLostLocation_Range   0x0000001F
#define HwiTxBufferTransmitStatusReg_BusErrorType_Range              0x00000007
#define HwiTxBufferTransmitStatusReg_BusErrorLocation_Range          0x0000001F

#define HwiReceiveStatusReg_RxFIFOPendingMessages            0x00000001
#define HwiReceiveStatusReg_RxFIFOOverrun                    0x00000002
#define HwiReceiveStatusReg_BusError                         0x00000004
#define HwiReceiveStatusReg_BusErrorType                     0x00000070
#define HwiReceiveStatusReg_BusErrorLocation                 0x00001F00

#define HwiReceiveStatusReg_RxFIFOPendingMessages_Shift      (0)
#define HwiReceiveStatusReg_RxFIFOOverrun_Shift              (1)
#define HwiReceiveStatusReg_BusError_Shift                   (2)
#define HwiReceiveStatusReg_BusErrorType_Shift               (4)
#define HwiReceiveStatusReg_BusErrorLocation_Shift           (8)

#define HwiReceiveStatusReg_RxFIFOPendingMessages_Range      0x00000001
#define HwiReceiveStatusReg_RxFIFOOverrun_Range              0x00000001
#define HwiReceiveStatusReg_BusError_Range                   0x00000001
#define HwiReceiveStatusReg_BusErrorType_Range               0x00000007
#define HwiReceiveStatusReg_BusErrorLocation_Range           0x0000001F

#define HwiErrorWarningLimitReg_WarningLimit         0x000000FF

#define HwiErrorWarningLimitReg_WarningLimit_Shift   (0)

#define HwiErrorWarningLimitReg_WarningLimit_Range   0x000000FF

#define HwiErrorCounterReg_TxErrorCounter            0x000001FF
#define HwiErrorCounterReg_RxErrorCounter            0x01FF0000

#define HwiErrorCounterReg_TxErrorCounter_Shift      (0)
#define HwiErrorCounterReg_RxErrorCounter_Shift      (16)

#define HwiErrorCounterReg_TxErrorCounter_Range      0x000001FF
#define HwiErrorCounterReg_RxErrorCounter_Range      0x000001FF

#define HwiIdentifierIndexReg_FilterIndex            0x0000001F
#define HwiIdentifierIndexReg_FilterUpdate           0x00000100

#define HwiIdentifierIndexReg_FilterIndex_Shift      (0)
#define HwiIdentifierIndexReg_FilterUpdate_Shift     (8)

#define HwiIdentifierIndexReg_FilterIndex_Range      0x0000001F
#define HwiIdentifierIndexReg_FilterUpdate_Range     0x00000001

#define HwiIdentifierFilterReg_ID                    0x1FFFFFFF
#define HwiIdentifierFilterReg_ExtendedID            0x40000000
#define HwiIdentifierFilterReg_Enable                0x80000000

#define HwiIdentifierFilterReg_ID_Shift              (0)
#define HwiIdentifierFilterReg_ExtendedID_Shift      (30)
#define HwiIdentifierFilterReg_Enable_Shift          (31)

#define HwiIdentifierFilterReg_ID_Range              0x1FFFFFFF
#define HwiIdentifierFilterReg_ExtendedID_Range      0x00000001
#define HwiIdentifierFilterReg_Enable_Range          0x00000001

#define HwiIdentifierMaskReg_Mask                    0x1FFFFFFF

#define HwiIdentifierMaskReg_Mask_Shift              (0)

#define HwiIdentifierMaskReg_Mask_Range              0x1FFFFFFF

#define HwiTFIReg_ExtendedFrame                  0x00000001
#define HwiTFIReg_RTRBit                         0x00000002
#define HwiTFIReg_DataLengthCode                 0x000000F0

#define HwiTFIReg_ExtendedFrame_Shift            (0)
#define HwiTFIReg_RTRBit_Shift                   (1)
#define HwiTFIReg_DataLengthCode_Shift           (4)

#define HwiTFIReg_ExtendedFrame_Range            0x00000001
#define HwiTFIReg_RTRBit_Range                   0x00000001
#define HwiTFIReg_DataLengthCode_Range           0x0000000F

#define HwiTIDReg_StandardID         0x000007FF
#define HwiTIDReg_ExtendedID         0x1FFFFFFF

#define HwiTIDReg_StandardID_Shift   (0)
#define HwiTIDReg_ExtendedID_Shift   (0)

#define HwiTIDReg_StandardID_Range   0x000007FF
#define HwiTIDReg_ExtendedID_Range   0x1FFFFFFF

#define HwiRFIReg_ExtendedFrame                  0x00000001
#define HwiRFIReg_RTRBit                         0x00000002
#define HwiRFIReg_DataLengthCode                 0x000000F0

#define HwiRFIReg_ExtendedFrame_Shift            (0)
#define HwiRFIReg_RTRBit_Shift                   (1)
#define HwiRFIReg_DataLengthCode_Shift           (4)

#define HwiRFIReg_ExtendedFrame_Range            0x00000001
#define HwiRFIReg_RTRBit_Range                   0x00000001
#define HwiRFIReg_DataLengthCode_Range           0x0000000F

#define HwiRIDReg_StandardID         0x000007FF
#define HwiRIDReg_ExtendedID         0x1FFFFFFF

#define HwiRIDReg_StandardID_Shift   (0)
#define HwiRIDReg_ExtendedID_Shift   (0)

#define HwiRIDReg_StandardID_Range   0x000007FF
#define HwiRIDReg_ExtendedID_Range   0x1FFFFFFF


// HwiGlobalControlReg
typedef struct _HwiGlobalControlReg
{
    ULONG   Reset;                      // R/W1
    ULONG   Active;                     // R/W
    ULONG   Silent;                     // R/W
    ULONG   Loopback;                   // R/W
    ULONG   TransmitWithNoAck;          // R/W
    ULONG   SelfReception;              // R/W
    ULONG   TransmitPriority;           // R/W
    ULONG   ArbitrationLostRetry;       // R/W
    ULONG   BusErrorRetry;              // R/W
    ULONG   IdentifierReset;            // R/W1
    ULONG   IdentifierEnable;           // R/W
    ULONG   IdentifierBypass;           // R/W
    ULONG   PowerSaving;                // R/W
    ULONG   RxBusError;                 // R/W
    
}HwiGlobalControlReg, *PHwiGlobalControlReg;

VOID
_SetHwiGlobalControlReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiGlobalControlReg         pHwiGlobalControlReg
    );

VOID
_GetHwiGlobalControlReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiGlobalControlReg         pHwiGlobalControlReg
    );

// HwiClockPrescalerReg
typedef struct _HwiClockPrescalerReg
{   
    ULONG   CKDIV;          // R/W
    ULONG   ExternalInput;  // R/W
    
}HwiClockPrescalerReg, *PHwiClockPrescalerReg;

VOID
_SetHwiClockPrescalerReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiClockPrescalerReg        pHwiClockPrescalerReg
    );

VOID
_GetHwiClockPrescalerReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiClockPrescalerReg        pHwiClockPrescalerReg
    );


// HwiBusTimingReg
typedef struct _HwiBusTimingReg
{   
    // 8 - 25
    // SYNC + Prop + Seg1 + Seg2
    // 1    + 4    + 1    + 2 = 8
    // 1    + 3    + 2    + 2 = 8
    // 1    + X    + 3    + 3 = 8~15
    // 1    + X    + 4    + 4 = 10~17
    // 1    + X    + 5    + 5 = 12~19
    // 1    + X    + 6    + 6 = 14~21
    // 1    + X    + 7    + 7 = 16~23
    // 1    + X    + 8    + 8 = 18~25
    // 1    + 8    + 8    + 8 = 25
    
    ULONG   PropagationSEG; // R/W
    ULONG   PhaseSEG1;      // R/W
    ULONG   PhaseSEG2;      // R/W
    ULONG   SJW;            // R/W
    ULONG   Sampling;       // R/W
    
}HwiBusTimingReg, *PHwiBusTimingReg;

VOID
_SetHwiBusTimingReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiBusTimingReg             pHwiBusTimingReg
    );

VOID
_GetHwiBusTimingReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiBusTimingReg             pHwiBusTimingReg
    );


// HwiInterruptEnableReg
typedef struct _HwiInterruptEnableReg
{   
    ULONG   Receive;                    // R/W
    ULONG   TxBuffer0Transmit;          // R/W
    ULONG   TxBuffer1Transmit;          // R/W
    ULONG   TxBuffer2Transmit;          // R/W
    ULONG   ErrorWarning;               // R/W
    ULONG   ErrorPassive;               // R/W
    ULONG   BusOff;                     // R/W
    ULONG   ArbitrationLost;            // R/W
    ULONG   RxBusError;                 // R/W
    ULONG   RxFIFOOverrun;              // R/W
    ULONG   PowerSavingExit;            // R/W
    
}HwiInterruptEnableReg, *PHwiInterruptEnableReg;

VOID
_SetHwiInterruptEnableReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiInterruptEnableReg       pHwiInterruptEnableReg
    );

VOID
_GetHwiInterruptEnableReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiInterruptEnableReg       pHwiInterruptEnableReg
    );


// HwiInterruptStatusReg
typedef struct _HwiInterruptStatusReg
{   
    ULONG   Receive;                    // R/W1C
    ULONG   TxBuffer0Transmit;          // R/W1C
    ULONG   TxBuffer1Transmit;          // R/W1C
    ULONG   TxBuffer2Transmit;          // R/W1C
    ULONG   ErrorWarning;               // R/W1C
    ULONG   ErrorPassive;               // R/W1C
    ULONG   BusOff;                     // R/W1C
    ULONG   ArbitrationLost;            // R/W1C
    ULONG   RxBusError;                 // R/W1C
    ULONG   RxFIFOOverrun;              // R/W1C
    ULONG   PowerSavingExit;            // R/W1C
    ULONG   BusErrorDirection;          // R
    
    
}HwiInterruptStatusReg, *PHwiInterruptStatusReg;

VOID
_SetHwiInterruptStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiInterruptStatusReg       pHwiInterruptStatusReg
    );

VOID
_GetHwiInterruptStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiInterruptStatusReg       pHwiInterruptStatusReg
    );


// HwiGlobalStatusReg
typedef struct _HwiGlobalStatusReg
{   
    ULONG   ReceivingMessage;       // R
    ULONG   TransmittingMessage;    // R
    ULONG   BusOff;                 // R
    ULONG   PowerSavingState;       // R
    ULONG   ErrorPassiveState;      // R
    ULONG   ErrorCounterWarning;    // R
    ULONG   ControllerState;        // R
    
}HwiGlobalStatusReg, *PHwiGlobalStatusReg;

/*VOID
_SetHwiGlobalStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiGlobalStatusReg          pHwiGlobalStatusReg
    );*/

VOID
_GetHwiGlobalStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiGlobalStatusReg          pHwiGlobalStatusReg
    );
    

// HwiRequestReg
typedef struct _HwiRequestReg
{   
    ULONG   TransmitTxBuffer0;      // R/W1
    ULONG   AbortTxBuffer0;         // R/W1
    ULONG   TransmitTxBuffer1;      // R/W1
    ULONG   AbortTxBuffer1;         // R/W1
    ULONG   TransmitTxBuffer2;      // R/W1
    ULONG   AbortTxBuffer2;         // R/W1
    ULONG   ReleaseReceivedMessage; // R/W1
    
}HwiRequestReg, *PHwiRequestReg;

VOID
_SetHwiRequestReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiRequestReg               pHwiRequestReg
    );

VOID
_GetHwiRequestReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRequestReg               pHwiRequestReg
    );


// HwiTxBufferTransmitStatusReg
typedef struct _HwiTxBufferTransmitStatusReg
{
    ULONG   RequestCompleted;       // R
    ULONG   RequestAborted;         // R
    ULONG   TransmissionComplete;   // R
    ULONG   ArbitrationLost;        // R
    ULONG   BusError;               // R
    ULONG   BusOff;                 // R
    ULONG   ArbitrationLostLocation;// R
    ULONG   BusErrorType;           // R
    ULONG   BusErrorLocation;       // R
    
}HwiTxBufferTransmitStatusReg, *PHwiTxBufferTransmitStatusReg;

VOID
_SetHwiTxBufferTransmitStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
    IN      PHwiTxBufferTransmitStatusReg    pHwiTxBufferTransmitStatusReg
    );

VOID
_GetHwiTxBufferTransmitStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
        OUT PHwiTxBufferTransmitStatusReg    pHwiTxBufferTransmitStatusReg
    );


// HwiReceiveStatusReg
typedef struct _HwiReceiveStatusReg
{   
    ULONG   RxFIFOPendingMessages;  // R
    ULONG   RxFIFOOverrun;          // R
    ULONG   BusError;               // R
    ULONG   BusErrorType;           // R
    ULONG   BusErrorLocation;       // R
    
}HwiReceiveStatusReg, *PHwiReceiveStatusReg;

/*VOID
_SetHwiReceiveStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiReceiveStatusReg         pHwiReceiveStatusReg
    );*/

VOID
_GetHwiReceiveStatusReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiReceiveStatusReg         pHwiReceiveStatusReg
    );


// HwiErrorWarningLimitReg
typedef struct _HwiErrorWarningLimitReg
{
    ULONG   WarningLimit;   // R/W
    
}HwiErrorWarningLimitReg, *PHwiErrorWarningLimitReg;

VOID
_SetHwiErrorWarningLimitReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiErrorWarningLimitReg     pHwiErrorWarningLimitReg
    );

VOID
_GetHwiErrorWarningLimitReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiErrorWarningLimitReg     pHwiErrorWarningLimitReg
    );
    
// HwiErrorCounterReg
typedef struct _HwiErrorCounterReg
{   
    ULONG   TxErrorCounter;   // R/W
    ULONG   RxErrorCounter;   // R/W
    
}HwiErrorCounterReg, *PHwiErrorCounterReg;

VOID
_SetHwiErrorCounterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiErrorCounterReg          pHwiErrorCounterReg
    );

VOID
_GetHwiErrorCounterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiErrorCounterReg          pHwiErrorCounterReg
    );


// HwiIdentifierIndexReg
typedef struct _HwiIdentifierIndexReg
{
    ULONG   FilterIndex;    // R/W
    ULONG   FilterUpdate;   // R/W1
    
}HwiIdentifierIndexReg, *PHwiIdentifierIndexReg;

VOID
_SetHwiIdentifierIndexReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiIdentifierIndexReg       pHwiIdentifierIndexReg
    );

VOID
_GetHwiIdentifierIndexReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiIdentifierIndexReg       pHwiIdentifierIndexReg
    );
    

// HwiIdentifierFilterReg
typedef struct _HwiIdentifierFilterReg
{   
    ULONG   ID;             // R/W
    ULONG   ExtendedID;     // R/W
    ULONG   Enable;         // R/W
    
}HwiIdentifierFilterReg, *PHwiIdentifierFilterReg;

VOID
_SetHwiIdentifierFilterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiIdentifierFilterReg      pHwiIdentifierFilterReg
    );

VOID
_GetHwiIdentifierFilterReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiIdentifierFilterReg      pHwiIdentifierFilterReg
    );
    

// HwiIdentifierMaskReg
typedef struct _HwiIdentifierMaskReg
{
    ULONG   Mask;           // R/W
    
}HwiIdentifierMaskReg, *PHwiIdentifierMaskReg;

VOID
_SetHwiIdentifierMaskReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiIdentifierMaskReg        pHwiIdentifierMaskReg
    );

VOID
_GetHwiIdentifierMaskReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiIdentifierMaskReg        pHwiIdentifierMaskReg
    );


// HwiTFIReg
typedef struct _HwiTFIReg
{   
    ULONG   ExtendedFrame;      // R/W
    ULONG   RTRBit;             // R/W
    ULONG   DataLengthCode;     // R/W
    
}HwiTFIReg, *PHwiTFIReg;

VOID
_SetHwiTFIReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
    IN      PHwiTFIReg                   pHwiTFIReg
    );

VOID
_GetHwiTFIReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,  // 0, 1, 2
        OUT PHwiTFIReg                   pHwiTFIReg
    );


// HwiTIDReg
typedef struct _HwiTIDReg
{
    ULONG   ID;     // R/W
    
}HwiTIDReg, *PHwiTIDReg;

VOID
_SetHwiTIDReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
    IN      PHwiTIDReg                   pHwiTIDReg
    );

VOID
_GetHwiTIDReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
        OUT PHwiTIDReg                   pHwiTIDReg
    );


// HwiTDLReg
typedef struct _HwiTDLReg
{   
    ULONG   Data;   // R/W, byte0-3
    
}HwiTDLReg, *PHwiTDLReg;

VOID
_SetHwiTDLReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
    IN      PHwiTDLReg                   pHwiTDLReg
    );

VOID
_GetHwiTDLReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
        OUT PHwiTDLReg                   pHwiTDLReg
    );

    
// HwiTDHReg
typedef struct _HwiTDHReg
{
    ULONG   Data;   // R/W,  byte4-7
    
}HwiTDHReg, *PHwiTDHReg;

VOID
_SetHwiTDHReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
    IN      PHwiTDHReg                   pHwiTDHReg
    );

VOID
_GetHwiTDHReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      ULONG                       TxBufferIndex,
        OUT PHwiTDHReg                   pHwiTDHReg
    );


// HwiRFIReg
typedef struct _HwiRFIReg
{   
    ULONG   ExtendedFrame;      // R
    ULONG   RTRBit;             // R
    ULONG   DataLengthCode;     // R
    
}HwiRFIReg, *PHwiRFIReg;

/*VOID
_SetHwiRFIReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiRFIReg                   pHwiRFIReg
    );*/

VOID
_GetHwiRFIReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRFIReg                   pHwiRFIReg
    );


// HwiRIDReg
typedef struct _HwiRIDReg
{
    ULONG   ID;     // R
    
}HwiRIDReg, *PHwiRIDReg;

/*VOID
_SetHwiRIDReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiRIDReg                   pHwiRIDReg
    );*/

VOID
_GetHwiRIDReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRIDReg                   pHwiRIDReg
    );


// HwiRDLReg
typedef struct _HwiRDLReg
{   
    ULONG   Data;   // R, byte0-3
    
}HwiRDLReg, *PHwiRDLReg;

/*VOID
_SetHwiRDLReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiRDLReg                   pHwiRDLReg
    );*/

VOID
_GetHwiRDLReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRDLReg                   pHwiRDLReg
    );

    
// HwiRDHReg
typedef struct _HwiRDHReg
{
    ULONG   Data;   // R,  byte4-7
    
}HwiRDHReg, *PHwiRDHReg;

/*VOID
_SetHwiRDHReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
    IN      PHwiRDHReg                   pHwiRDHReg
    );*/

VOID
_GetHwiRDHReg(
    IN      PMappedBusAddress           hMappedCANRegisters,
        OUT PHwiRDHReg                   pHwiRDHReg
    );







#endif

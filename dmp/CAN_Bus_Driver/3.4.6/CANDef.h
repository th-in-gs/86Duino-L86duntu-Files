#ifndef CANDef_H
#define CANDef_H

//-----
//  CANMessage
//

#define CANMessage_FrameFormat_Standard     0
#define CANMessage_FrameFormat_Extended     1

#define CANMessage_FrameType_Data       0
#define CANMessage_FrameType_Remote     1

typedef struct _CANMessage
{
    ULONG   FrameFormat;
    ULONG   ID;
    ULONG   FrameType;
    ULONG   DataLength;     // 0 ~ 8
    UCHAR   Data[8];
}CANMessage, *PCANMessage;

//-----
//  CANFilter
//

typedef struct _CANFilter
{
    ULONG   bEnable;
    ULONG   bExtendedID;
    ULONG   ID;
    ULONG   Mask;
}CANFilter, *PCANFilter;


//----
//  CANGlobalControl
//

typedef struct _CANGlobalControl
{
    ULONG   bSilent;
    ULONG   bTxRxPinLoopback;
    ULONG   bTransmitWithNoACK;
    ULONG   bSelfReception;
    ULONG   TransmitPriorityMode;       // 0: buffer index, 1: ID, 2: round robin
    ULONG   bArbitrationLostRetry;
    ULONG   bBusErrorRetry;
    ULONG   bPowerSaving;
    ULONG   bRxBusError;
    
}CANGlobalControl, *PCANGlobalControl;


//----
//  CANInterrupt
//

typedef struct _CANInterrupt
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
    
    //ULONG   BusErrorDirection;          // R
    
}CANInterrupt, *PCANInterrupt;

//----
//  CANTransmitStatus

typedef struct _CANTransmitStatus
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
    
}CANTransmitStatus, *PCANTransmitStatus;


//----
//  CANReceiveStatus

typedef struct _CANReceiveStatus
{
    ULONG   RxFIFOPendingMessages;  // R
    ULONG   RxFIFOOverrun;          // R
    ULONG   BusError;               // R
    ULONG   BusErrorType;           // R
    ULONG   BusErrorLocation;       // R
    
}CANReceiveStatus, *PCANReceiveStatus;


typedef struct _CANTxBuffer
{
    ULONG   bExtendedFrame;
    ULONG   ID;
    ULONG   RTRBit;
    ULONG   DataLengthCode;
    ULONG   Byte0_3;
    ULONG   Byte4_7;
    
}CANTxBuffer, *PCANTxBuffer;

typedef struct _CANRxBuffer
{
    ULONG   bExtendedFrame;
    ULONG   ID;
    ULONG   RTRBit;
    ULONG   DataLengthCode;
    ULONG   Byte0_3;
    ULONG   Byte4_7;
    
}CANRxBuffer, *PCANRxBuffer;


//----
//  CANVolatileInfo
//  App may not use this structure.
typedef struct _CANVolatileInfo
{
    // volatile info
    
    ULONG               NumRxCANMessage;
    CANRxBuffer         sCANRxBuffer;
    CANRxBuffer         sCANRxBuffer2;
    //CANRxBuffer         sCANRxBuffer3;
    
    CANReceiveStatus    sCANReceiveStatus;
    //CANReceiveStatus    sCANReceiveStatus2;
    
    ULONG               BusErrorDirection;
    
    ULONG               TxErrorCounter;
    ULONG               RxErrorCounter;
    
    // debug info
    ULONG   dummy;
}CANVolatileInfo, *PCANVolatileInfo;

//-----
//  CANDebugInfo
//

typedef struct _CANDebugInfo
{
    ULONG   dummy;
}CANDebugInfo, *PCANDebugInfo;


#endif

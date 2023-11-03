#ifndef DrvDebug_H
#define DrvDebug_H

//#define   DPFLTR_ERROR_LEVEL     0
//#define   DPFLTR_WARNING_LEVEL   1
//#define   DPFLTR_TRACE_LEVEL     2
//#define   DPFLTR_INFO_LEVEL      3


#define DMT_Error      1
#define DMT_Warning    2
#define DMT_Trace      4
#define DMT_Info       8

/*
VOID
DriverDbgPrint(
    IN      ULONG                       DebugMessageType,
    IN      PCHAR                       DebugMessageFormat,
    ...
    );

VOID
DriverDbgLog(
    IN      ULONG                       DebugMessageType,
    IN      PCHAR                       DebugMessageFormat,
    ...
    );

#define DrvDbgShallNotRunToHere DriverDbgPrint(DMT_Error, "the program shall not run to here. %s %d\n", __FILE__, __LINE__)
*/

VOID
DrvDbgPrint1(
    IN      PCHAR                       ModuleName,
    IN      PCHAR                       DebugMessageFormat,
    ...
    );

#define DBGPRINTF   1
#ifdef DBGPRINTF
    //#define DrvDbgPrint(DebugPrintLevel, format, arg...) printk(KERN_INFO "%s: ", DRIVER_NAME); printk(KERN_INFO format, ## arg)
    #define DrvDbgPrint(DebugPrintLevel, format, arg...) DrvDbgPrint1(DRIVER_NAME, format, ## arg)
    
    //#define DrvDbgPrint(DebugPrintLevel, format, arg...) printk(KERN_INFO format, ## arg)
    #define DrvDbgPrintIf(bShow, format, arg...) if (bShow) { printk(KERN_INFO format, ## arg); }
    #define DrvDbgShallNotRunToHere DrvDbgPrint(0, "the program shall not run to here. %s %d\n", __FILE__, __LINE__)
#else
    #define DrvDbgPrint(DebugPrintLevel, format, arg...)
    #define DrvDbgShallNotRunToHere DrvDbgPrint(0, "the program shall not run to here. %s %d\n", __FILE__, __LINE__)
#endif
/*
#if DBG == 1
    #define DrvDbgPrint DriverDbgPrint
    
    #define LockTrace(Lock) Lock = __LINE__
    
    #define FSMTraceStart(FSM) FSM = 1
    
    #define FSMTrace(FSM) FSM = __LINE__
    
    #define FSMTraceEnd(FSM) FSM = 0xFFFF
    
    #define DbgCodeStart(Enable) if (Enable) {
    
    #define DbgCodeEnd() }
    
    #define DbgMeterClear SPMeter_Initialize
    
    #define DbgMeterStart SPMeter_Start
    
    #define DbgMeterStop SPMeter_Stop
    
    #define DbgMeterPrint SPMeter_DbgPrint
    
#else
    #define DrvDbgPrint
    
    #define LockTrace(Lock) 
    
    #define FSMTraceStart(FSM) 
    
    #define FSMTrace(FSM) 
    
    #define FSMTraceEnd(FSM) 
    
    #define DbgCodeStart(Enable) if (0) {
    
    #define DbgCodeEnd() }
    
    #define DbgMeterClear 
    
    #define DbgMeterStart
    
    #define DbgMeterStop
    
    #define DbgMeterPrint 
    
#endif

// redefine/override
#pragma warning(disable:4005)
    
    //#define DrvDbgPrint
    
    #define LockTrace(Lock) 
    
    #define FSMTraceStart(FSM) 
    
    #define FSMTrace(FSM) 
    
    #define FSMTraceEnd(FSM)
    
    #define DbgCodeStart(Enable) if (0) {
    
    #define DbgCodeEnd() }
    
    //#define DbgMeterClear 
    
    //#define DbgMeterStart
    
    //#define DbgMeterStop
    
    //#define DbgMeterPrint 
*/
#endif
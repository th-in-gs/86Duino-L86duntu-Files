#ifndef SystemPorting_H
#define SystemPorting_H

// SP SystemPorting
// SPF SystemPortingFunction

typedef struct _SPEvent
{
    struct completion           event;
}SPEvent, *PSPEvent;

VOID
SPEvent_Initialize(
    IN      PSPEvent    hSPEvent
    );

VOID
SPEvent_Clear(
    IN      PSPEvent    hSPEvent
    );
    
VOID
SPEvent_Set(
    IN      PSPEvent    hSPEvent
    );
    
ULONG
SPEvent_Wait(
    IN      PSPEvent    hSPEvent,
    IN      ULONG       Timeoutms,
        OUT PULONG      pbTimeout
    );

// Semaphore can be used in DPC and passive.
typedef struct _SPSemaphore
{
    struct semaphore            lock;
    
}SPSemaphore, *PSPSemaphore;

VOID
SPSemaphore_Initialize(
    IN      PSPSemaphore    hSPSemaphore,
    IN      ULONG           Limit       // if Limit = 1, SPSemaphore = SPMutex
    );

VOID
SPSemaphore_Acquire(
    IN      PSPSemaphore    hSPSemaphore
    );
    
VOID
SPSemaphore_Release(
    IN      PSPSemaphore    hSPSemaphore
    );

// spinlock can be used in DPC, passive.
// In Linux, spinlock also can be used in ISR.
// dont call wait function between acquire and relese
typedef struct _SPSpinLock
{
    spinlock_t                  lock;
    
}SPSpinLock, *PSPSpinLock;
    
VOID
SPSpinLock_Initialize(
    IN      PSPSpinLock     hSPSpinLock
    );

VOID
SPSpinLock_Acquire(
    IN      PSPSpinLock     hSPSpinLock
    );
    // this is spin_trylock_bh.
    // In general, spinlock use spin_trylock_bh in ioctl rountine(WorkQueue, system call)

VOID
SPSpinLock_Release(
    IN      PSPSpinLock     hSPSpinLock
    );

VOID
SPSpinLock_AcquireAtDpcLevel(
    IN      PSPSpinLock     hSPSpinLock,
    IN      PUINT           pflags
    );
    // this is spin_lock_irqsave.
    // In general, spinlock use spin_lock_irqsave in DPC rountine(tasklet, run in soft interrupt)

VOID
SPSpinLock_ReleaseFromDpcLevel(
    IN      PSPSpinLock     hSPSpinLock,
    IN      PUINT           pflags
    );


typedef struct _SPInterruptLock
{
    spinlock_t                  lock;
    
}SPInterruptLock, *PSPInterruptLock;
    // In general, spinlock use spin_lock in irq rountine.
    
VOID
SPInterruptLock_Initialize(
    IN      PSPInterruptLock    hSPInterruptLock
    );

VOID
SPInterruptLock_Acquire(
    IN      PSPInterruptLock    hSPInterruptLock
    );
    // this is spin_trylock_irq.
    // In general, spinlock use spin_trylock_irq in ioctl rountine(WorkQueue, system call)
    
VOID
SPInterruptLock_Release(
    IN      PSPInterruptLock    hSPInterruptLock
    );

VOID
SPInterruptLock_AcquireAtDpcLevel(
    IN      PSPInterruptLock    hSPInterruptLock,
    IN      PUINT               pflags
    );
    // this is spin_lock_irqsave.
    // In general, spinlock use spin_lock_irqsave in DPC rountine(tasklet, run in soft interrupt)

VOID
SPInterruptLock_ReleaseFromDpcLevel(
    IN      PSPInterruptLock    hSPInterruptLock,
    IN      PUINT               pflags
    );

// memory operation

PVOID
SPAllocatePoolWithTag(
    IN      ULONG           bPagedPool,
    IN      ULONG           NumberOfBytes,
    IN      ULONG           Tag
    );

VOID
SPFreePool(
    IN      PVOID           hBuffer
    );

VOID
SPZeroMemory(
    IN      VOID            *Destination,
    IN      ULONG           Length
    );

VOID 
SPCopyMemory(
    IN      VOID            *Destination,
    IN      VOID            *Source,
    IN      ULONG           Length
    );

typedef struct _SPTime
{
    struct timeval  start;
    struct timeval  end;
    
    // typedef long		__kernel_time_t;
}SPTime, *PSPTime;

VOID
SPTime_StartTime(
    IN      PSPTime         hSPTime
    );

VOID
SPTime_EndTime(
    IN      PSPTime         hSPTime
    );

VOID
SPTime_EscapeTime(
    IN      PSPTime         hSPTime,
        OUT PULONGLONG      pms
    );

#endif
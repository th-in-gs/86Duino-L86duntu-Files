#include <linux/kernel.h>

#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/sched.h>

#include <linux/time.h>
#include <linux/slab.h>     // kzalloc

#include "SPtype.h"
#include "SystemPorting.h"

VOID
SPEvent_Initialize(
    IN      PSPEvent    hSPEvent
    )
{
    init_completion(&hSPEvent->event);
}

VOID
SPEvent_Clear(
    IN      PSPEvent    hSPEvent
    )
{
    init_completion(&hSPEvent->event);
}

VOID
SPEvent_Set(
    IN      PSPEvent    hSPEvent
    )
{
    complete(&hSPEvent->event);
}


ULONG
SPEvent_Wait(
    IN      PSPEvent    hSPEvent,
    IN      ULONG       Timeoutms,
        OUT PULONG      pbTimeout
    )
{
    ULONG                       timeout;
    
    *pbTimeout = FALSE;
    
    timeout = wait_for_completion_interruptible_timeout(&hSPEvent->event, msecs_to_jiffies(Timeoutms));
    if (timeout == 0)
    {
        *pbTimeout = TRUE;
    }
    
    return TRUE;
}

VOID
SPSemaphore_Initialize(
    IN      PSPSemaphore    hSPSemaphore,
    IN      ULONG           Limit
    )
{
    sema_init(&hSPSemaphore->lock, Limit);
    
}

VOID
SPSemaphore_Acquire(
    IN      PSPSemaphore    hSPSemaphore
    )
{
    down(&hSPSemaphore->lock);

}
    
VOID
SPSemaphore_Release(
    IN      PSPSemaphore    hSPSemaphore
    )
{
    up(&hSPSemaphore->lock);

}

VOID
SPSpinLock_Initialize(
    IN      PSPSpinLock     hSPSpinLock
    )
{
    spin_lock_init(&hSPSpinLock->lock);
    
}

VOID
SPSpinLock_Acquire(
    IN      PSPSpinLock     hSPSpinLock
    )
{
    //spin_lock(&hSPSpinLock->lock);
    
    while(spin_trylock_bh(&hSPSpinLock->lock) == 0)
    {
        schedule();
    }
    
}

VOID
SPSpinLock_Release(
    IN      PSPSpinLock     hSPSpinLock
    )
{
    //spin_unlock(&hSPSpinLock->lock);
    
    spin_unlock_bh(&hSPSpinLock->lock);
}


VOID
SPSpinLock_AcquireAtDpcLevel(
    IN      PSPSpinLock     hSPSpinLock,
    IN      PUINT           pflags
    )
{
    // verified
    spin_lock_irqsave(&hSPSpinLock->lock, *pflags);
    
}

VOID
SPSpinLock_ReleaseFromDpcLevel(
    IN      PSPSpinLock     hSPSpinLock,
    IN      PUINT           pflags
    )
{
    // verified
    spin_unlock_irqrestore(&hSPSpinLock->lock, *pflags);
    
}

VOID
SPInterruptLock_Initialize(
    IN      PSPInterruptLock    hSPInterruptLock
    )
{
    spin_lock_init(&hSPInterruptLock->lock);
}

VOID
SPInterruptLock_Acquire(
    IN      PSPInterruptLock     hSPInterruptLock
    )
{
    //if (hSPInterruptLock->Interrupt != NULL)
    {
        //spin_lock_irq(&hSPInterruptLock->lock);
        
        while(spin_trylock_irq(&hSPInterruptLock->lock) == 0)
        {
            schedule();
        }
    }
}

VOID
SPInterruptLock_Release(
    IN      PSPInterruptLock     hSPInterruptLock
    )
{
    //if (hSPInterruptLock->Interrupt != NULL)
    {
        spin_unlock_irq(&hSPInterruptLock->lock);
    }
}


PVOID
SPAllocatePoolWithTag(
    IN      ULONG           bPagedPool,
    IN      ULONG           NumberOfBytes,
    IN      ULONG           Tag
    )
{
    PVOID       hBuffer;
    
    if (bPagedPool == TRUE)
    {
        hBuffer = kzalloc(
            NumberOfBytes,
            GFP_KERNEL
            );
    }
    else
    {
        hBuffer = kzalloc(
            NumberOfBytes,
            GFP_ATOMIC
            );
    }
    
    return hBuffer;
}

VOID
SPFreePool(
    IN      PVOID           hBuffer
    )
{
    if (hBuffer != 0)
    {
        kfree(hBuffer);
    }
}

VOID
SPZeroMemory(
    IN      VOID            *Destination,
    IN      ULONG           Length
    )
{
    memset(Destination, 0, Length);
}

VOID 
SPCopyMemory(
    IN      VOID            *Destination,
    IN      VOID            *Source,
    IN      ULONG           Length
    )
{
    memcpy(Destination, Source, Length);
}

VOID
SPTime_StartTime(
    IN      PSPTime         hSPTime
    )
{
    do_gettimeofday(&hSPTime->start);
}

VOID
SPTime_EndTime(
    IN      PSPTime         hSPTime
    )
{
    do_gettimeofday(&hSPTime->end);
}

VOID
SPTime_EscapeTime(
    IN      PSPTime         hSPTime,
        OUT PULONGLONG      pms
    )
{
    __kernel_time_t             sec;
    __kernel_time_t             usec;
    
    sec = hSPTime->end.tv_sec - hSPTime->start.tv_sec;
    usec = hSPTime->end.tv_usec - hSPTime->start.tv_usec;
    
    if (sec < 0)
    {
        *pms = 0;
        return; 
    }
    else if (sec == 0)
    {
        if (usec < 0)
        {
           *pms = 0;
            return; 
        }
    }
    
    if (usec > 0)
    {
    }
    else
    {
        sec -= 1;
        usec += 1000000;
    }
    
    *pms = sec*1000 + usec/1000;
}
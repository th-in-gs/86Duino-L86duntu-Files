/*
    This module contains the function of debug.
*/

#include <linux/kernel.h>
#include <stdarg.h>

#include "SPtype.h"
#include "DrvDebug.h"

VOID
DrvDbgPrint1(
    IN      PCHAR                       ModuleName,
    IN      PCHAR                       DebugMessageFormat,
    ...
    )
{
    va_list                     args;
    UCHAR                       Buffer1[256];
    UCHAR                       Buffer2[512];
    
    //
    // initialize local variables
    //
    
    //
    // check parameters
    //
    
    //
    // function body
    //
    
    va_start(args, DebugMessageFormat);
    {
        sprintf(Buffer1, "%s: %s", ModuleName, DebugMessageFormat);
        vsprintf(Buffer2, Buffer1, args);
        printk(Buffer2);
    }

    va_end(args);
    
    //goto funcexit;
//funcexit:

    //
    // free local buffers
    //
    
    //
    // unsuccessful handle
    //
    
    //return funcresult;
}


#ifndef SPtype_H
#define SPtype_H

#define TRUE    1
#define FALSE   0

#ifndef NULL
#define NULL    0
#endif

#define IN
#define OUT
#define VOID void

/*
// in Linux

int8_t     8-bit signed interger
int16_t    16-bit signed interger
int32_t    32-bit signed interger
int64_t    64-bit signed interger
uint8_t    8-bit unsigned interger
uint16_t   16-bit unsigned interger
uint32_t   32-bit unsigned interger
uint64_t   64-bit unsigned interger

// in Windows

INT8       8-bit signed integer
INT16      16-bit signed integer
INT32      32-bit signed integer
INT64      64-bit signed integer
UINT8      8-bit unsigned integer
UINT16     16-bit unsigned integer
UINT32     32-bit unsigned integer
UINT64     64-bit unsigned integer
*/

//1.In Linux, 
//  if OS is 32bit, sizeof(long) == 4
//  if OS is 64bit, sizeof(long) == 8
//2.In Windows
//  if OS is 32bit, sizeof(long) == 4
//  if OS is 64bit, sizeof(long) == 4

//#define InWindows
#define InLinux
//#define InDos

#ifdef InDos   // _INT16
    typedef short INT, *PINT;
    typedef unsigned short UINT, *PUINT;
#endif

#ifdef InWindows
    typedef int     INT,    *PINT;
    typedef unsigned int UINT,    *PUINT;
    
    typedef INT8    CHAR,   *PCHAR;     // 1
    typedef INT16   SHORT,  *PSHORT;    // 2
    typedef INT32   LONG,   *PLONG;     // 4
    typedef INT64   DLONG,  *PDLONG;    // 8
    
    //typedef int64_t INTPTR;
    typedef UINT UINTPTR;
    
    typedef UINT8    UCHAR, *PUCHAR;    // 1
    typedef UINT16   USHORT,*PUSHORT;   // 2
    typedef UINT32   ULONG, *PULONG;    // 4
    typedef UINT64   UDLONG,*PUDLONG;   // 8
#endif

#ifdef InLinux
    // we dont use unsigned long to declare a variable.
    
    typedef long    INT,    *PINT;
    typedef unsigned long UINT,    *PUINT;
    
    typedef int8_t  CHAR,   *PCHAR;     // 1
    typedef int16_t SHORT,  *PSHORT;    // 2
    typedef int32_t LONG,   *PLONG;     // 4
    typedef int32_t LONG4,  *PLONG4;    // 4
    typedef int64_t DLONG,  *PDLONG;    // 8
    typedef int64_t DLONG4, *PDLONG4;   // 8
    
    //typedef int64_t INTPTR;
    //typedef uintptr_t UINTPTR;
    typedef UINT UINTPTR;
    
    typedef uint8_t  UCHAR, *PUCHAR;    // 1
    typedef uint16_t USHORT,*PUSHORT;   // 2
    typedef uint32_t ULONG, *PULONG;    // 4
    typedef uint64_t UDLONG,*PUDLONG;   // 8
    
    
#endif


typedef union _QUAD {
    struct {
        ULONG   LowPart;
        LONG    HighPart;
    };
    DLONG       QuadPart;
}QUAD, *PQUAD;

typedef union _UQUAD {
    struct {
        ULONG   LowPart;
        ULONG   HighPart;
    };
    DLONG       QuadPart;
}UQUAD, *PUQUAD;

typedef void *PVOID;
typedef PVOID HANDLE;

typedef SHORT WCHAR, *PWCHAR;    // wc,   16-bit UNICODE character

//#define REG1    register
//#define REG2    register
//#define REG3    register
//#define REG4
//#define REG5
//#define REG6
//#define REG7
//#define REG8
//#define REG9

typedef DLONG LONGLONG, *PLONGLONG;
typedef UDLONG ULONGLONG, *PULONGLONG;


typedef union _LARGE_INTEGER {
    struct {
        ULONG   LowPart;
        LONG    HighPart;
    };
    LONGLONG    QuadPart;
}LARGE_INTEGER, *PLARGE_INTEGER;

typedef union _LARGE_UINTEGER {
    struct {
        ULONG   LowPart;
        ULONG   HighPart;
    };
    ULONGLONG   QuadPart;
}LARGE_UINTEGER, *PLARGE_UINTEGER;


typedef LARGE_UINTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS;

#endif

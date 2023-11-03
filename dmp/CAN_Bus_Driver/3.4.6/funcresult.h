#ifndef funcresult_H
#define funcresult_H

//
//
typedef ULONG FRSTATUS;

//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

//
// Define the facility codes
//
#define FACILITY_GenericFunction        0x0


#define FR_Success                      (0x00000000L)

#define FR_Info                         (0x40000000L)

#define FR_Warning                      (0x80000000L)

#define FR_Error                        (0xC0000000L)

#define FR_SUCCESS(funcresult)          ((funcresult & 0xC0000000L) == FR_Success)

#define FR_INFO(funcresult)             ((funcresult & 0xC0000000L) == FR_Info)

#define FR_WARNING(funcresult)          ((funcresult & 0xC0000000L) == FR_Warning)
// program can work, but there is a warning

#define FR_ERROR(funcresult)            ((funcresult & 0xC0000000L) == FR_Error)
// program can not work becasue FR_ERROR



#endif
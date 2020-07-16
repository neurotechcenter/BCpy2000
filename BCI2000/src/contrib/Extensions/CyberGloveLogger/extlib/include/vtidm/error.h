/*============================================================================

  $Id: error.h,v 1.2 2002/10/11 22:33:17 dgomez Exp $

  HISTORY:

 ============================================================================*/

#ifndef _VT_ERROR_H_
#define _VT_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

/*  error codes */
#include "types.h"

/* common errors */
#define VTERR_BASE                      0
#define VTERR_NO_ERROR                  (VTERR_BASE - 0)
#define VTERR_GENERIC_ERROR             (VTERR_BASE - 1)
#define VTERR_INVALID_PARAM             (VTERR_BASE - 2)
#define VTERR_OUT_OF_MEMORY             (VTERR_BASE - 3)
#define VTERR_BUFFER_TOO_SMALL          (VTERR_BASE - 4)
#define VTERR_NOT_IMPLEMENTED           (VTERR_BASE - 60)

#define VTERR_INVALID_PLUGIN_DIR        (VTERR_BASE - 5)
#define VTERR_INVALID_PLUGIN            (VTERR_BASE - 6)
#define VTERR_PLUGIN_INCOMPATIBLE_VERSION (VTERR_BASE - 7)
#define VTERR_PLUGIN_NOT_INITIALIZED    (VTERR_BASE - 8)

#define VTERR_INVALID_INSTANCE          (VTERR_BASE - 9)
#define VTERR_INSTANCE_SHARED_BY_OTHERS (VTERR_BASE - 10)
#define VTERR_INVALID_FUNCTABLE         (VTERR_BASE - 11)

#define VTERR_INVALID_MESSAGE           (VTERR_BASE - 12)
#define VTERR_MESSAGE_NOT_IMPLEMENTED   (VTERR_BASE - 13)
#define VTERR_MESSAGE_INVALID_ARGCODE   (VTERR_BASE - 14)
#define VTERR_MESSAGE_INVALID_BODY      (VTERR_BASE - 15)
#define VTERR_MESSAGE_INVALID_KEY       (VTERR_BASE - 16)
#define VTERR_MESSAGE_TOO_LARGE         (VTERR_BASE - 17)
#define VTERR_MESSAGE_DELIVER_ERROR     (VTERR_BASE - 18)
#define VTERR_MESSAGE_INCOMPLETE        (VTERR_BASE - 19)

#define VTERR_INVALID_DEVICE            (VTERR_BASE - 20)
#define VTERR_DEVICE_NOT_READY          (VTERR_BASE - 21)
#define VTERR_DEVICE_IN_USE             (VTERR_BASE - 22)
#define VTERR_DEVICE_PACKET_SIZE        (VTERR_BASE - 57)
#define VTERR_DEVICE_GENERIC            (VTERR_BASE - 58)
#define VTERR_DEVICE_NO_DATA_TO_REPORT  (VTERR_BASE - 62)
#define VTERR_DEVICE_EXPECT_MORE_DATA   (VTERR_BASE - 63)

#define VTERR_INVALID_SERIAL            (VTERR_BASE - 23)
#define VTERR_SERIAL_READ               (VTERR_BASE - 24)
#define VTERR_SERIAL_WRITE              (VTERR_BASE - 25)
#define VTERR_SERIAL_SELECT             (VTERR_BASE - 26)
#define VTERR_SERIAL_READ_TIMEOUT       (VTERR_BASE - 27)
#define VTERR_SERIAL_WRITE_TIMEOUT      (VTERR_BASE - 28)
#define VTERR_SERIAL_INVALID_BAUDRATE   (VTERR_BASE - 29)

#define VTERR_SOCKET_OPEN               (VTERR_BASE - 30)
#define VTERR_SOCKET_CLOSE              (VTERR_BASE - 31)
#define VTERR_INVALID_SOCKET            (VTERR_BASE - 32)
#define VTERR_SOCKET_WRITE              (VTERR_BASE - 33)
#define VTERR_SOCKET_READ               (VTERR_BASE - 34)
#define VTERR_SOCKET_SELECT             (VTERR_BASE - 35)
#define VTERR_SOCKET_ACCEPT             (VTERR_BASE - 36)
#define VTERR_SOCKET_READ_TIMEOUT       (VTERR_BASE - 37)
#define VTERR_SOCKET_WRITE_TIMEOUT      (VTERR_BASE - 38)
#define VTERR_SOCKET_TOO_MANY           (VTERR_BASE - 56)

#define VTERR_INVALID_URL               (VTERR_BASE - 39)

#define VTERR_INVALID_DATASTREAM        (VTERR_BASE - 40)
#define VTERR_DATASTREAM_IN_PROGRESS    (VTERR_BASE - 41)
#define VTERR_DATASTREAM_READ           (VTERR_BASE - 42)
#define VTERR_DATASTREAM_WRITE          (VTERR_BASE - 43)

#define VTERR_THREAD_TIMEOUT            (VTERR_BASE - 44)
#define VTERR_THREAD_TOO_MANY           (VTERR_BASE - 45)

#define VTERR_INVALID_DID               (VTERR_BASE - 46)

#define VTERR_INVALID_FILE              (VTERR_BASE - 47)
#define VTERR_FILE_OPEN                 (VTERR_BASE - 48)
#define VTERR_FILE_CLOSE                (VTERR_BASE - 49)
#define VTERR_FILE_READ                 (VTERR_BASE - 50)
#define VTERR_FILE_WRITE                (VTERR_BASE - 51)
#define VTERR_FILE_INVALID_CONTENT      (VTERR_BASE - 52)

#define VTERR_IPC_TIMEOUT               (VTERR_BASE - 53)
#define VTERR_IPC_LOCK                  (VTERR_BASE - 54)
#define VTERR_IPC_UNLOCK                (VTERR_BASE - 55)

#define VTERR_SEARCH_NO_MATCH           (VTERR_BASE - 59) 
#define VTERR_CONFIG                    (VTERR_BASE - 61)
#define VTERR_MAX_NUMBER                (VTERR_BASE - 64)

/* next 35 */
#define VTERR_PLUGIN_SPECIFIC_ERROR     (VTERR_BASE - 100)

VT_DLL_PROC void VT_ErrorPrint(vt_error err, char * str);

#ifdef __cplusplus
}
#endif

#endif

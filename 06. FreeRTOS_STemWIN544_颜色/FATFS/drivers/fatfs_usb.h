/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2013          /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED_USB
#define _DISKIO_DEFINED_USB

#include "stm32f4xx.h"
#include "diskio.h"
#include "integer.h"

/* USB timeout max value */
#ifndef FATFS_USB_TIMEOUT
#define FATFS_USB_TIMEOUT	50000
#endif

/*---------------------------------------*/
/* Prototypes for disk control functions */
DSTATUS TM_FATFS_USB_disk_initialize(void);
DSTATUS TM_FATFS_USB_disk_status(void);
DRESULT TM_FATFS_USB_disk_read(BYTE* buff, DWORD sector, UINT count);
DRESULT TM_FATFS_USB_disk_write(const BYTE* buff, DWORD sector, UINT count);
DRESULT TM_FATFS_USB_disk_ioctl(BYTE cmd, void* buff);

#endif


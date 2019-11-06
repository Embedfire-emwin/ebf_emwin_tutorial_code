/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "./drivers/fatfs_usb.h" 

#if	FATFS_USE_USB

#include "./Bsp/usb/usbh_bsp.h"			/* µ×²ãÇý¶¯ */

#define SECTOR_SIZE		512

/*-----------------------------------------------------------------------*/
/* Initialize USB                                                        */
/*-----------------------------------------------------------------------*/
DSTATUS TM_FATFS_USB_disk_initialize(void) {
	DSTATUS stat = STA_NOINIT;
	if(HCD_IsDeviceConnected(&USB_OTG_Core))
	{
		stat &= ~STA_NOINIT;
	}
	return stat;
}

/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS TM_FATFS_USB_disk_status(void) {
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/
DRESULT TM_FATFS_USB_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;
	//res = USB_disk_read(buff, sector, count);
	{
		BYTE status = USBH_MSC_OK;

		//if (Stat & STA_NOINIT) 	return RES_NOTRDY;

		if (HCD_IsDeviceConnected(&USB_OTG_Core))
		{
			do
			{
				status = USBH_MSC_Read10(&USB_OTG_Core, buff,sector,512 * count);
				USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);

				if (!HCD_IsDeviceConnected(&USB_OTG_Core))
				{
					break;
				}
			}
			while (status == USBH_MSC_BUSY );
		}

		if (status == USBH_MSC_OK)
		{
			res = RES_OK;
		}
		else
		{
			res = RES_ERROR;
		}
	}
	return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT TM_FATFS_USB_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	//res = USB_disk_write(buff, sector, count);
	{
		BYTE status = USBH_MSC_OK;
		//if (drv || !count) return RES_PARERR;
		//if (Stat & STA_NOINIT) return RES_NOTRDY;
		//if (Stat & STA_PROTECT) return RES_WRPRT;
		if (HCD_IsDeviceConnected(&USB_OTG_Core))
		{
			do
			{
				status = USBH_MSC_Write10(&USB_OTG_Core,(BYTE*)buff,sector, 512 * count);
				USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

				if(!HCD_IsDeviceConnected(&USB_OTG_Core))
				{
					break;
				}
			}
			while(status == USBH_MSC_BUSY );

		}

		if (status == USBH_MSC_OK)
		{
			res = RES_OK;
		}
		else
		{
			res = RES_ERROR;
		}
	}
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT TM_FATFS_USB_disk_ioctl (
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	//if (drv) return RES_PARERR;
	res = RES_ERROR;
	//if (Stat & STA_NOINIT) return RES_NOTRDY;
	switch (cmd)
	{
		case CTRL_SYNC :		/* Make sure that no pending write process */
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			*(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
			res = RES_OK;
			break;
		case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
			*(WORD*)buff = 512;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */\
			*(DWORD*)buff = 512;
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif

#else   // FATFS_USE_USB

DSTATUS TM_FATFS_USB_disk_initialize(void){return RES_ERROR;}
DSTATUS TM_FATFS_USB_disk_status(void){return RES_ERROR;}
DRESULT TM_FATFS_USB_disk_read(BYTE* buff, DWORD sector, UINT count){return RES_ERROR;}
DRESULT TM_FATFS_USB_disk_write(const BYTE* buff, DWORD sector, UINT count){return RES_ERROR;}
DRESULT TM_FATFS_USB_disk_ioctl(BYTE cmd, void* buff){return RES_ERROR;}

#endif  // FATFS_USE_USB

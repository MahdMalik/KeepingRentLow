#include "sd_driver.h"
#include "spi_setup.h"
#include "usb_setup.h"


static bool isInitialized = false;

static DSTATUS SD_disk_initialize(BYTE lun) {
    if(SetupSdCard() == 0x05)
    {
    	isInitialized = true;
    	return RES_OK;
    }
    return STA_NOINIT;
}

static DSTATUS SD_disk_status(BYTE lun) {
    return isInitialized ? RES_OK : STA_NOINIT;
}

static DRESULT SD_disk_read(BYTE lun, BYTE* buff, DWORD sector, UINT count) {
    for (UINT i = 0; i < count; i++) {
        if (!ReadBlock(sector + i, buff + (i * 512)))
        {
        	return RES_ERROR;
        }
    }
    return RES_OK;
}

static DRESULT SD_disk_write(BYTE lun, const BYTE* buff, DWORD sector, UINT count) {
    return RES_WRPRT; // read-only for now, implement WriteBlock later
}

static DRESULT SD_disk_ioctl(BYTE lun, BYTE cmd, void* buff) {
    switch (cmd) {
        case CTRL_SYNC:       return RES_OK;
        case GET_SECTOR_SIZE: *(WORD*)buff = 512;  return RES_OK;
        case GET_BLOCK_SIZE:  *(DWORD*)buff = 1;   return RES_OK;
        default:              return RES_PARERR;
    }
}

const Diskio_drvTypeDef SD_Driver = {
    SD_disk_initialize,
    SD_disk_status,
    SD_disk_read,
    SD_disk_write,
    SD_disk_ioctl,
};

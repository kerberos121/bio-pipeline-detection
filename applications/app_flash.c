#include "app_flash.h"
#include "board.h"

uint32_t STMFLASH_ReadWord(uint32_t faddr)
{
    return *(__IO uint32_t*)faddr;
}

void STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t addrx, endaddr;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError = 0;

    if (WriteAddr < 0x08000000 || WriteAddr % 4) return;

    HAL_FLASH_Unlock();

    addrx = WriteAddr;
    endaddr = WriteAddr + NumToWrite * 4;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = FLASH_SECTOR_11;
    EraseInitStruct.NbSectors = 1;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (STMFLASH_ReadWord(WriteAddr) != 0xFFFFFFFF) {
        status = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
        if (status != HAL_OK) {
            HAL_FLASH_Lock();
            return;
        }
    }

    while (WriteAddr < endaddr) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, *pBuffer) != HAL_OK) {
            break;
        }
        WriteAddr += 4;
        pBuffer++;
    }

    HAL_FLASH_Lock();
}

void STMFLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead)
{
    for (uint32_t i = 0; i < NumToRead; i++) {
        pBuffer[i] = STMFLASH_ReadWord(ReadAddr);
        ReadAddr += 4;
    }
}

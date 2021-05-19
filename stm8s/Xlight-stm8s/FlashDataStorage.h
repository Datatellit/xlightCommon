#ifndef _FLASHDATASTORAGE_H_
#define _FLASHDATASTORAGE_H_
#include "stm8s.h"

// Unique ID
#if defined(STM8S105) || defined(STM8S005) || defined(STM8AF626x)
  #define     UNIQUE_ID_ADDRESS         (0x48CD)
#endif
#if defined(STM8S103) || defined(STM8S003) ||  defined(STM8S903)
  #define     UNIQUE_ID_ADDRESS         (0x4865)
#endif

// Starting Flash block number of backup config
#define CONFIG_DATA_NUM                 0          //(only be writen at config time)
#define BACKUP_CONFIG_BLOCK_NUM         2
#define BACKUP_CONFIG_ADDRESS           (FLASH_DATA_START_PHYSICAL_ADDRESS + BACKUP_CONFIG_BLOCK_NUM * FLASH_BLOCK_SIZE)
#define STATUS_DATA_NUM                 4
#define STATUS_DATA_ADDRESS             (FLASH_DATA_START_PHYSICAL_ADDRESS + STATUS_DATA_NUM * FLASH_BLOCK_SIZE)

uint8_t *Read_UniqueID(uint8_t *UniqueID, const uint16_t Length);

int8_t wait_flashflag_status(uint8_t flag,uint8_t status);
void Flash_ReadBuf(uint32_t Address, uint8_t *Buffer, uint16_t Length);
bool Flash_WriteBuf(uint32_t Address, uint8_t *Buffer, uint16_t Length);
bool Flash_WriteDataBlock(uint16_t nStartBlock, uint8_t *Buffer, uint16_t Length);

#endif
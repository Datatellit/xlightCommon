#ifndef _COMMON_H
#define _COMMON_H

#include "stm8l15x.h"
#include "publicDefine.h"
#include "xliSensorMap.h"

#define WWDG_COUNTER                    0x7f
#define WWDG_WINDOW                     0x77

// Unique ID for STM8L151x4
#define     UNIQUE_ID_ADDRESS           (0x4926)

//------------------------------------------------------------------------------
// STM8L151x4 EEPROM Memory Map (1K = 128 Byte/Page * 8 Pages)
//------------------------------------------------------------------------------
// Page 0 | Page 1 | Page 2 | Page 3 | Page 4 | Page 5 | Page 6 | Page 7 |
//------------------------------------------------------------------------------
//                 |                 |        |                          |
//    SysConfig    |    SysConfig    | Status |     Application          |
//                 |      Backup     |  Data  |         Data             |
//                 |                 |        |                          |
//------------------------------------------------------------------------------

// Starting Flash block number of backup config
#define BACKUP_CONFIG_BLOCK_NUM         2
#define BACKUP_CONFIG_ADDRESS           (FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + BACKUP_CONFIG_BLOCK_NUM * FLASH_BLOCK_SIZE)
#define STATUS_DATA_NUM                 4
#define STATUS_DATA_ADDRESS             (FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + STATUS_DATA_NUM * FLASH_BLOCK_SIZE)
#define EEPROM_APP_DATA_NUM             5
#define EEPROM_APP_DATA_ADDRESS         (FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + EEPROM_APP_DATA_NUM * FLASH_BLOCK_SIZE)
#define EEPROM_APP_DATA_SIZE            (3 * FLASH_BLOCK_SIZE)


// Return NodeType by giving NodeID
UC NodeID2Type(const UC _nid);

bool isIdentityEmpty(const UC *pId, const UC nLen);
bool isIdentityEqual(const UC *pId1, const UC *pId2, const UC nLen);
uint8_t *Read_UniqueID(uint8_t *UniqueID, const uint16_t Length);

void wwdg_init();
void feed_wwdg(void);
int8_t wait_flashflag_status(const uint8_t flag, const uint8_t status);
void Flash_ReadBuf(const uint32_t Address, uint8_t *Buffer, const uint16_t Length);
bool Flash_WriteBuf(const uint32_t Address, uint8_t *Buffer, const uint16_t Length);
bool Flash_WriteDataBlock(const uint16_t nStartBlock, uint8_t *Buffer, const uint16_t Length);

#endif /* _COMMON_H */
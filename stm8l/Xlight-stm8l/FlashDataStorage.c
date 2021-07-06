#include "FlashDataStorage.h"
#include "common.h"
#include "wwdg.h"

// is flash writting
uint8_t flashWritting = 0;

uint8_t *Read_UniqueID(uint8_t *UniqueID, const uint16_t Length)  
{
  Flash_ReadBuf(UNIQUE_ID_ADDRESS, UniqueID, Length);
  return UniqueID;
}

int8_t wait_flashflag_status(const uint8_t flag, const uint8_t status)
{
    uint16_t timeout = 60000;
    while( FLASH_GetFlagStatus(flag)== status && timeout--) {
      feed_wwdg();
    }
    if(!timeout) {
      return 1;
    }
    return 0;
}

void Flash_ReadBuf(const uint32_t Address, uint8_t *Buffer, const uint16_t Length) {
  assert_param(IS_FLASH_ADDRESS(Address));
  assert_param(IS_FLASH_ADDRESS(Address+Length));
  
  for( uint16_t i = 0; i < Length; i++ ) {
    Buffer[i] = FLASH_ReadByte(Address+i);
  }
}

bool Flash_WriteBuf(const uint32_t Address, uint8_t *Buffer, const uint16_t Length) {
  assert_param(IS_FLASH_DATA_EEPROM_ADDRESS(Address));
  assert_param(IS_FLASH_DATA_EEPROM_ADDRESS(Address+Length));
  if( flashWritting == 1 ) {
    return FALSE;
  }
  flashWritting = 1;
  // Init Flash Read & Write
  FLASH_SetProgrammingTime(FLASH_ProgramMode_Standard);

  FLASH_Unlock(FLASH_MemType_Data);
  /* Wait until Data EEPROM area unlocked flag is set*/
  //while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
  if(wait_flashflag_status(FLASH_FLAG_DUL,RESET)) return FALSE;
  // Write byte by byte
  bool rc = TRUE;
  uint8_t bytVerify, bytAttmpts;
  for( uint16_t i = 0; i < Length; i++ ) {
    bytAttmpts = 0;
    while(++bytAttmpts <= 3) {
      FLASH_ProgramByte(Address+i, Buffer[i]);
      FLASH_WaitForLastOperation(FLASH_MemType_Data);
      
      // Read and verify the byte we just wrote
      bytVerify = FLASH_ReadByte(Address+i);
      if( bytVerify == Buffer[i] ) break;
    }
    if( bytAttmpts > 3 ) {
      rc = FALSE;
      break;
    }
  }
  FLASH_Lock(FLASH_MemType_Data);
  flashWritting = 0;
  return rc;
}
 
bool Flash_WriteDataBlock(const uint16_t nStartBlock, uint8_t *Buffer, const uint16_t Length) {
  // Init Flash Read & Write
  if( flashWritting ==  1) {
    return FALSE;
  }
  flashWritting = 1;

  FLASH_SetProgrammingTime(FLASH_ProgramMode_Standard);
  FLASH_Unlock(FLASH_MemType_Data);
  if(wait_flashflag_status(FLASH_FLAG_DUL,RESET)) return FALSE;
  uint8_t WriteBuf[FLASH_BLOCK_SIZE];
  uint16_t nBlockNum = (Length - 1) / FLASH_BLOCK_SIZE + 1;
  for( uint16_t block = nStartBlock; block < nStartBlock + nBlockNum; block++ ) {
    clearBuffer(WriteBuf, FLASH_BLOCK_SIZE);
    uint8_t maxLen = FLASH_BLOCK_SIZE;
    if( block == nStartBlock + nBlockNum - 1 ) {
      maxLen = Length - (nBlockNum -1)*FLASH_BLOCK_SIZE;
    }
    for( uint16_t i = 0; i < maxLen; i++ ) {
      WriteBuf[i] = Buffer[(block - nStartBlock) * FLASH_BLOCK_SIZE + i];
    }
    FLASH_ProgramBlock(block, FLASH_MemType_Data, FLASH_ProgramMode_Standard, WriteBuf);
    // very important( FLASH_WaitForLastOperation(FLASH_MemType_Data) not usefull,cause program dead)
    if(wait_flashflag_status(FLASH_FLAG_EOP,RESET)) return FALSE; 
  }
  FLASH_Lock(FLASH_MemType_Data);

  flashWritting = 0;
  return TRUE;
}

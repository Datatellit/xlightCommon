#include "common.h"
#include "debugDefine.h"

// Return NodeType by giving NodeID
UC NodeID2Type(const UC _nid) {
    eNodeType_t lv_ntype;
    if( _nid == NODEID_GATEWAY ) lv_ntype = NODE_TYP_GW;
    else if( IS_LAMP_NODEID(_nid) ) lv_ntype = NODE_TYP_LAMP;
    else if( IS_COLORBAR_NODEID(_nid) ) lv_ntype = NODE_TYP_COLORBAR;
    else if( IS_SWITCH_NODEID(_nid) ) lv_ntype = NODE_TYP_SWITCH;
    else if( IS_REMOTE_NODEID(_nid) ) lv_ntype = NODE_TYP_REMOTE;
    else if( IS_FAN_NODEID(_nid) ) lv_ntype = NODE_TYP_FAN;
    else if( IS_AC_NODEID(_nid) ) lv_ntype = NODE_TYP_AC;
    else if( IS_IR_NODEID(_nid) ) lv_ntype = NODE_TYP_IR;
    else if( IS_SENSOR_NODEID(_nid) ) lv_ntype = NODE_TYP_SENSOR;
    else if( IS_AIRPURE_NODEID(_nid) ) lv_ntype = NODE_TYP_AIRPURE;
    else if( IS_CURTAIN_NODEID(_nid) ) lv_ntype = NODE_TYP_CURTAIN;
    else if( IS_MODBUS_NODEID(_nid) ) lv_ntype = NODE_TYP_MODBUS;
    else if( IS_SPECIAL_NODEID(_nid) ) lv_ntype = NODE_TYP_SPECIAL;
    else if( IS_GROUP_NODEID(_nid) ) lv_ntype = NODE_TYP_GROUP;
    else if( IS_SYSTEM_NODEID(_nid) ) lv_ntype = NODE_TYP_SYSTEM;
    else lv_ntype = NODE_TYP_THIRDPARTY;
    return (UC)lv_ntype;
}

bool isIdentityEmpty(const UC *pId, const UC nLen)
{
  for( int i = 0; i < nLen; i++ ) { if(pId[i] > 0) return FALSE; }
  return TRUE;
}

bool isIdentityEqual(const UC *pId1, const UC *pId2, const UC nLen)
{
  for( int i = 0; i < nLen; i++ ) { if(pId1[i] != pId2[i]) return FALSE; }
  return TRUE;
}

uint8_t *Read_UniqueID(uint8_t *UniqueID, const uint16_t Length)  
{
  Flash_ReadBuf(UNIQUE_ID_ADDRESS, UniqueID, Length);
  return UniqueID;
}

// Initialize Window Watchdog
void wwdg_init() {
#ifndef DEBUG_NO_WWDG  
  WWDG_Init(WWDG_COUNTER, WWDG_WINDOW);
#endif  
}

// Feed the Window Watchdog
void feed_wwdg(void) {
#ifndef DEBUG_NO_WWDG    
  uint8_t cntValue = WWDG_GetCounter() & WWDG_COUNTER;
  if( cntValue < WWDG_WINDOW ) {
    WWDG_SetCounter(WWDG_COUNTER);
  }
#endif
}

// is flash writting
uint8_t flashWritting = 0;
int8_t wait_flashflag_status(const uint8_t flag, const uint8_t status)
{
    uint16_t timeout = 60000;
    while( FLASH_GetFlagStatus(flag)== status && timeout--) {
      feed_wwdg();
    }
    if(!timeout) {
      //printlog("timeout!");
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
    //printlog("iswriting");
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
    //printlog("iswriting");
    return FALSE;
  }
  flashWritting = 1;

  FLASH_SetProgrammingTime(FLASH_ProgramMode_Standard);
  FLASH_Unlock(FLASH_MemType_Data);
  if(wait_flashflag_status(FLASH_FLAG_DUL,RESET)) return FALSE;
  uint8_t WriteBuf[FLASH_BLOCK_SIZE];
  uint16_t nBlockNum = (Length - 1) / FLASH_BLOCK_SIZE + 1;
  for( uint16_t block = nStartBlock; block < nStartBlock + nBlockNum; block++ ) {
    memset(WriteBuf, 0x00, FLASH_BLOCK_SIZE);
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
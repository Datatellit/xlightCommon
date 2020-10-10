#include "_global.h"
#include "rf24l01.h"
#include "MyMessage.h"

bool gNeedSaveBackup = FALSE;
bool gIsStatusChanged = FALSE;
bool gIsConfigChanged = FALSE;
bool gResetRF = FALSE;
bool gResetNode = FALSE;
bool gResendPresentation = FALSE;

uint8_t _uniqueID[UNIQUE_ID_LEN];

// Load config from Flash
void LoadConfig()
{
    // Load the most recent settings from FLASH
    Flash_ReadBuf(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS, (uint8_t *)&gConfig, sizeof(gConfig));
    if( IsConfigInvalid() ) {
      Flash_ReadBuf(BACKUP_CONFIG_ADDRESS, (uint8_t *)&gConfig, sizeof(gConfig));
      if( IsConfigInvalid() ) {
        clearBuffer((UC *)&gConfig, sizeof(gConfig));
        gConfig.version = XLA_VERSION;
        gConfig.present = 0;
        gConfig.enSDTM = 0;
        gConfig.rptTimes = 1;
        gConfig.nodeID = XLA_PRODUCT_NODEID;
        gConfig.rfChannel = RF24_CHANNEL;
        gConfig.rfPowerLevel = RF24_PA_MAX;
        gConfig.rfDataRate = RF24_250KBPS;   
        gConfig.type = XLA_PRODUCT_Type;
        copyBuffer(gConfig.NetworkID, RF24_BASE_RADIO_ID, ADDRESS_WIDTH);
      }
      gIsConfigChanged = TRUE;
    }
    else {
      uint8_t bytVersion;
      Flash_ReadBuf(BACKUP_CONFIG_ADDRESS, (uint8_t *)&bytVersion, sizeof(bytVersion));
      if( bytVersion != gConfig.version ) gNeedSaveBackup = TRUE;
    }
    // Load the most recent status from FLASH
    uint8_t pData[50] = {0};
    uint16_t nLen = (uint16_t)(&(gConfig.nodeID)) - (uint16_t)(&gConfig);
    Flash_ReadBuf(STATUS_DATA_ADDRESS, pData, nLen);
    if(pData[0] >= XLA_MIN_VER_REQUIREMENT && pData[0] <= XLA_VERSION) {
      copyBuffer((UC *)&gConfig, pData, nLen);
    }
}

// Save config to Flash
void SaveBackupConfig()
{
  if( gNeedSaveBackup ) {
    // Overwrite entire config FLASH
    if(Flash_WriteDataBlock(BACKUP_CONFIG_BLOCK_NUM, (uint8_t *)&gConfig, sizeof(gConfig))) {
      gNeedSaveBackup = FALSE;
    }
  }
}

// Save status to Flash
void SaveStatusData()
{
  if( gIsStatusChanged ) {
    // Skip the first byte (version)
    uint8_t pData[50] = {0};
    uint16_t nLen = (uint16_t)(&(gConfig.nodeID)) - (uint16_t)(&gConfig);
    copyBuffer(pData, (uint8_t *)&gConfig, nLen);
    if(Flash_WriteDataBlock(STATUS_DATA_NUM, pData, nLen)) {
      gIsStatusChanged = FALSE;
    }
  }
}

// Save config to Flash
void SaveConfig()
{
  if( gIsConfigChanged ) {
    // Ensure Status has another chance to be saved even if Saving Config failed
    gIsStatusChanged = TRUE;
    // Overwrite entire config FLASH 
    uint8_t Attmpts = 0;
    while(++Attmpts <= 3) {
      if(Flash_WriteDataBlock(0, (uint8_t *)&gConfig, sizeof(gConfig))) {
        gIsStatusChanged = FALSE;
        gIsConfigChanged = FALSE;
        gNeedSaveBackup = TRUE;
        break;
      }
    }
  }
  SaveStatusData();
}

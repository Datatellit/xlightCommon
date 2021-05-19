#ifndef _XLIGHTCOMBUS_DEV_H_
#define _XLIGHTCOMBUS_DEV_H_

#include "FlashDataStorage.h"

void LoadConfig();
void SaveBackupConfig();
void SaveStatusData();
void SaveConfig();

extern bool gIsConfigChanged;
extern bool gNeedSaveBackup;
extern bool gIsStatusChanged;
extern bool gResetRF;
extern bool gResetNode;
extern bool gSubIDChanged;
extern bool gResendPresentation;

extern uint8_t _uniqueID[UNIQUE_ID_LEN];

#endif
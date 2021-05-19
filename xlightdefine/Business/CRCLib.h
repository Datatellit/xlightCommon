#ifndef _CRC_COMMON_LIB_H_
#define _CRC_COMMON_LIB_H_

#include "publicDefine.h"

uint8_t CRC8_Check(const uint8_t *pushMsg, const uint16_t usDataLen, const uint8_t ucCRCH, const uint8_t ucCRCL);

#endif // _CRC_COMMON_LIB_H_